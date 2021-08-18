 #include "desktop.h"
#include <QPalette>


//这里设置结果界面大小
int DESKTOP_WIDTH = 600;
int DESKTOP_HEIGHT = 600;


DeskTop::DeskTop(QWidget *parent) : QOpenGLWidget(parent)
{
    this->setGeometry(10, 20, DESKTOP_WIDTH, DESKTOP_HEIGHT);
    QPalette pal(this->palette());

    //设置背景黑色
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    manager = new OGLManager(this->width(), this->height());
}

DeskTop::~DeskTop()
{
    delete manager;
}

void DeskTop::initializeGL()
{
    manager->init();
    deltaTime = 0.0f;
    lastFrame = 0.0f;

    isFirstMouse = GL_TRUE;
    isLeftMousePress = GL_FALSE;
    lastX = width() / 2.0f;
    lastY = height() / 2.0f;

    time.start();
}

void DeskTop::resizeGL(int w, int h)
{
    manager->resize(w, h);
}

void DeskTop::paintGL()
{
  GLfloat currentFrame = (GLfloat)time.elapsed()/100;
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  manager->processInput(deltaTime);
  manager->update(deltaTime);
  manager->draw(deltaTime);

//  qDebug() << currentFrame;/**/
//  update();
}

void DeskTop::handleKeyPressEvent(QKeyEvent *event)
{
    GLuint key = event->key();
    if(key >= 0 && key <= 1024){
        manager->keys[key] = GL_TRUE;
//      update();
    }
}

void DeskTop::handleKeyReleaseEvent(QKeyEvent *event)
{
    GLuint key = event->key();
    if(key >= 0 && key <= 1024){
      manager->keys[key] = GL_FALSE;
//      update();
    }
}

void DeskTop::mouseMoveEvent(QMouseEvent *event)
{
    GLint xpos = event->pos().x();
    GLint ypos = event->pos().y();

    if(isLeftMousePress){
        if (isFirstMouse){
            lastX = xpos;
            lastY = ypos;
            isFirstMouse = GL_FALSE;
        }

        GLint xoffset = xpos - lastX;
        GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;
        manager->camera->processMouseMovement(xoffset, yoffset);
//        update();
    }
}

void DeskTop::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    manager->camera->processMouseScroll(offset.y()/20.0f);
    update();
}

void DeskTop::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){ //注意是button（）不是buttons（）；
        isLeftMousePress = GL_TRUE;
//        update();
    }
}

void DeskTop::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){ //注意是button（）不是buttons（）；
        isLeftMousePress = GL_FALSE;
        isFirstMouse = GL_TRUE;
//        update();
    }
}
