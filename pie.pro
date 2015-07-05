TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11 -Wpedantic -Wall

SOURCES += main.cpp
INCLUDEPATH += /usr/include/cairomm-1.0/
INCLUDEPATH += /usr/lib/cairomm-1.0/include/
INCLUDEPATH += /usr/include/sigc++-2.0/

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    terminalcolors.h \
    fileparsing.h \
    drawing.h

LIBS += -L/lib/
LIBS += -lboost_program_options
LIBS += -lboost_filesystem
LIBS += -lboost_system
