#include "paint.h"

int PAINT_WIDTH = 340;
int PAINT_HEIGHT = 300;

QPointF getBezierPoint(QVector<QPointF> points, float t);

Paint::Paint(QWidget *parent) : QWidget(parent){

  this->setGeometry(12, 10, PAINT_WIDTH, PAINT_HEIGHT); //设置画线窗口位置和大小

  image = QImage(PAINT_WIDTH, PAINT_HEIGHT, QImage::Format_RGB32);
  backColor = qRgb(210, 210, 210);
  image.fill(backColor);
}


QVector<QPointF> Paint::getBezierPoints(QVector<QPointF> sel_points, int num_precision){
  QVector<QPointF> bez_points;
//  int num_precision = num_curves; //控制精度的 点的数量  可调！！！！
  float pre = 1.0f / num_precision;
  float t = 0.0f;
  for(int i = 0; i < num_precision; i++){
    bez_points.push_back(getBezierPoint(sel_points, t));
    t += pre;
  }

  drawBezier(image, bez_points);//绘出贝塞尔曲线
  return bez_points;
}

void Paint::cancelLastStep(){
  if(image_hand_points.isEmpty()){
    qDebug() << "image_hand_points. is empty!!! cancel error!";
    return;
  }


  this->image_hand_points.pop_back();
  this->image_crl_points.pop_back();
  this->image_bez_points.pop_back();

  image = QImage(PAINT_WIDTH, PAINT_HEIGHT, QImage::Format_RGB32);
  image.fill(backColor);
  QPainter painter(&image);

  //手绘线条
  for(int i = 0; i < image_hand_points.size(); ++i){
    for(int j = 0; j < image_hand_points[i].size()-1; ++j)
      painter.drawLine(image_hand_points[i][j], image_hand_points[i][j+1]);
  }

  //控制点
  QPen pen;
  pen.setColor(Qt::white);
  pen.setWidth(5);
  painter.setPen(pen);

  for(int i = 0; i < image_crl_points.size(); ++i){
    painter.drawPoints(&image_crl_points[i][0], points.size());
  }

  //贝塞尔曲线
  pen.setColor(Qt::black);
  pen.setWidth(1);
  painter.setPen(pen);
  for(int i = 0; i < image_bez_points.size(); ++i){
    for(int j = 0; j < image_bez_points[i].size()-1; ++j)
      painter.drawLine(image_bez_points[i][j], image_bez_points[i][j+1]);
  }

  update();
}

void Paint::reset(){
  image = QImage(PAINT_WIDTH, PAINT_HEIGHT, QImage::Format_RGB32);
  image.fill(backColor);

  this->ctl_points.clear();
  this->image_hand_points.clear();
  this->image_crl_points.clear();
  this->image_bez_points.clear();

  update();
}

void Paint::paintEvent(QPaintEvent *event){
  QPainter painter(this);
  painter.drawImage(0, 0, image);
}

void Paint::mouseMoveEvent(QMouseEvent *event){
  if(event->buttons()&Qt::LeftButton){
    endPoint = event->pos();
    points.push_back(endPoint);
    drawLines(this->image);
  }
  //强制重绘
}

void Paint::mousePressEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton){
    points.clear(); //新画一条线就清空 上一条线所保存的所有点
//    bez_angles.angles.clear(); //意义同上
    lastPoint = event->pos();
    points.push_back(lastPoint);
  }

}

void Paint::mouseReleaseEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton){
    endPoint = event->pos();
    points.push_back(endPoint);
    drawLines(this->image);

    this->image_hand_points.push_back(points);
    /*********************/
    QVector<QPointF> sel_points = selectPoints();
    this->image_crl_points.push_back(sel_points);
    /*********************/
    QVector<QPointF> bez_points = getBezierPoints(sel_points, 30);
    this->image_bez_points.push_back(bez_points);
    desk->getOGLManager()->generateCube(bez_points);

  }
}

void Paint::drawLines(QImage &theImage){
  QPainter painter(&theImage);
  painter.drawLine(lastPoint, endPoint);
  lastPoint = endPoint;
  update();
}

QVector<QPoint> selPoints; //selectPoints,选择好的点，数个点。十几个吧
void Paint::drawPoints(QImage &theImage, QVector<QPointF> points){
  QPainter painter(&theImage);

  QPen pen;
  pen.setColor(Qt::white);
  pen.setWidth(5);
  painter.setPen(pen);

  painter.drawPoints(&points[0], points.size());

  update();
}

void Paint::drawBezier(QImage &theImage, QVector<QPointF> points)
{
  QPainter painter(&theImage);

  QPen pen;
  pen.setColor(Qt::black);
  painter.setPen(pen);

//  qDebug() << points.size() << endl;
//  for(int i = 0; i < points.size(); ++i)
//      qDebug() << points[i] << endl;

  for(int i = 0; i < points.size()-1; ++i)
    painter.drawLine(points[i], points[i+1]);

  update();
}

QVector<QPointF> Paint::selectPoints(){
  QVector<QPointF> sel_points; //储存贝塞尔曲线控制点的 vector

  int offset = points.size() / 6; //6是 6或7个控制点，可调！！！！
  for(int i = 0; i < points.size(); i+=offset)
    sel_points.push_back(points[i]);

  this->ctl_points.push_back(sel_points);

  drawPoints(image, sel_points); //绘出控制点
  return sel_points;
}



QPointF getBezierPoint(QVector<QPointF> points, float t){ //计算某一比例t,计算 贝塞尔曲线在该比例下的位置
  int num_crl = points.size();//控制点的数量
  int n = num_crl - 1;

  for(int r=0; r<n; r++)
  {
      for(int i=0; i<n-r; i++)
      {
          points[i] = (1.0 - t) * points[i] + t * points[i+1];
      }
  }

  return points[0];
}
