#include "oglmanager.h"
#include <QKeyEvent>
#include <QDebug>
#include "resourcemanager.h"
#include "cube.h"
#include <QtGlobal>
#include <QTime>
#include "model.h"
#include "weather.h"
//#include "model/Model.h"

const QVector3D CAMERA_POSITION(0.0f, 0.0f, 3.0f);

/***********这就是结果界面中的发光小方块***********/
const QVector3D LIGHT_POSITION(1.0f, 1.0f, -0.5f);
/***********这就是结果界面中的发光小方块***********/


GLfloat LIGHT_CONSTANT = 1.0f;
GLfloat LIGHT_LINEAR = 0.00025f;
GLfloat LIGHT_QUADRATIC = 0.0015f;
GLfloat MODEL_SHININESS = 64.0f;
GLfloat PLANT_SHININESS = 16.0f;

const int animationFrame = 40;

/***************  阴影用  **********************/
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
//const GLuint SCR_WIDTH = 600, SCR_HEIGHT = 600;
const GLfloat near_plane = 1.0f, far_plane = 7.5f;
bool isShadowOpen = true;
GLuint depthMapFBO = 0;
GLuint depthMap = 0;
/***************  阴影用  **********************/

/************待删除**********************/

/************待删除**********************/

GLboolean isMovePlant = false;//是否按下 计算 按钮，是叶子开始移动

//Cube *board;
Floor *board;
Light *light;
Coordinate *coordinate;
Model *penzi;
Model *solid;
Weather *rain;

OGLManager::OGLManager(GLuint w, GLuint h){
  this->width = w;
  this->height = h;
  for(GLuint i = 0; i != 1024; ++i)
    keys[i] = GL_FALSE;
  isFill = GL_TRUE;

  this->isWindOpen = GL_FALSE;
  this->isCoordOpen = GL_TRUE;
  this->isRainOpen = GL_FALSE;

}

OGLManager::~OGLManager(){
  delete this->camera;
  ResourceManager::clear();
}

