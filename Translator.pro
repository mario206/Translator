#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T17:39:21
#
#-------------------------------------------------

QT       += core gui network multimedia
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Translator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    selectedtext.cpp \
    translator.cpp \
    form.cpp \
    words.cpp \
    MyGlobalShortcut/MyGlobalShortCut.cpp \
    MyGlobalShortcut/MyWinEventFilter.cpp

HEADERS  += mainwindow.h \
    selectedtext.h \
    translator.h \
    form.h \
    words.h \
    MyGlobalShortcut/MyGlobalShortCut.h \
    MyGlobalShortcut/MyWinEventFilter.h

FORMS    += mainwindow.ui \
    form.ui

RESOURCES += \
    resource.qrc
