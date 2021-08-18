#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector>

#include <stdio.h>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>



class Model
{
public:
  Model();
  bool init(const char *path);
  void draw();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint positionVBO;
  GLuint uvVBO;
  GLuint normalVBO;
  QVector<QVector3D> positions;
  QVector<QVector2D> uvs;
  QVector<QVector3D> normals;
};

#endif // MODEL_H