void OGLManager::init(){
  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //随机时间的种子

  /*********  地板  **********/
  board = new Floor();
  board->init();
  /*********  地板  **********/

  rain = new Weather();
  rain->init(500, QVector3D(0.004f, 0.07f, 1.0f));

  light = new Light();
  light->init();

  coordinate = new Coordinate();
  coordinate->init();

  this->camera = new Camera(CAMERA_POSITION);

  penzi = new Model();
  solid = new Model();
//  penzi->init(":/models/res/models/huapen/penzi.obj");
//  penzi->init("C:/Users/136411501/Desktop/Practice/QtPractice/2018_1_14/Potted_Plant/res/models/huapen/penzi.obj");
  penzi->init("./res/models/huapen/pen_new.obj");
  solid->init("./res/models/huapen/solid_new.obj");


  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

  ResourceManager::loadTexture("wood", "./res/textures/wood.png");
  ResourceManager::loadTexture("wood_normal", "./res/textures/wood_normal.png");
//  ResourceManager::loadTexture("diaolan", ":/textures/res/textures/diaolan_leaf.png", true);
//  ResourceManager::loadTexture("diaolan_normal", ":/textures/res/textures/diaolan_leaf_normal.png");
  ResourceManager::loadTexture("white", "./res/models/huapen/white.jpg");
  ResourceManager::loadTexture("solid", "./res/models/huapen/Arch41_039_bark.jpg");
  ResourceManager::loadTexture("rain", "./res/textures/rain.png", true);



  ResourceManager::loadShader("model", "./res/shaders/model.vert", "./res/shaders/model.frag");
  ResourceManager::loadShader("cube", "./res/shaders/cube.vert", "./res/shaders/cube.frag");
  ResourceManager::loadShader("light", "./res/shaders/light.vert", "./res/shaders/light.frag");
  ResourceManager::loadShader("coordinate", "./res/shaders/coordinate.vert", "./res/shaders/coordinate.frag");
  ResourceManager::loadShader("normal", "./res/shaders/normal.vert", "./res/shaders/normal.frag", "./res/shaders/normal.geom");
  ResourceManager::loadShader("tangent", "./res/shaders/tangent.vert", "./res/shaders/tangent.frag", "./res/shaders/tangent.geom");
  ResourceManager::loadShader("bitangent", "./res/shaders/bitangent.vert", "./res/shaders/bitangent.frag", "./res/shaders/bitangent.geom");
  ResourceManager::loadShader("shadow_mapping_depth", "./res/shaders/shadow_mapping_depth.vert", "./res/shaders/shadow_mapping_depth.frag");
  ResourceManager::loadShader("floor", "./res/shaders/floor.vert", "./res/shaders/floor.frag");
  ResourceManager::loadShader("weather", "./res/shaders/weather.vert", "./res/shaders/weather.frag");


  ResourceManager::getShader("floor").use().setInteger("diffuseTexture", 0);
  ResourceManager::getShader("floor").use().setInteger("shadowMap", 1);

//  ResourceManager::getShader("point_shadows_depth").use().setFloat("far_plane", far_plane);
//  ResourceManager::getShader("point_shadows_depth").use().setVector3f("lightPos", LIGHT_POSITION);

  ResourceManager::getShader("weather").use().setInteger("diffuse", 0);

  ResourceManager::getShader("cube").use().setInteger("diffuseMap", 0);
  ResourceManager::getShader("cube").use().setInteger("normalMap", 1);
  ResourceManager::getShader("cube").use().setVector3f("lightPos", LIGHT_POSITION);
//  ResourceManager::getShader("cube").use().setFloat("light.ambient", 0.6f);
//  ResourceManager::getShader("cube").use().setFloat("light.diffuse", 0.6f);
//  ResourceManager::getShader("cube").use().setFloat("light.specular", 0.5f);

  ResourceManager::getShader("model").use().setInteger("material.diffuse", 0);
  ResourceManager::getShader("model").use().setInteger("material.specular", 1);
  ResourceManager::getShader("model").use().setFloat("material.shininess", MODEL_SHININESS);
  ResourceManager::getShader("model").use().setVector3f("light.position", LIGHT_POSITION);

  ResourceManager::getShader("model").use().setFloat("light.constant",LIGHT_CONSTANT);
  ResourceManager::getShader("model").use().setFloat("light.linear", LIGHT_LINEAR);
  ResourceManager::getShader("model").use().setFloat("light.quadratic", LIGHT_QUADRATIC);
  ResourceManager::getShader("model").use().setInteger("shadowMap", 2);

  QMatrix4x4 lightProjection, lightView;
  QMatrix4x4 lightSpaceMatrix;

  lightProjection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  lightView.lookAt(LIGHT_POSITION, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0, 1.0, 0.0));
  lightSpaceMatrix = lightProjection * lightView;
  // - render scene from light's point of view
  ResourceManager::getShader("shadow_mapping_depth").use().setMatrix4f("lightSpaceMatrix", lightSpaceMatrix);
  ResourceManager::getShader("model").use().setMatrix4f("lightSpaceMatrix", lightSpaceMatrix);

  ResourceManager::getShader("floor").use().setMatrix4f("lightSpaceMatrix", lightSpaceMatrix);
  ResourceManager::getShader("floor").use().setFloat("light.ambient", 0.4f);
  ResourceManager::getShader("floor").use().setFloat("light.diffuse", 0.8f);
  ResourceManager::getShader("floor").use().setFloat("light.specular", 0.85f);


  QMatrix4x4 model;
  model.translate(QVector3D(0.0f, -0.07f, 0.0f));
  model.scale(0.0001);
  ResourceManager::getShader("model").use().setMatrix4f("model", model);
  ResourceManager::getShader("normal").use().setMatrix4f("model", model);

  model.setToIdentity();
  model.translate(0.0f, -0.17f, 0.0f);
  model.scale(2.0f, 1.0f, 2.0f);
  ResourceManager::getShader("floor").use().setMatrix4f("model", model);

  model.setToIdentity();
  model.translate(LIGHT_POSITION);
  model.scale(0.1f);
  ResourceManager::getShader("light").use().setMatrix4f("model", model);

  model.setToIdentity();
  ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);
