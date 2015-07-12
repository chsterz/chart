TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11 -Wpedantic -Wall

SOURCES += main.cpp
INCLUDEPATH += /usr/include/cairomm-1.0/
INCLUDEPATH += /usr/lib/cairomm-1.0/include/
INCLUDEPATH += /usr/include/sigc++-2.0/
INCLUDEPATH += /usr/lib/sigc++-2.0/include/
INCLUDEPATH += /usr/include/cairo/
INCLUDEPATH += /usr/include/freetype2/

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    terminalcolors.h \
    fileparsing.h \
    drawing.h \
    types.h

LIBS += -L/lib/
LIBS += -L/usr/lib
LIBS += -lboost_program_options
LIBS += -lcairomm-1.0
LIBS += -lcairo
