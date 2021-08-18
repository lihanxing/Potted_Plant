﻿
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

//out vec2 UV;
out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

void main(){
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
//		UV = vertexUV;
		FragPos = vec3(model * vec4(aPos, 1.0f));
		Normal = mat3(transpose(inverse(model))) * aNormal;
		TexCoords = aTex;
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

}
