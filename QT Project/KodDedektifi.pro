#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T15:10:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KodDedektifi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        addtypedefs.cpp \
        cleancode.cpp \
        detailwindow.cpp \
        hashing.cpp \
        main.cpp \
        mainwindow.cpp \
        processdocuments.cpp \
        resultwindow.cpp \
        transformcode.cpp \
        transformdoc.cpp \
        winnowing.cpp

HEADERS += \
        addtypedefs.h \
        cleancode.h \
        detailwindow.h \
        fundamentals.h \
        hashing.h \
        mainwindow.h \
        processdocuments.h \
        resultwindow.h \
        transformcode.h \
        transformdoc.h \
        winnowing.h

FORMS += \
        codewindow.ui \
        detailwindow.ui \
        mainwindow.ui \
        resultwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    files.qrc

#ICON = images/logo.ico
#ICON = images/logo.icns # for MacOS

INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/local/lib -lpoppler-qt5
