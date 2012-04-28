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

#include "emitter.h"
#include "architect.h"
#include "synth.h"
#include "player.h"

#include "gentypedialog.h"
#include "settings_basicgen.h"
#include "meterupdater.h"

#include <QSignalMapper>
#include <QTimer>
#include <QFileDialog>

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

    void updateCPU(double use) {
        ui->cpuMeter->setValue(use*1000);
    }

public slots:
    //void disableRtAu();
    //void disableMidi();

private slots:
    //Implemented in soundbenchmain-slots.cpp
    void importOpen();
    void exportOpen();
    void restyleCPUMeter();
    void testSynth(bool);
    void playSynth(bool);
    void silence();
    void setMasterVolume(int);
    void setSampleRate(int);
    void closePopups();
    void setGenType(int);
    void setGenSett(int);

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
    QTimer* teimer;

    GenTypeDialog* gtd;
    union {
        BasicGenerSettings* basic;
    } gsd;
    MeterUpdater* metup;

    bool cpumeter_orange;
};

#endif // SOUNDBENCHMAIN_H
