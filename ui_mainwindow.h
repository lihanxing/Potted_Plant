/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButton_3;
    QGroupBox *groupBox;
    QPushButton *pushButton_4;
    QPushButton *pushButton_2;
    QPushButton *pushButton_5;
    QSlider *horizontalSlider;
    QLabel *label;
    QGroupBox *groupBox_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QComboBox *comboBox;
    QLabel *label_2;
    QGroupBox *groupBox_3;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_2;
    QDoubleSpinBox *doubleSpinBox_3;
    QDoubleSpinBox *doubleSpinBox;
    QWidget *tab_2;
    QSlider *horizontalSlider_2;
    QLabel *label_4;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 650);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(620, 10, 371, 601));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 420, 80, 20));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(240, 320, 120, 121));
        QFont font;
        font.setFamily(QStringLiteral("AcadEref"));
        font.setPointSize(9);
        groupBox->setFont(font);
        groupBox->setFlat(true);
        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(20, 60, 80, 20));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 30, 80, 20));
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(20, 90, 80, 20));
        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 390, 211, 20));
        horizontalSlider->setMinimum(12);
        horizontalSlider->setMaximum(108);
        horizontalSlider->setSingleStep(2);
        horizontalSlider->setValue(24);
        horizontalSlider->setOrientation(Qt::Horizontal);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 420, 81, 16));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(240, 470, 120, 80));
        QFont font1;
        font1.setFamily(QStringLiteral("Agency FB"));
        groupBox_2->setFont(font1);
        groupBox_2->setFlat(true);
        checkBox = new QCheckBox(groupBox_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(20, 20, 91, 18));
        checkBox->setChecked(true);
        checkBox_2 = new QCheckBox(groupBox_2);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(20, 50, 73, 18));
        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(90, 320, 131, 22));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(13, 320, 61, 21));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 470, 200, 80));
        groupBox_3->setFont(font1);
        groupBox_3->setFlat(true);
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 55, 161, 16));
        doubleSpinBox_2 = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setGeometry(QRect(80, 30, 45, 20));
        doubleSpinBox_2->setMaximum(2);
        doubleSpinBox_2->setSingleStep(0.05);
        doubleSpinBox_3 = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setGeometry(QRect(140, 30, 45, 20));
        doubleSpinBox_3->setMaximum(2);
        doubleSpinBox_3->setSingleStep(0.05);
        doubleSpinBox = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(20, 30, 45, 20));
        doubleSpinBox->setMaximum(2);
        doubleSpinBox->setSingleStep(0.05);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalSlider_2 = new QSlider(tab_2);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(40, 240, 201, 16));
        horizontalSlider_2->setMinimum(5);
        horizontalSlider_2->setMaximum(80);
        horizontalSlider_2->setValue(5);
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(280, 240, 54, 12));
        checkBox_3 = new QCheckBox(tab_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(260, 280, 73, 18));
        checkBox_4 = new QCheckBox(tab_2);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(260, 320, 73, 18));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "\350\256\241\347\256\227", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\216\247\345\210\266\345\217\260", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\222\244\351\224\200\344\270\212\344\270\200\346\255\245", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "\347\247\273\345\212\250", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\345\242\236\350\241\245\346\225\260\351\207\217: 24", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\234\272\346\231\257\346\216\247\345\210\266", Q_NULLPTR));
        checkBox->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257\345\235\220\346\240\207\347\263\273", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("MainWindow", "\347\272\277\346\200\247\346\270\262\346\237\223", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\220\212\345\205\260", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\350\231\216\347\232\256\345\205\260", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\220\233\345\255\220\345\205\260", Q_NULLPTR)
        );
        label_2->setText(QApplication::translate("MainWindow", "\346\244\215\347\211\251\347\261\273\345\236\213\357\274\232", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\345\217\266\345\255\220\345\244\247\345\260\217", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", " x\350\275\264       y\350\275\264       z\350\275\264", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\345\237\272\347\241\200\345\267\245\345\205\267", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\351\243\216\345\274\272\357\274\2325", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257\351\243\216\345\212\250", Q_NULLPTR));
        checkBox_4->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257\351\233\250\345\244\251", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\345\244\251\346\260\224\347\263\273\347\273\237", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
