#-------------------------------------------------
#
# Project created by QtCreator 2017-12-18T15:17:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BRLCADeditor
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
        editor.cpp \
    highlightercpp.cpp \
    highlighterascii.cpp \
    dialogexportstl.cpp \
    codeeditorwidget.cpp \
    dialogexportdxf.cpp \
    dialogimportdxf.cpp \
    dialogimportstl.cpp

HEADERS  += editor.h \
    highlighterascii.h \
    highlightercpp.h \
    dialogexportstl.h \
    codeeditorwidget.h \
    dialogexportdxf.h \
    dialogimportdxf.h \
    dialogimportstl.h

FORMS    += editor.ui \
    dialogexportstl.ui \
    dialogexportdxf.ui \
    dialogimportdxf.ui \
    dialogimportstl.ui
