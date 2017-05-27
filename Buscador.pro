#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T13:43:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Buscador
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Buscador.cpp \
    indexadorHash.cpp \
    indexadorInformacion.cpp \
    ResultadoRI.cpp \
    stemmer.cpp \
    tokenizador.cpp

HEADERS  += mainwindow.h \
    Buscador.h \
    indexadorHash.h \
    indexadorInformacion.h \
    ResultadoRI.h \
    stemmer.h \
    tokenizador.h

FORMS    += mainwindow.ui
