QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -Wall -Werror -pedantic -Wextra
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    actions.cpp \
    drawing.cpp \
    edge.cpp \
    edges.cpp \
    errors.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    point.cpp \
    points.cpp \
    qt_drawing.cpp

HEADERS += \
    actions.h \
    drawing.h \
    edge.h \
    edges.h \
    errors.h \
    mainwindow.h \
    model.h \
    model_transform.h \
    point.h \
    points.h \
    qt_drawing.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
