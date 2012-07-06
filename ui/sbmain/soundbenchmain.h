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
#include "architect.h"
#include "synth.h"
#include "player/player.h"

#include "ui/settings/basicgen.h"
#include "cpumeter.h"

#include <QSignalMapper>
#include <QFileDialog>
#include <QThread>

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
    void importOpen();
    void exportOpen();
    void testSynth(bool);
    void playSynth(bool);
    void silence();
    void setMasterVolume(int);
    void setSampleRate(int);
    void setGenType(int);
    void setGenSett(int);
    void setTrack(int);
    void setTempo(int);
    void setPoly(int);
    void loadDevices();
    void loadPorts();

private:
    void genSetts(size_t i);
    inline void stopAndReset() {
        em->stop();
        syn->reset();
        ui->playButton->setChecked(false);
        ui->holdA4Button->setChecked(false);
    }

    Ui::SoundbenchMain *ui;
    sb::Synth* syn;
    sb::Blueprint* blu;
    sb::Architect* arch;
    sb::Emitter* em;
    sb::Player* plai;

    QSignalMapper *sett_sigmap, *type_sigmap, *rate_sigmap;

    union {
        BasicGenerSettings* basic;
    } gsd;

    CpuMeter* met;
};

#endif // SOUNDBENCHMAIN_H
