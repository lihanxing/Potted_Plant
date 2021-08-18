#include "Model.h"

GLuint Model::TextureFromFile(const char * path, string directory)
{
  string filename = string(path);
  filename = directory + '/' + filename;

  GLuint textureID;
  core->glGenTextures(1, &textureID);
  int width, height;

  unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

  core->glBindTexture(GL_TEXTURE_2D, textureID);
  core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  core->glGenerateMipmap(GL_TEXTURE_2D);

  //参数
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  core->glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);

  return textureID;
}

Model::Model(string path)
{
//  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
  this->loadModel(path);

}

void Model::Draw(Shader shader)
{
  for (GLuint i = 0; i < meshes.size(); ++i)
    this->meshes[i].Draw(shader);
}

void Model::loadModel(string path)
{
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
    return;
  }

  this->directory = path.substr(0, path.find_last_of('/'));
  this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
  //cout << "processNode " << node->mNumChildren << endl;
  for (GLuint i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    this->meshes.push_back(this->processMesh(mesh, scene));
  }

  for (GLuint i = 0; i < node->mNumChildren; ++i) {
    //cout << node->mName.C_Str() << " " << node->mNumChildren << endl;
    this->processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

  for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;

    glm::vec3 vector;
    //顶点位置
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.Position = vector;

      //法线
    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.Normal = vector;

    //纹理
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoords = vec;
    }
    else {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (GLuint j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    //diffuse
    vector<Texture> diffuseMaps = this->loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //specular
    vector<Texture> specularMaps = this->loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  }

  return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTexture(aiMaterial * mat, aiTextureType type, string typeName)
{
  vector<Texture> textures;
  for (GLuint i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);
    GLboolean skip = false;
    for (GLuint j = 0; j < textures_loaded.size(); ++j) {
      if (textures_loaded[j].path == str) {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }

    if (!skip) {
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), this->directory);
      texture.type = typeName;
      texture.path = str;

      textures.push_back(texture);
      this->textures_loaded.push_back(texture);
    }
  }

  return textures;
}