//  ResourceManager::getShader("weather").use().setMatrix4f("model", model);

  //开启状态
//  core->glClearColor(1.4f, 1.4f, 1.4f, 1.0f);
  core->glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
  core->glViewport(0, 0, this->width, this->height);
  core->glEnable(GL_DEPTH_TEST);

  /*********** 阴影用 ***************/

  core->glGenFramebuffers(1, &depthMapFBO);
  // - Create depth texture

  core->glGenTextures(1, &depthMap);
  core->glBindTexture(GL_TEXTURE_2D, depthMap);;


  core->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
  core->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  core->glDrawBuffer(GL_NONE);
  core->glReadBuffer(GL_NONE);
  if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
  core->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  /*********** 阴影用 ***************/

  this->setLeafIndex(0);
}

void OGLManager::processInput(GLfloat dt){
  if (keys[Qt::Key_W])
    camera->processKeyboard(FORWARD, dt);
  if (keys[Qt::Key_S])
    camera->processKeyboard(BACKWARD, dt);
  if (keys[Qt::Key_A])
    camera->processKeyboard(LEFT, dt);
  if (keys[Qt::Key_D])
    camera->processKeyboard(RIGHT, dt);
  if (keys[Qt::Key_E])
    camera->processKeyboard(UP, dt);
  if (keys[Qt::Key_Q])
    camera->processKeyboard(DOWN, dt);

}


void OGLManager::update(GLfloat dt){
  QMatrix4x4 projection, model;
  projection.perspective(camera->zoom, (GLfloat)width/(GLfloat)height, 0.1f, 200.f);

  ResourceManager::getShader("cube").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("cube").use().setVector3f("viewPos", camera->position);
  ResourceManager::getShader("cube").use().setMatrix4f("view", camera->getViewMatrix());

  ResourceManager::getShader("light").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("light").use().setMatrix4f("view", camera->getViewMatrix());

  ResourceManager::getShader("weather").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("weather").use().setMatrix4f("view", camera->getViewMatrix());

  ResourceManager::getShader("coordinate").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("coordinate").use().setMatrix4f("view", camera->getViewMatrix());

  ResourceManager::getShader("normal").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("normal").use().setMatrix4f("view", camera->getViewMatrix());
  ResourceManager::getShader("tangent").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("tangent").use().setMatrix4f("view", camera->getViewMatrix());
  ResourceManager::getShader("bitangent").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("bitangent").use().setMatrix4f("view", camera->getViewMatrix());
  ResourceManager::getShader("model").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("model").use().setMatrix4f("view", camera->getViewMatrix());
  ResourceManager::getShader("model").use().setVector3f("viewPos", camera->position);

  ResourceManager::getShader("floor").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("floor").use().setMatrix4f("view", camera->getViewMatrix());
  ResourceManager::getShader("floor").use().setVector3f("light.position", LIGHT_POSITION);
  ResourceManager::getShader("floor").use().setVector3f("viewPos", camera->position);


}

void OGLManager::resize(GLuint w, GLuint h){
  core->glViewport(0, 0, w, h);
}

int temp = 0;
bool tag = true;//控制动画的启动与结束，默认启动

