#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T17:48:56
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDVis
TEMPLATE = app

LIBS += -lopengl32 -lglu32 -lglut32

SOURCES += main.cpp\
        MainWindow.cpp \
    Atom.cpp \
    FileReader.cpp \
    Residue.cpp \
    xdrfile.c \
    xdrfile_xtc.c \
    MyOpenGLWidget.cpp \
    Vertex.cpp \
    ColourLegend.cpp \
    Transform3D.cpp \
    Camera3D.cpp

HEADERS  += MainWindow.h \
    Atom.h \
    FileReader.h \
    Residue.h \
    xdrfile.h \
    xdrfile_xtc.h \
    MyOpenGLWidget.h \
    Vertex.h \
    ColourLegend.h \
    Transform3D.h \
    Camera3D.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
