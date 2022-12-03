#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T10:50:43
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtWuziqi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GameModel.cpp \
    Ai.cpp \
    Evaluation.cpp \
    HashAi.cpp

HEADERS  += mainwindow.h \
    GameModel.h \
    Ai.h \
    Evaluation.h \
    HashAi.h

RESOURCES += \
    resource.qrc

CONFIG +=console
RC_ICONS=goBang.ico
VERSION=1.1.2.3
QMAKE_TARGET_PRODUCT=������
QMAKE_TARGET_DESCRIPTION=��Ӧ�ó�����AI�ǻ���ͳ��ѧ�ķ���
QMAKE_TARGET_COMPANY=ˮ��
QMAKE_TARGET_COPYRIGHT=Copyright  2021 - 2039 Plumbrilliant. All Right Reserved.

win32: LIBS += -L$$PWD/./ -lHashAi

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
