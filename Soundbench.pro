######################################################################
# Automatically generated by qmake (2.01a) Tue Feb 21 08:35:29 2012
######################################################################

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += architect.h \
           backend.h \
           backend.portaudio.h \
           basicwaves.h \
           blueprint.h \
           common.h \
           emitter.h \
           errorpopup.h \
           gentypedialog.h \
           midi.h \
           modules.h \
           soundbenchmain.h \
           synth.h \
           warningpopup.h \
    geners.h \
    settings.basicgen.h \
    application.h \
    meterupdater.h

FORMS += errorpopup.ui \
         gentypedialog.ui \
         soundbenchmain.ui \
         warningpopup.ui \
    basicgenersettings.ui

SOURCES += architect.cpp \
           backend.portaudio.cpp \
           basicwaves.cpp \
           emitter.cpp \
           errorpopup.cpp \
           gentypedialog.cpp \
           main.cpp \
           midi.cpp \
           soundbenchmain.cpp \
           warningpopup.cpp \
    gen.basic.cpp \
    synth.cpp \
    settings.basicgen.cpp \
    application.cpp \
    meterupdater.cpp

RESOURCES += sbMainResources.qrc

unix:!symbian: LIBS += -L/usr/lib/ -lportaudio -lportaudiocpp -lpulse -lpulse-simple
