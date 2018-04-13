#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T19:18:55
#
#-------------------------------------------------

QT       += core gui sql axcontainer
CONFIG += axcontainer


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Sarl Les Arenes"
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    addproduct.cpp \
    chooseproduct.cpp \
    outputmanager.cpp \
    chooseclient.cpp \
    ajouterclient.cpp \
    showf.cpp \
    editablesqlmodel.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    addproduct.h \
    chooseproduct.h \
    outputmanager.h \
    chooseclient.h \
    ajouterclient.h \
    showf.h \
    editablesqlmodel.h

FORMS    += mainwindow.ui \
    addproduct.ui \
    chooseproduct.ui \
    chooseclient.ui \
    ajouterclient.ui \
    showf.ui

win32 {

    RC_FILE = ressource.rc

}

DISTFILES += \
    ressource.rc
