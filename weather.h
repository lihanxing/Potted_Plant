#ifndef WEATHER_H
#define WEATHER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector>

struct Particle {
  QVector3D position;
  QVector3D velocity;

  GLfloat Life;
  Particle(){

  }
};

class Weather{
public:
  Weather();
  ~Weather();
  void init(int num_particle, QVector3D vec3_scale);
  void draw();
  QVector<QMatrix4x4> getModelMats();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;

  QVector<Particle> particles;
  QVector3D vec3_scale;

};

#endif // WEATHER_H
