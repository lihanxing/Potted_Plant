#ifndef DESKTOP_H
#define DESKTOP_H

#include <QOpenGLWidget> //中文测试
#include "oglmanager.h"
#include <QKeyEvent>
#include <QTime>

class DeskTop : public QOpenGLWidget
{
public:
    DeskTop(QWidget *parent = 0);
    ~DeskTop();
    void handleKeyPressEvent(QKeyEvent *event);   //键盘按下事件
    void handleKeyReleaseEvent(QKeyEvent *event);  //键盘释放事件
    OGLManager* getOGLManager(){
      return manager;
    }
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标事件
    void wheelEvent(QWheelEvent *event) override;    //滚轮事件
    void mousePressEvent(QMouseEvent *event) override;  //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *event) override; //鼠标释放事件
private:
    OGLManager *manager;

    QTime time;
    GLfloat deltaTime;
    GLfloat lastFrame;

    GLboolean isFirstMouse;
    GLboolean isLeftMousePress;
    GLint lastX;
    GLint lastY;
};

#endif // DESKTOP_H
