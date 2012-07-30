/*
    This file is part of Soundbench.

    Soundbench is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Soundbench is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Soundbench.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012  Amaya S.
*/

#ifndef SOUNDBENCHMAIN_H
#define SOUNDBENCHMAIN_H

#include "ui_soundbenchmain.h"

#include "emitter/emitter.h"
#include "architect/architect.h"
#include "synth.h"
#include "player/player.h"

#include "ui/settings/basicgen.h"
#include "cpumeter.h"

#include <QSignalMapper>
#include <QFileDialog>
#include <QThread>
#include <QShortcut>

namespace Ui {
    class SoundbenchMain: public Ui_SoundBenchMain {};
} // namespace Ui

class SoundbenchMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit SoundbenchMain(QWidget *parent = 0);
    ~SoundbenchMain();
    void delayedConstructor();

public slots:
    //void disableRtAu();
    //void disableMidi();

private slots:
    void reloadEmitter();
    void reloadPlayer();
    void importOpen();
    void exportOpen();
    void testSynth(bool);
    void playSynth(bool);
    void silence();
    void setMasterVolume(int);
    void setSampleRate(int);
    void setGenType(int);
    void setGenSett(int);
    void setTrack();
    void setTempo(int);
    void setPoly(int);
    void loadDevices();
    void loadPorts();
    void setPort();
    void useVPort(bool);
    void record(bool);
    void editMetadata(bool);

    void refreshPresets(); //New preset.
    void resetBlueprint();
    void loadInternalPreset();
    void loadExternalPreset();
    void deletePreset();
    void importPreset();
    void savePreset();
    void exportPreset();
    void holdKeyboard(int);
    void displayPresets();

private:
    void genSetts(size_t i);
    void writePresetRecord();
    void loadPresetList();
    void stopAndReset();
    void updateToBlueprint();
    void resetSelectUI();

    std::vector <sb::PresetMeta> presetlist;
    size_t currpreset;
    sb::PresetMeta externalpresetdata;
    bool external;

    Ui::SoundbenchMain *ui;
    sb::Synth* syn;
    sb::Blueprint* blu;
    sb::Architect* arch;
    sb::Emitter* em;
    sb::Player* plai;

    QShortcut *sc_new, *sc_open, *sc_export, *sc_save;

    std::string datadir;

    QSignalMapper *sett_sigmap, *type_sigmap, *rate_sigmap;

    union {
        BasicGenerSettings* basic;
    } gsd;

    CpuMeter* met;
};

#endif // SOUNDBENCHMAIN_H
