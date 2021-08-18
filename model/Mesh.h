//#ifndef MESH_H
//#define MESH_H

//#include <QVector>
//#include <assimp/scene.h>
//#include <QOpenGLFunctions_3_3_Core>
//#include "shader.h"
//#include "texture2d.h"
//#include <sstream>
//#include <string>


//using std::stringstream;
//using std::string;

//struct Vertex {
//  QVector3D Position;
//  QVector3D Normal;
//  QVector2D TexCoords;
//};

//struct Texture {
//  GLuint id;
//  string type;
//  aiString path;
//  Texture2D tex;
//};

//class Mesh {
//public:
//  Mesh(QVector<Vertex> vertices, QVector<GLuint> indices, QVector<Texture> textures);
//  void Draw(Shader shader);

//  QVector<Vertex> vertices;
//  QVector<GLuint> indices;
//  QVector<Texture> textures;

//private:
//  QOpenGLFunctions_3_3_Core *core;
//  GLuint VBO, VEO;
//  void setupMesh();
//};



//#endif
#ifndef MESH_H
#define MESH_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "shader.h"
#include <QOpenGLFunctions_3_3_Core>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stringstream;

//#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>


struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  GLuint id;
  string type;
  aiString path;
};

class Mesh {
public:
  Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
  void Draw(Shader shader);

  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VAO, VBO, VEO;
  void setupMesh();
};

#endif
