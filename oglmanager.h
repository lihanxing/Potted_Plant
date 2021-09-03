#ifndef OGLMANAGER_H
#define OGLMANAGER_H

#include <QOpenGLFunctions_3_3_Core>
#include "camera.h"
#include "shader.h"
#include <QOpenGLTexture>
#include "texture2d.h"
#include "cube.h"
#include <GL/freeglut.h>

class OGLManager
{
public:
  OGLManager(GLuint w, GLuint h);
  ~OGLManager();

  void init();
  void processInput(GLfloat dt);
  void update(GLfloat dt);
  void resize(GLuint w, GLuint h);
  void draw(GLfloat dt);
//  void generateCube(QVector<float> angles, bool isLeft);//使用指定角度，生成一片叶子
  void generateCube(QVector<QPointF> bez_points);//使用指定角度，生成一片叶子

  void deleteCubes();
  void movePlant();//生成移动叶子所需的 4*4矩阵，
  void cancelLastStep();
  void setLeafIndex(int index);//根据所以编号 选择特定纹理，吊兰，君子兰 虎皮兰之类
  GLboolean keys[1024];
  GLboolean isFill;//是实体还是线框渲染模式
  Camera *camera;

  QVector<float> windMovePro;//风动产生的移动比例
  GLboolean isWindOpen;
  GLboolean isCoordOpen;
  GLboolean isRainOpen;
  QVector3D vec3_leaf_scale;
  QVector4D vec4_branch_scale;
  GLuint windStrength;//风的强度
private:
  QVector<Orchid*> left_orchides;
  QVector<Orchid*> right_orchides;
  QOpenGLFunctions_3_3_Core *core;
  GLuint width;
  GLuint height;

  QVector<bool> generateType;//撤销上一步所用，记录是撤销左还是右枝
  /****待删除**/
  /****待删除**/
};

#endif // OGLMANAGER_H
