#ifndef WINDSYSTEM_H
#define WINDSYSTEM_H

#include <QWidget>
#include <QImage>
#include <QVector>

class WindSystem : public QWidget
{
public:
  explicit WindSystem(QWidget *parent = 0);
  void init();
  QVector<float> getProportion();//返回贝塞尔曲线的y值，以0.0-1.0的形式；用于风动的幅度计算上
protected:
  void paintEvent(QPaintEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
private:
  QImage image;
  QVector<QPointF> crl_points;//曲线的控制点
  QVector<QPointF> bezier_points;//存储贝塞尔曲线所有的点的容器，大小为0-200之间，即这个widget的大小

  void drawPoints(QImage &theImage, QVector<QPointF> points);//绘出控制点
  void drawBezier(QImage &theImage, QVector<QPointF> points);//绘出贝塞尔曲线
  void drawCoordinate(QImage &theImage);
};

#endif // WINDSYSTEM_H
