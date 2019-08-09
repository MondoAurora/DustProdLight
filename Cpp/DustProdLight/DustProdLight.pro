#-------------------------------------------------
#
# Project created by QtCreator 2019-08-08T11:49:52
#
#-------------------------------------------------

QT       -= gui

TARGET = DustProdLight
TEMPLATE = lib

DEFINES += DUSTPRODLIGHT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        dplentity.cpp \
        dustprodlight.cpp

HEADERS += \
        dplentity.h \
        dustprodlight.h \
        dustprodlight_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Dust-Desktop_Qt_5_13_0_MSVC2015_64bit-Debug/release/ -lDust
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Dust-Desktop_Qt_5_13_0_MSVC2015_64bit-Debug/debug/ -lDust
else:unix: LIBS += -L$$PWD/../build-Dust-Desktop_Qt_5_13_0_MSVC2015_64bit-Debug/ -lDust

INCLUDEPATH += $$PWD/../Dust
DEPENDPATH += $$PWD/../Dust
