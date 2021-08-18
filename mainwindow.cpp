#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGlobal>
#include <QTime>
#include <QTimer>

int NUM_ADDTO_CUBES;//将场景中枝叶的数量增添至该值

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

  ui->setupUi(this);
  desk = new DeskTop(this);
  paint = new Paint(ui->tab);
  wind = new WindSystem(ui->tab_2);

  paint->setDeskTop(desk);


  this->setWindowTitle("Test");

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateDesktop);
  timer->start(10);

  QPalette pal(this->palette());

  //这里设置背景颜色
  pal.setColor(QPalette::Background, QColor(99, 103, 106));
  this->setAutoFillBackground(true);
  this->setPalette(pal);

  NUM_ADDTO_CUBES = ui->horizontalSlider->value();

  QPalette label_pa;
  label_pa.setColor(QPalette::WindowText,Qt::white);
  ui->label->setPalette(label_pa);
  ui->label_2->setPalette(label_pa);
  ui->label_3->setPalette(label_pa);
  ui->label_4->setPalette(label_pa);

  ui->doubleSpinBox->setValue(0.2);
  ui->doubleSpinBox_2->setValue(1.0);
  ui->doubleSpinBox_3->setValue(1.0);

  desk->getOGLManager()->windStrength = ui->horizontalSlider_2->value();
  //  desk->getOGLManager()->setLeafIndex(ui->comboBox->currentIndex());
}

MainWindow::~MainWindow(){
  delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
  desk->handleKeyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
  desk->handleKeyReleaseEvent(event);
}



//void MainWindow::on_pushButton_2_clicked(){
//  desk->getOGLManager()->isFill = GL_TRUE;
//}


void MainWindow::on_pushButton_3_clicked(){
  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

  int num_cubes = NUM_ADDTO_CUBES;// 参数可调！！！！！！！！！！！！！！
  int j = 0;
  for(int i = paint->ctl_points.size(); i <= num_cubes; ++i){
    if(j == paint->ctl_points.size())
      j = 0;
    QVector<QPointF> points = paint->ctl_points[j];
    for(int k = 0; k < points.size(); ++k){//这里对一段贝塞尔曲线上的控制点做随机调整
      points[k].setX(points[k].x() + qrand() % 30);
      points[k].setY(points[k].y() + qrand() % 30);
    }
    QVector<QPointF> bez_points = paint->getBezierPoints(points);
    desk->getOGLManager()->generateCube(bez_points);
    //desk->getOGLManager()->addCubes(paint->ctl_points,  36);
    j++;
  }
}

void MainWindow::on_pushButton_4_clicked(){

  paint->reset();
  desk->getOGLManager()->deleteCubes();
}

void MainWindow::on_pushButton_5_clicked()
{
  desk->getOGLManager()->movePlant();
}



void MainWindow::on_pushButton_2_clicked(){
  paint->cancelLastStep();
  desk->getOGLManager()->cancelLastStep();
}



void MainWindow::on_horizontalSlider_valueChanged(int value){
  NUM_ADDTO_CUBES = value;
  QString str;
  str.setNum(value);
  ui->label->setText(QStringLiteral("增补数量: ") + str);
}

void MainWindow::on_checkBox_clicked(bool checked){
  if(checked)
    desk->getOGLManager()->isCoordOpen = GL_TRUE;
  else
    desk->getOGLManager()->isCoordOpen = GL_FALSE;
}

void MainWindow::on_checkBox_2_clicked(bool checked){
  if(checked)
    desk->getOGLManager()->isFill = GL_FALSE;
  else
    desk->getOGLManager()->isFill = GL_TRUE;
}

void MainWindow::on_comboBox_currentIndexChanged(int index){
  desk->getOGLManager()->setLeafIndex(index);
  QVector3D scale = desk->getOGLManager()->vec3_leaf_scale;

  ui->doubleSpinBox->setValue(scale.x());
  ui->doubleSpinBox_2->setValue(scale.y());
  ui->doubleSpinBox_3->setValue(scale.z());
  desk->setFocus();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1){
  double y = ui->doubleSpinBox_2->value();
  double z = ui->doubleSpinBox_3->value();
  desk->getOGLManager()->vec3_leaf_scale = QVector3D(arg1, y, z);
  desk->setFocus();
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1){
  double x = ui->doubleSpinBox->value();
  double z = ui->doubleSpinBox_3->value();
  desk->getOGLManager()->vec3_leaf_scale = QVector3D(x, arg1, z);
  desk->setFocus();
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1){
  double x = ui->doubleSpinBox->value();
  double y = ui->doubleSpinBox_2->value();
  desk->getOGLManager()->vec3_leaf_scale = QVector3D(x, y, arg1);
  desk->setFocus();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value){
  desk->getOGLManager()->windStrength = value;
  desk->getOGLManager()->movePlant();

  QString str;
  str.setNum(value);
  ui->label_4->setText(QStringLiteral("风强: ") + str);
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
  if(checked){
    desk->getOGLManager()->windMovePro = wind->getProportion();
    desk->getOGLManager()->isWindOpen = GL_TRUE;
    desk->getOGLManager()->movePlant();
  }else{
    desk->getOGLManager()->isWindOpen = GL_FALSE;
    desk->getOGLManager()->movePlant();
  }
}

void MainWindow::on_checkBox_4_clicked(bool checked){
  if(checked){
      desk->getOGLManager()->isRainOpen = GL_TRUE;
      desk->getOGLManager()->movePlant();
  }else{
      desk->getOGLManager()->isRainOpen = GL_FALSE;
      desk->getOGLManager()->movePlant();
  }
}
