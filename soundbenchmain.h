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
#include "midi.h"

#include "gentypedialog.h"
#include "settings.basicgen.h"

#include <limits>
#include <functional>

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
    void testSynth(bool b) {
        if (b)
            syn->noteOn(36,1.0);
        else {
            syn->noteOff(36);
            syn->reset();
        }
    }
    void silence() {
        stopAndReset();
        em->start();
    }
    void setMasterVolume(int) {
        syn->volume() = static_cast<sbSample>(ui->volumeSlider->value())/ui->volumeSlider->maximum();
    }

    //TODO: Clean all of this up. Qt does have something to deal with situations like these, after all.
    void setSampleRate1() {
        em->setSamplingRate(sb::sampling_rates[0]);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setSampleRate2() {
        em->setSamplingRate(sb::sampling_rates[1]);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setSampleRate3() {
        em->setSamplingRate(sb::sampling_rates[2]);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setSampleRate4() {
        em->setSamplingRate(sb::sampling_rates[3]);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setSampleRate5() {
        em->setSamplingRate(sb::sampling_rates[4]);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setSampleRate6() {
        em->setSamplingRate(sb::sampling_rates[5]);
        arch->buildSynth(syn,blu);
        em->start();
    }

    void setGenType1() {
        gtd = new GenTypeDialog(&blu->gener[0],&blu->gener_data[0],ui->gener1Label,ui->gen1SettButton);
        stopAndReset();
        gtd->exec();
        delete gtd;
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenType2() {
        gtd = new GenTypeDialog(&blu->gener[1],&blu->gener_data[1],ui->gener2Label,ui->gen2SettButton);
        stopAndReset();
        gtd->exec();
        delete gtd;
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenType3() {
        gtd = new GenTypeDialog(&blu->gener[2],&blu->gener_data[2],ui->gener3Label,ui->gen3SettButton);
        stopAndReset();
        gtd->exec();
        delete gtd;
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenType4() {
        gtd = new GenTypeDialog(&blu->gener[3],&blu->gener_data[3],ui->gener4Label,ui->gen4SettButton);
        stopAndReset();
        gtd->exec();
        delete gtd;
        arch->buildSynth(syn,blu);
        em->start();
    }

    void setGenSett1() {
        stopAndReset();
        setGenSett(0);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenSett2() {
        stopAndReset();
        setGenSett(1);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenSett3() {
        stopAndReset();
        setGenSett(2);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenSett4() {
        stopAndReset();
        setGenSett(3);
        arch->buildSynth(syn,blu);
        em->start();
    }
    //END TODO

private:
    void setGenSett(size_t i);
    inline void stopAndReset() {
        em->stop();
        syn->reset();
        ui->playButton->setChecked(false);
    }

    Ui::SoundbenchMain *ui;
    sb::Synth* syn;
    sb::Blueprint* blu;
    sb::Architect* arch;
    sb::Emitter* em;

    GenTypeDialog* gtd;
    union {
        BasicGenerSettings* basic;
    } gsd;
};

#endif // SOUNDBENCHMAIN_H
