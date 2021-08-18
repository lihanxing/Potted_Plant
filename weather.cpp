#include "weather.h"
#include <QTime>

Weather::Weather(){
  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //随机时间的种子
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Weather::~Weather(){
  core->glDeleteBuffers(1, &VBO);
}

void Weather::init(int num_particle, QVector3D vec3_scale){
    this->vec3_scale = vec3_scale;


    for(int i = 0; i < num_particle; ++i){
        Particle par;
        float x = (-qrand()%400/100.0f)+2.0f;
        float z = (-qrand()%400/100.0f)+2.0f;
        float y = 5.0f;

        par.position = QVector3D(x, y, z);
        float v_y = qrand()%100/200.0f;
        par.velocity = QVector3D(0.0f, -v_y, 0.0f);
        par.Life = 1.0f;
        particles.push_back(par);
    }


    float vertices[] = {
        // positions          // normals           // texture coords
      1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

      1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f
    };

    core->glGenBuffers(1, &VBO);

    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Weather::draw(){
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    core->glEnableVertexAttribArray(0);
//    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    core->glDrawArrays(GL_TRIANGLES, 0, 6);
}

QVector<QMatrix4x4> Weather::getModelMats(){
  QVector<QMatrix4x4> models;

  for(int i = 0; i < particles.size(); ++i){
    QMatrix4x4 model;
    model.translate(particles[i].position);
    model.scale(vec3_scale);
    models.push_back(model);

    particles[i].position += particles[i].velocity;
//    particles[i].Life -=  0.02;
    if(particles[i].position.y() < -0.2f){
      float x = (-qrand()%400/100.0f)+2.0f;
      float z = (-qrand()%400/100.0f)+2.0f;
      float y = 5.0f;
      particles[i].position = QVector3D(x, y, z);
//      particles[i].Life = 1.0f;
    }
//    if(particles[i].Life < 0.0f){
//      float x = (-qrand()%400/100.0f)+2.0f;
//      float z = (-qrand()%400/100.0f)+2.0f;
//      float y = 5.0f;
//      particles[i].position = QVector3D(x, y, z);
//      particles[i].Life = 1.0f;
//    }
  }

  return models;
}
