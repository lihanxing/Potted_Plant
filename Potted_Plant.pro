#-------------------------------------------------
#
# Project created by QtCreator 2018-05-07T15:49:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Potted_Plant
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    cube.cpp \
    desktop.cpp \
    oglmanager.cpp \
    resourcemanager.cpp \
    shader.cpp \
    texture2d.cpp \
    paint.cpp \
    model.cpp \
    windsystem.cpp \
    weather.cpp

HEADERS  += mainwindow.h \
    camera.h \
    cube.h \
    desktop.h \
    oglmanager.h \
    resourcemanager.h \
    shader.h \
    texture2d.h \
    paint.h \
    model.h \
    windsystem.h \
    weather.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lassimp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lassimpd
#else:unix: LIBS += -L$$PWD/lib/ -lassimp

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lSOIL
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lSOIL
#else:unix: LIBS += -L$$PWD/lib/ -lSOIL

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.
#C:/Users/136411501/Desktop/Practice/QtPractice/2018_1_14/Potted_Plant
#LIBS += -LC:/Users/136411501/Desktop/Practice/QtPractice/2018_1_14/Potted_Plant/lib/ -lassimpd
#LIBS += -LC:/Users/136411501/Desktop/Practice/QtPractice/2018_1_14/Potted_Plant/lib/ -lSOIL



#win32: LIBS += $$PWD/lib/ SOIL
#win32: LIBS += $$PWD/lib/ assimpd

#INCLUDEPATH += $$PWD/lib
#DEPENDPATH += $$PWD/lib

#win32: LIBS += -lassimpd
#win32: LIBS += -lSOIL

#LIBS += -L'H:/IHNF/IHNF Tools/Visual Studio/VC/lib/' -lassimpd
##LIBS += -L'H:/IHNF/IHNF Tools/Visual Studio/VC/lib/' -lSOIL

#INCLUDEPATH += 'H:/IHNF/IHNF Tools/Visual Studio/VC/include'
##DEPENDPATH += 'H:/IHNF/IHNF Tools/Visual Studio/VC/include'
