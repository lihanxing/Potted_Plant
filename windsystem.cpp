#include "windsystem.h"
#include <QtMath>
#include <QDebug>
#include "paint.h" //可以是贝塞尔曲线的计算函数，省的 再算
//#include "oglmanager.h" //使用animationFrame这个变量。动画帧数

extern QPointF getBezierPoint(QVector<QPointF> points, float t);

QVector<QPointF> getBezierPoints(QVector<QPointF> sel_points, int num_precision);

const int animationFrame = 40; //这里应该用extern的，把oglmanager里的拿过来
const int WINDSYSYEM_WIDTH = 340;
const int WINDSYSYEM_HEIGHT = 200;


WindSystem::WindSystem(QWidget *parent) : QWidget(parent){
// this->setGeometry(600, 50, WINDSYSYEM_WIDTH, WINDSYSYEM_HEIGHT);
    this->setGeometry(12, 10, WINDSYSYEM_WIDTH, WINDSYSYEM_HEIGHT);

 image = QImage(WINDSYSYEM_WIDTH, WINDSYSYEM_HEIGHT, QImage::Format_RGB32);
 image.fill(qRgb(180, 180, 180));
 this->init();
}

void WindSystem::init(){

  crl_points.push_back(QPointF(30.0f, 100.0f));
  crl_points.push_back(QPointF(90.0f, 100.0f));
  crl_points.push_back(QPointF(150.0f, 100.0f));
  crl_points.push_back(QPointF(210.0f, 100.0f));
  crl_points.push_back(QPointF(270.0f, 100.0f));
  crl_points.push_back(QPointF(330.0f, 100.0f));


  bezier_points = getBezierPoints(crl_points, animationFrame);

  this->drawCoordinate(image);
  this->drawPoints(image, crl_points);
  this->drawBezier(image, bezier_points);

}

QVector<float> WindSystem::getProportion(){
  QVector<float> pro;
  float pro_y;
  for(int i = 0; i < bezier_points.size(); ++i){
    pro_y = (WINDSYSYEM_HEIGHT-bezier_points[i].y())/WINDSYSYEM_HEIGHT;
    pro.push_back(pro_y);
  }

  return pro;
}

void WindSystem::paintEvent(QPaintEvent *event){
  QPainter painter(this);
  painter.drawImage(0, 0, image);
}

void WindSystem::mouseMoveEvent(QMouseEvent *event){
  if(event->buttons()&Qt::LeftButton){
    QPointF click_pos = event->localPos();
    QPointF delta_point;
    for(int i = 0; i < crl_points.size(); ++i){
      delta_point = crl_points[i]-click_pos;
      if(qAbs(delta_point.x()) < 10 && qAbs(delta_point.y()) < 10){ //如果选中控制点，可以重新绘制贝塞尔曲线
        image = QImage(WINDSYSYEM_WIDTH, WINDSYSYEM_HEIGHT, QImage::Format_RGB32);
        image.fill(qRgb(180, 180, 180));

        crl_points[i] = click_pos; //调整选中控制点的位置
        bezier_points = getBezierPoints(crl_points, animationFrame);//重新生成贝塞尔曲线
        this->drawCoordinate(image);
        this->drawPoints(image, crl_points);
        this->drawBezier(image, bezier_points);

//        this->drawPoints(image, bezier_points);//待删除
      }
    }
  }
}

void WindSystem::mousePressEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton){

  }
}

void WindSystem::mouseReleaseEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton){

  }
}

void WindSystem::drawPoints(QImage &theImage, QVector<QPointF> points){
  QPainter painter(&theImage);

  QPen pen;
  pen.setColor(Qt::red);
  pen.setWidth(7);
  painter.setPen(pen);

  painter.drawPoints(&points[0], points.size());

  update();
}

void WindSystem::drawBezier(QImage &theImage, QVector<QPointF> points){
  QPainter painter(&theImage);

  QPen pen;
  pen.setColor(Qt::blue);
  pen.setWidth(3);
  painter.setPen(pen);

  for(int i = 0; i < points.size()-1; ++i)
    painter.drawLine(points[i], points[i+1]);

  pen.setColor(QColor(0, 255, 0));
  pen.setWidth(2);
  painter.setPen(pen);
  painter.drawPoints(&points[0], points.size());

  update();
}

void WindSystem::drawCoordinate(QImage &theImage){
  QPainter painter(&theImage);

  QPen pen;
  pen.setColor(QColor(130, 130, 130));
  pen.setWidth(2);
  painter.setPen(pen);

//  const int WINDSYSYEM_WIDTH = 340;
//  const int WINDSYSYEM_HEIGHT = 200;
  int num_row = 6;
  int num_col = 9;
  int offset_row = WINDSYSYEM_HEIGHT/num_row;
  int offset_col = WINDSYSYEM_WIDTH/num_col;
  for(int i = 1; i < num_row; ++i)
    painter.drawLine(QPoint(10, offset_row*i), QPoint(330, offset_row*i));

  for(int i = 1; i < num_col; ++i)
    painter.drawLine(QPoint(offset_col*i, 10), QPoint(offset_col*i, 190));

//  painter.drawLine(QPoint(0, 10), QPoint(340, 10));

  update();
}

QVector<QPointF> getBezierPoints(QVector<QPointF> crl_points, int num_precision){
  QVector<QPointF> bez_points;
  float pre = 1.0f / num_precision;//控制精度的 点的数量  可调！！！！
  float t = 0.0f;
  for(int i = 0; i < num_precision; i++){
    bez_points.push_back(getBezierPoint(crl_points, t));
    t += pre;
  }

  return bez_points;
}

//QPointF getBezierPoint(QVector<QPointF> points, float t){ //计算某一比例t,计算 贝塞尔曲线在该比例下的位置
//  int num_crl = points.size();//控制点的数量
//  int n = num_crl - 1;

//  for(int r=0; r<n; r++)
//  {
//      for(int i=0; i<n-r; i++)
//      {
//          points[i] = (1.0 - t) * points[i] + t * points[i+1];
//      }
//  }

//  return points[0];
//}