void OGLManager::draw(GLfloat dt){
  core->glViewport(0, 0, this->width,  this->height);
  core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(!isFill)
    core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if(tag){ //控制动画的启动与结束，默认启动，最好别删除
    temp+=1;
    if(temp >= animationFrame-1)
      tag = false;
  }else{
    temp-=1;
    if(temp <= 0)
      tag = true;
  }

  QMatrix4x4 model;

  /******** 绘制地板 ***********/
  //先注释掉绘制地板的代码
  //ResourceManager::getShader("floor").use();
  //core->glActiveTexture(GL_TEXTURE0);
  //ResourceManager::getTexture("wood").bind();
  //core->glActiveTexture(GL_TEXTURE1);
  //core->glBindTexture(GL_TEXTURE_2D, depthMap);
  //board->draw();
  /******** 绘制地板 ***********/


  /******** 绘制天气 ***********/
//  ResourceManager::getShader("weather").use();
  if(isRainOpen){
    core->glActiveTexture(GL_TEXTURE0);
    ResourceManager::getTexture("rain").bind();
    QVector<QMatrix4x4> weather_models = rain->getModelMats();
    for(int i = 0; i < weather_models.size(); ++i){
      ResourceManager::getShader("weather").use().setMatrix4f("model", weather_models[i]);
      rain->draw();
    }
  }
  /******** 绘制天气 ***********/


  core->glActiveTexture(GL_TEXTURE0);
  ResourceManager::getTexture("diaolan").bind();
  core->glActiveTexture(GL_TEXTURE1);
  ResourceManager::getTexture("diaolan_normal").bind();

  for(int i = 0; i < left_orchides.size(); ++i){
//      qDebug() << left_orchides[0]->models.size();
    if(isMovePlant)
      model = left_orchides[i]->models[0+temp];
    else
      model = left_orchides[i]->initModel;
    ResourceManager::getShader("cube").use().setMatrix4f("model", model);
    left_orchides[i]->cube->drawCube();
  }

  for(int i = 0; i < right_orchides.size(); ++i){
    if(isMovePlant)
      model = right_orchides[i]->models[0+temp];
    else
      model = right_orchides[i]->initModel;
    ResourceManager::getShader("cube").use().setMatrix4f("model", model);
    right_orchides[i]->cube->drawCube();

  }


  ResourceManager::getShader("light").use();
  light->drawLight();

  if(isCoordOpen){
    ResourceManager::getShader("coordinate").use();
    coordinate->draw();
  }
  /*********  盆子  *********/
  ResourceManager::getShader("model").use().setFloat("light.ambient", 0.4f);
  ResourceManager::getShader("model").use().setFloat("light.diffuse", 0.5f);
  ResourceManager::getShader("model").use().setFloat("light.specular", 0.54f);
  core->glActiveTexture(GL_TEXTURE0);
  ResourceManager::getTexture("white").bind();
  core->glActiveTexture(GL_TEXTURE2);
  core->glBindTexture(GL_TEXTURE_2D, depthMap);
  penzi->draw();

  core->glActiveTexture(GL_TEXTURE0);
  ResourceManager::getTexture("solid").bind();
  core->glActiveTexture(GL_TEXTURE2);
  core->glBindTexture(GL_TEXTURE_2D, depthMap);
  ResourceManager::getShader("model").use().setFloat("light.ambient", 0.4f);
  ResourceManager::getShader("model").use().setFloat("light.diffuse", 0.6f);
  ResourceManager::getShader("model").use().setFloat("light.specular", 0.2f);
  solid->draw();
//  ResourceManager::getShader("normal").use();
//  penzi->draw();
  /*********  盆子  *********/


  if(!isFill)
      core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


  /********* 制作深度贴图  **************/
  core->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    core->glClear(GL_DEPTH_BUFFER_BIT);
    core->glCullFace(GL_FRONT); //解决 阴影的悬浮 问题， peter pan问题
    model.setToIdentity();
    model.translate(QVector3D(0.0f, -0.07f, 0.0f));
    model.scale(0.0001);
    ResourceManager::getShader("shadow_mapping_depth").use().setMatrix4f("model", model);
    penzi->draw();
//    solid->draw();

    model.setToIdentity();
    model.translate(0.0f, -0.17f, 0.0f);
    model.scale(2.0f, 1.0f, 2.0f);
    ResourceManager::getShader("shadow_mapping_depth").use().setMatrix4f("model", model);
    board->draw();

//    model.setToIdentity();
//    model.scale(QVector3D(0.05f, 1.0f, 1.0f));
    for(int i = 0; i < left_orchides.size(); ++i){
  //      qDebug() << left_orchides[0]->models.size();
      if(isMovePlant){
        model = left_orchides[i]->models[0+temp];
        model.scale(0.25f, 1.0f, 1.0f);
      }else{
        model = left_orchides[i]->initModel;
        model.scale(0.25f, 1.0f, 1.0f);
      }

      ResourceManager::getShader("shadow_mapping_depth").use().setMatrix4f("model", model);
      left_orchides[i]->cube->drawCube();
    }

    for(int i = 0; i < right_orchides.size(); ++i){
      if(isMovePlant){
        model = right_orchides[i]->models[0+temp];
        model.scale(0.25f, 1.0f, 1.0f);
      }else{
        model = right_orchides[i]->initModel;
        model.scale(0.25f, 1.0f, 1.0f);
      }
      ResourceManager::getShader("shadow_mapping_depth").use().setMatrix4f("model", model);
      right_orchides[i]->cube->drawCube();

    }


    core->glCullFace(GL_BACK);
  core->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  /********* 制作深度贴图  **************/

}

