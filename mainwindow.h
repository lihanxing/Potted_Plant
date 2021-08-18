#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "desktop.h"
#include "paint.h"
#include "windsystem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);   //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event);  //键盘释放事件
private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();
    void updateDesktop(){
      desk->update();
    }
    void on_pushButton_2_clicked();



    void on_horizontalSlider_valueChanged(int value);

    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_comboBox_currentIndexChanged(int index);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_checkBox_3_clicked(bool checked);

    void on_checkBox_4_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    DeskTop *desk;
    Paint *paint;
    WindSystem *wind;
};

#endif // MAINWINDOW_H
