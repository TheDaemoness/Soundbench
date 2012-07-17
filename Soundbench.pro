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
    blueprint.h \
    common.h \
    emitter/emitter.h \
    errorpopup.h \
    ui/sbmain/soundbenchmain.h \
    synth.h \
    warningpopup.h \
    meterupdater.h \
    player/player.h \
    midichain.h \
    problemfix.h \
    backend/base.h \
    sbutilities.h \
    ui/settings/basicgen.h \
    soundbases.h \
    gener/basic.h \
    cpumeter.h \
    midifio/midifio.h \
    sfwriter.h \
    sampletable.h \
    frontend/base.h \
    trapezoid.h \
    presetenums.h \
    ticker.h \
    printhelp.h \
    waves/waves.h \
    waves/basicwaves.h \
    backend/portaudio.h \
    backend/rtaudio.h \
    frontend/rtmidi.h \
    midievents.h

FORMS += errorpopup.ui \
    ui/sbmain/soundbenchmain.ui \
    warningpopup.ui \
    ui/settings/basicgen.ui

SOURCES += emitter/initportaudio.cpp \
    architect.cpp \
    backend/portaudio.cpp \
    emitter/emitter.cpp \
    errorpopup.cpp \
    main.cpp \
    warningpopup.cpp \
    synth.cpp \
    meterupdater.cpp \
    ui/settings/basicgen.cpp \
    gener/basic.cpp \
    midifio/read.cpp \
    midifio/readeropen.cpp \
    midifio/misc.cpp \
    sfwriter.cpp \
    sampletable.cpp \
    trapezoid.cpp \
    emitter/initsomething.cpp \
    ui/sbmain/mainfunc.cpp \
    ui/sbmain/maininit.cpp \
    ui/sbmain/mainslots.cpp \
    waves/basicwaves.cpp \
    backend/rtaudio.cpp \
    emitter/initrtaudio.cpp \
    frontend/rtmidi.cpp \
    makenode.cpp \
    ui/sbmain/mainplayer.cpp \
    ui/sbmain/mainchannels.cpp \
    doevent.cpp \
    player/playermisc.cpp \
    player/midiinit.cpp \
    player/playerload.cpp \
    player/playerwrite.cpp \
    player/playerinit.cpp

RESOURCES += \
    resources.qrc

unix: LIBS += -L/usr/lib/

LIBS += -lsndfile

#Configuration switches to remove certain functionality and dependencies where necessary.
!noPortAudio {
    LIBS += -lportaudio -lportaudiocpp
}
noPortAudio {
    DEFINES += NO_PORTAUDIO
}

!noRtAudio{
    LIBS += -lrtaudio
}
noRtAudio {
    DEFINES += NO_RTAUDIO
}

!noRtMidi{
    LIBS += -lrtmidi
}
noRtMidi {
    DEFINES += NO_RTMIDI
}


OTHER_FILES += \
    ProgrammingStyleRegulations.txt \
    COPYING \
    README
