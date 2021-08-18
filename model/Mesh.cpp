//#include "Mesh.h"

//Mesh::Mesh(QVector<Vertex> vertices, QVector<GLuint> indices, QVector<Texture> textures) {
//  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

//  this->vertices = vertices;
//  this->indices = indices;
//  this->textures = textures;

//  this->setupMesh();
//}

//void Mesh::setupMesh() {
////  core->glGenVertexArrays(1, &this->VAO);
//  core->glGenBuffers(1, &this->VBO);
//  core->glGenBuffers(1, &this->VEO);

////  glBindVertexArray(this->VAO);
//  core->glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//  core->glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

//  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VEO);
//  core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
//}

//void Mesh::Draw(Shader shader) {
//  GLuint diffuseNo = 1;
//  GLuint specularNo = 1;

//  for (GLuint i = 0; i < this->textures.size(); ++i) {
//    core->glActiveTexture(GL_TEXTURE0 + i);
//    core->glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
//    //this->textures[i].tex.bind();

//    stringstream ss;
//    string number;
//    string name = this->textures[i].type;

//    if (name == "texture_diffuse")
//      ss << diffuseNo++;
//    else if (name == "texture_specular")
//      ss << specularNo++;

//    number = ss.str();
//    shader.setInteger(("material." + name + number).c_str(), i);
//    //core->glUniform1i(glGetUniformLocation(shader.ProgramID, ("material." + name + number).c_str()), i);
//  }


////  想buffer传送数据
//  core->glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//  core->glEnableVertexAttribArray(0);

//  core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
//  core->glEnableVertexAttribArray(1);

//  core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
//  core->glEnableVertexAttribArray(2);

//  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VEO);
//  core->glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

//  //��ԭ
//  for (GLuint i = 0; i < this->textures.size(); ++i) {
//    core->glActiveTexture(GL_TEXTURE0 + i);
//    core->glBindTexture(GL_TEXTURE_2D, 0);
//  }
//}
#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  this->setupMesh();
}

void Mesh::setupMesh() {
     core->glGenBuffers(1, &this->VBO);
     core->glGenBuffers(1, &this->VEO);

    //  glBindVertexArray(this->VAO);
      core->glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      core->glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

     core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VEO);
      core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

}

void Mesh::Draw(Shader shader) {
  GLuint diffuseNo = 1;
  GLuint specularNo = 1;
  for (GLuint i = 0; i < this->textures.size(); ++i) {
     core->glActiveTexture(GL_TEXTURE0 + i);
     core->glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    stringstream ss;
    string number;
    string name = this->textures[i].type;

    if (name == "texture_diffuse")
      ss << diffuseNo++;
    else if (name == "texture_specular")
      ss << specularNo++;

    number = ss.str();
    shader.setInteger(("material." + name + number).c_str(), i);
    //glUniform1i(glGetUniformLocation(shader.programID, ("material." + name + number).c_str()), i);
  }

    core->glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    core->glEnableVertexAttribArray(0);

    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    core->glEnableVertexAttribArray(1);

    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    core->glEnableVertexAttribArray(2);

    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VEO);
    core->glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);


  //还原
  for (GLuint i = 0; i < this->textures.size(); ++i) {
     core->glActiveTexture(GL_TEXTURE0 + i);
     core->glBindTexture(GL_TEXTURE_2D, 0);
  }
}