void OGLManager::generateCube(QVector<QPointF> bez_points){
  QMatrix4x4 model;
  model.scale(this->vec3_leaf_scale);

  Cube *cube = new Cube();
  bool left = cube->init(bez_points);
  Orchid *orchid = new Orchid(cube, model, this->vec3_leaf_scale);
  generateType.push_back(left);
  if(left)
    left_orchides.push_back(orchid);
  else
    right_orchides.push_back(orchid);

}

void OGLManager::deleteCubes(){
  for(int i = 0; i < left_orchides.size(); ++i)
    delete left_orchides[i];
  for(int i = 0; i < right_orchides.size(); ++i)
    delete right_orchides[i];

  core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    core->glClear(GL_DEPTH_BUFFER_BIT);
  core->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  left_orchides.clear();
  right_orchides.clear();
  generateType.clear();
  isMovePlant = false;
  isWindOpen = false;
  isRainOpen = false;
}

void OGLManager::movePlant(){
//    qDebug() << "size " << windMovePro.size();
//    for(int i = 0;i < windMovePro.size(); ++i){
//        qDebug() << windMovePro[i];
//    }

  for(int i = 0; i < left_orchides.size(); ++i)
    left_orchides[i]->models.clear();
  for(int i = 0; i < right_orchides.size(); ++i)
    right_orchides[i]->models.clear();


  float angle = 180.0f / left_orchides.size();
  for(int i = 0; i < left_orchides.size(); ++i){
    QMatrix4x4 model;
    float a = angle * i + (qrand()%5);
    float b = (qrand()%20)/400.0f;
    float c = qrand()%20;
//    qDebug() <<c;
    for(int j = 0; j < animationFrame; ++j){
      model.setToIdentity();
      model.rotate(a, QVector3D(0.0f, 1.0f, 0.0f));
      model.translate(0.0f, 0.0f, -b);
      if(isWindOpen){
        int windFrequency = j*(windStrength/15.0f);
        while(windFrequency >= animationFrame)
          windFrequency-=animationFrame;

//        qDebug()<<windFrequency;
        float ttt = windStrength * windMovePro[windFrequency];
        model.rotate(ttt, QVector3D(1.0f, 0.0f, 0.0f));//风动效果的上下摆动 所需旋转矩阵
//        ttt = c * windMovePro[j]*(left_orchides.size()-i)/left_orchides.size();
        ttt = (c+windStrength*2) * windMovePro[windFrequency];
        model.rotate(ttt, QVector3D(0.0f, 1.0f, 0.0f));//由风向控制的左右摆动
      }
      if(isRainOpen){
        float ttt =  10.0f * (sin(j/10.0f)+1.0f)/2.0f;
// qDebug() << ((qrand()%3+3)/10.0f)
        model.rotate(ttt, QVector3D(1.0f, 0.0f, 0.0f));//风动效果的上下摆动 所需旋转矩阵
      }
      model.scale(left_orchides[i]->initScale);
      left_orchides[i]->models.push_back(model);
    }
  }

  angle = 180.0f / right_orchides.size();
//  qDebug() << "angle: " << angle << endl;
  for(int i = 0; i < right_orchides.size(); ++i){
    QMatrix4x4 model;
    float a = angle * i + (qrand()%5);
    float b = (qrand()%20)/400.0f;
    float c = qrand()%20;
    for(int j = 0; j < animationFrame; ++j){
      model.setToIdentity();
      model.rotate(a, QVector3D(0.0f, 1.0f, 0.0f));
      model.translate(0.0f, 0.0f, b);
      if(isWindOpen){
        int windFrequency = j*(windStrength/15.0f);
        while(windFrequency >= animationFrame)
          windFrequency-=animationFrame;

        float ttt = windStrength * windMovePro[windFrequency];
        model.rotate(ttt, QVector3D(1.0f, 0.0f, 0.0f));//风动效果的上下摆动 所需旋转矩阵
//        ttt = c * windMovePro[j]*(right_orchides.size()-i)/right_orchides.size();
        ttt = (c+windStrength*2) * windMovePro[windFrequency];
        model.rotate(ttt, QVector3D(0.0f, 1.0f, 0.0f));//由风向控制的左右摆动
      }
      if(isRainOpen){
        float ttt = 10.0f * (sin(j/10.0f)+1.0f)/2.0f;

        model.rotate(ttt, QVector3D(-1.0f, 0.0f, 0.0f));//风动效果的上下摆动 所需旋转矩阵
      }
      model.scale(right_orchides[i]->initScale);
      right_orchides[i]->models.push_back(model);
    }
  }


  isMovePlant = true;
}

