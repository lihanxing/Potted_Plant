#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <Qmatrix4x4>

class Cube;

struct Orchid{ //兰花
  Orchid(){
    cube = NULL;
  }
  Orchid(Cube *cube, QMatrix4x4 initModel, QVector3D initScale){
    this->cube = cube;
    this->initModel = initModel;
    this->initScale = initScale;
  }

  ~Orchid(){
    if(cube)
      delete cube;
    models.clear();
  }

  Cube *cube;
  QVector<QMatrix4x4> models; //一组动画用的帧
  QMatrix4x4 initModel;
  QVector3D initScale;
};

class Cube
{
public:
  Cube();
  ~Cube();
//  void init(QVector<float> angles, GLboolean isLeft);
  bool init(QVector<QPointF> bez_points);
  void initPlane();
  void drawCube();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint posVBO, uvVBO, norVBO, tanVBO, bitVBO, elementVBO;

  QVector<GLushort> indices;
  QVector<QVector3D> indexed_positions;
  QVector<QVector2D> indexed_uvs;
  QVector<QVector3D> indexed_normals;
  QVector<QVector3D> indexed_tangents;
  QVector<QVector3D> indexed_bitangents;
};


class Light{
public:
  Light();
  ~Light();
  void init();
  void drawLight();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint lightVBO;
};

/************坐标类，一个简单的显示xyz坐标的类**************/
class Coordinate{
public:
  Coordinate();
  ~Coordinate();
  void init();
  void draw();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************地板，一个简单的显示xyz坐标的类**************/
class Floor{
public:
  Floor();
  ~Floor();
  void init();
  void draw();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

#endif // CUBE_H
