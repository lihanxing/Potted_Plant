#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include<QPainter>
#include<QMouseEvent>
#include <desktop.h>
#include <QVector>
#include <QDebug>
#include <QtMath>

class Paint : public QWidget{
public:
  explicit Paint(QWidget *parent = 0);
  void setDeskTop(DeskTop *desk){
    this->desk = desk;
  }

  QVector<QPointF> getBezierPoints(QVector<QPointF> sel_points, int num_precision = 100);//由一组控制点得到 贝塞尔曲线上的所有点
  QVector<QVector<QPointF>> ctl_points;//每条贝塞尔曲线的控制点
  void cancelLastStep();
  void reset();
  QRgb backColor;
protected:
  void paintEvent(QPaintEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  void drawLines(QImage &theImage);
  void drawPoints(QImage &theImage, QVector<QPointF> points);//待删除
  void drawBezier(QImage &theImage, QVector<QPointF> points);
  QVector<QPointF> selectPoints();//选择贝塞尔曲线需要的控制点，并绘出这些控制点

  QImage image;
  QPoint lastPoint, endPoint;
  QVector<QPoint> points; //存储画线路径上的所有点。

  DeskTop *desk;
  /***********/
  QVector<QVector<QPoint>> image_hand_points;//在画板上画所有手绘的线条
  QVector<QVector<QPointF>> image_crl_points;//在画板上画所有组的控制点
  QVector<QVector<QPointF>> image_bez_points;//在画板上画所有条贝塞尔线
  /***********/
};

#endif // PAINT_H