void OGLManager::cancelLastStep(){
  if(generateType.isEmpty()){
    qDebug() << "没有枝叶可供撤销";
    return;
  }

  bool result = generateType.last();
  generateType.pop_back();
  if(result){
    Orchid *temp = left_orchides.last();
    delete temp;
    left_orchides.pop_back();
  }else{
    Orchid *temp = right_orchides.last();
    delete temp;
    right_orchides.pop_back();
  }
}

void OGLManager::setLeafIndex(int index){
  if(index == 0){//吊兰
    ResourceManager::loadTexture("diaolan", ":/textures/res/textures/diaolan_leaf.png", true);
    ResourceManager::loadTexture("diaolan_normal", ":/textures/res/textures/diaolan_leaf_normal.png");
    ResourceManager::getShader("cube").use().setFloat("light.ambient", 0.6f);
    ResourceManager::getShader("cube").use().setFloat("light.diffuse", 0.6f);
    ResourceManager::getShader("cube").use().setFloat("light.specular", 0.5f);
    vec3_leaf_scale = QVector3D(0.2f, 1.0f, 1.0f);
  }else if(index == 1){//虎皮兰
    ResourceManager::loadTexture("diaolan", ":/textures/res/textures/hupilan.png", true);
    ResourceManager::loadTexture("diaolan_normal", ":/textures/res/textures/hupilan_normal.png");
    ResourceManager::getShader("cube").use().setFloat("light.ambient", 0.6f);
    ResourceManager::getShader("cube").use().setFloat("light.diffuse", 0.6f);
    ResourceManager::getShader("cube").use().setFloat("light.specular", 0.5f);
    vec3_leaf_scale = QVector3D(0.1f, 1.0f, 1.0f);
  }else if(index = 2){//君子兰
    ResourceManager::loadTexture("diaolan", ":/textures/res/textures/junzilan.png", true);
    ResourceManager::loadTexture("diaolan_normal", ":/textures/res/textures/junzilan_normal_.png");
    ResourceManager::getShader("cube").use().setFloat("light.ambient", 0.4f);
    ResourceManager::getShader("cube").use().setFloat("light.diffuse", 0.6f);
    ResourceManager::getShader("cube").use().setFloat("light.specular", 0.7f);
    vec3_leaf_scale = QVector3D(0.15f, 1.0f, 1.0f);
  }
}
