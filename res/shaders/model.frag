
#version 330 core

struct Material{
		sampler2D diffuse;
		sampler2D specular;

		float shininess;
};

struct Light{
		vec3 position;
		float ambient;
		float diffuse;
		float specular;

		float constant;
		float linear;
		float quadratic;
};

out vec4 color;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
//uniform vec4 extColor;
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace){
		// perform perspective divide
		vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

		// transform to [0,1] range
		projCoords = projCoords * 0.5 + 0.5;

		//get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
		float closestDepth = texture(shadowMap, projCoords.xy).r;

		// get depth of current fragment from light's perspective
		float currentDepth = projCoords.z;

		// calculate bias (based on depth map resolution and slope)
		vec3 normal = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

		// check whether current frag pos is in shadow
				// PCF
		float shadow = 0.0;
		vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		for(int x = -1; x <= 1; ++x){
				for(int y = -1; y <= 1; ++y){
						float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
						shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
				}
		}
		shadow /= 9.0;

		// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
		if(currentDepth > 1.0)
				shadow = 0.0;

		return shadow;
//		return closestDepth;
}


void main(){
		//Ambient
		vec3 ambient = light.ambient * vec3(texture2D(material.diffuse, TexCoords));

		//Diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = light.diffuse * diff * vec3(texture2D(material.diffuse, TexCoords));

		//specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
//  vec3 specular = light.specular * spec * vec3(texture2D(material.specular, TexCoords));
		vec3 specular = light.specular * spec * vec3(texture2D(material.diffuse, TexCoords));

		float distance = length(light.position - FragPos);
		float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

		float shadow = ShadowCalculation(FragPosLightSpace);
//		color = vec4(shadow, shadow, shadow, 1.0f);
		vec3 result = (ambient + (1.0 - shadow) *(diffuse + specular)) * attenuation;
		color = vec4(result, 1.0f);


}
