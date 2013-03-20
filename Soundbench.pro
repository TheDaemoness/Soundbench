#######################################################################
# Automatically generated by the Soundbench team at some point in time.
#######################################################################

QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -pedantic
QMAKE_CXXFLAGS += -std=c++0x

DEFINES += IS_SOUNDBENCH

TEMPLATE = app
TARGET = 
DEPENDPATH += .

unix: INCLUDEPATH += /usr/include/

# Input
HEADERS += architect/architect.h \
    blueprint.h \
    common.h \
    emitter/emitter.h \
    ui/sbmain/soundbenchmain.h \
    meterupdater.h \
    player/player.h \
    midichain.h \
    backend/base.h \
    sbutilities.h \
    ui/settings/basicgen.h \
    soundbases.h \
    gener/basic.h \
    cpumeter.h \
    midifio/midifio.h \
    sfwriter.h \
    frontend/base.h \
    presetenums.h \
    printhelp.h \
    waves/waves.h \
    waves/basicwaves.h \
    backend/portaudio.h \
    backend/rtaudio.h \
    frontend/rtmidi.h \
    midievents.h \
    fileparsers.h \
    filewriters.h \
    backend/jackaudio.h \
    emitter/initbackend.h \
    frontend/jackmidi.h \
    synth/synth.h \
    ui/settings/biquadfx.h \
    fx/biquad.h \
    gener/sampler.h \
    util/floatticker.h \
    util/intticker.h \
    util/jackbase.h \
    err/problemfix.h \
    err/errorpopup.h \
    err/warningpopup.h \
    envelope/trapezoid.h \
    sampletable/periodictable.h

FORMS += errorpopup.ui \
    ui/sbmain/soundbenchmain.ui \
    warningpopup.ui \
    ui/settings/basicgen.ui \
    ui/settings/biquadfx.ui

SOURCES += \
    architect/architect.cpp \
    backend/portaudio.cpp \
    emitter/emitter.cpp \
    errorpopup.cpp \
    main.cpp \
    warningpopup.cpp \
    meterupdater.cpp \
    ui/settings/basicgen.cpp \
    gener/basic.cpp \
    sfwriter.cpp \
    waves/basicwaves.cpp \
    backend/rtaudio.cpp \
    frontend/rtmidi.cpp \
    makenode.cpp \
    ui/sbmain/mainchannels.cpp \
    doevent.cpp \
    player/playermisc.cpp \
    player/midiinit.cpp \
    player/playerload.cpp \
    player/playerwrite.cpp \
    player/playerinit.cpp \
    architect/presetload.cpp \
    architect/presetmeta.cpp \
    architect/presetsave.cpp \
    ui/sbmain/mainmisc.cpp \
    midifio/fiomisc.cpp \
    midifio/fioread.cpp \
    midifio/fioreadopen.cpp \
    backend/jackaudio.cpp \
    frontend/jackmidi.cpp \
    synth/synth.cpp \
    ui/sbmain/init/initgeneral.cpp \
    ui/sbmain/init/initsetup.cpp \
    ui/sbmain/mainsetup.cpp \
    ui/sbmain/mainpresetload.cpp \
    ui/sbmain/mainpresetsave.cpp \
    ui/sbmain/mainpreset.cpp \
    ui/settings/biquadfx.cpp \
    fx/biquad.cpp \
    fx/biquad_ctrl.cpp \
    synth/synthticks.cpp \
    architect/architectdefaults.cpp \
    ui/sbmain/init/initchannels.cpp \
    gener/sampler.cpp \
    envelope/trapezoid.cpp \
    sampletable/periodictable.cpp

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

!noJack{
    LIBS += -ljack
}
noJack {
    DEFINES += NO_RTMIDI
}


OTHER_FILES += \
    ProgrammingStyleRegulations.txt \
    COPYING \
    README \
    PresetSpecification.txt \
    RecordSpecification.txt
