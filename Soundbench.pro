#######################################################################
# Automatically generated by the Soundbench team at some point in time.
#######################################################################

QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -pedantic
QMAKE_CXXFLAGS += -std=c++0x

DEFINES += IS_SOUNDBENCH NO_JACK  #JACK support doesn't exist yet.

TEMPLATE = app
TARGET = 
DEPENDPATH += .

unix: INCLUDEPATH += /usr/include/

# Input
HEADERS += architect.h \
           backend.portaudio.h \
           basicwaves.h \
           blueprint.h \
           common.h \
           emitter.h \
           errorpopup.h \
           gentypedialog.h \
           soundbenchmain.h \
           synth.h \
           warningpopup.h \
    application.h \
    meterupdater.h \
    player.h \
    midichain.h \
    problemfix.h \
    backend.h \
    sbutilities.h \
    midienums.h \
    settings_basicgen.h \
    soundbases.h \
    gener_basic.h \
    cpumeter.h \
    midifio.h \
    sfwriter.h \
    sampletable.h \
    backend.jack.h \
    frontend.h \
    frontend.portmidi.h \
    noteinput.h \
    trapezoid.h \
    presetenums.h

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
           warningpopup.cpp \
    synth.cpp \
    application.cpp \
    meterupdater.cpp \
    soundbenchmain-slots.cpp \
    soundbenchmain-func.cpp \
    settings_basicgen.cpp \
    gener_basic.cpp \
    soundbenchmain-init.cpp \
    midifio-read.cpp \
    midifio-readeropen.cpp \
    midifio-misc.cpp \
    sfwriter.cpp \
    player-load.cpp \
    player-write.cpp \
    sampletable.cpp \
    backend.jack.cpp \
    frontend.portmidi.cpp \
    frontend.portmidi-parse.cpp \
    noteinput.cpp \
    trapezoid.cpp

RESOURCES += sbMainResources.qrc

unix: LIBS += -L/usr/lib/  -lsndfile


#Configuration switches to remove certain functionality and dependencies where necessary.
!noPortAudio {
    unix: LIBS += -lportaudio -lportaudiocpp
}
noPortAudio {
    DEFINES += NO_PORTAUDIO
}

!noPortMidi{
    unix: LIBS += -lportmidi
}
noPortMidi {
    DEFINES += NO_MIDI
}


OTHER_FILES += \
    ProgrammingStyleRegulations.txt \
    BUILDING \
    COPYING
