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
#include "settings.basicgen.h"
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

    void importOpen() {
        QString chosenfile = QFileDialog::getOpenFileName(
                NULL,"Choose a file to import...", "~/", "Standard MIDI Files (*.mid *.smf);;All Files (*)");
        if (!chosenfile.size())
            return;
        plai->loadFile(chosenfile.toStdString());
    }
    void exportOpen() {
        QString chosenfile = QFileDialog::getSaveFileName(
                NULL,"Choose a file to export to...", "~/",
                "Wave File (*.wav);;AIFF File (*.aiff);;Standard MIDI File (*.mid);;Headerless RAW (*.raw);;MAT5 Binary Data File (*.mat)");
        if (!chosenfile.size())
            return;
        plai->writeFile(chosenfile.toStdString());
    }

    void restyleCPUMeter() {
        if(((metup->artificial_limit() && (ui->cpuMeter->value() >= 600)) || (ui->cpuMeter->value() >= 750)) && !cpumeter_orange) {
            ui->cpuMeter->setStyleSheet(R"del(QProgressBar::chunk {
                                            background-image: url(:/meters/cpumeter_warn.png);
                                            background-position: center left;
                                            background-repeat: 0;
                                            }
                                            QProgressBar {
                                            border-image: url(:/meters/cpumeter_off.png);
                                            color: black;
                                            })del");
        }
        else if (!((metup->artificial_limit() && (ui->cpuMeter->value() >= 600)) || ui->cpuMeter->value() >= 750) && cpumeter_orange)
            ui->cpuMeter->setStyleSheet("");

    }

    void testSynth(bool b) {
        if (b)
            syn->noteOn(12.0,1.0);
        else {
            syn->noteOff(12.0);
            syn->reset();
        }
    }
    void playSynth(bool b) {
        if (b)
            plai->play();
        else
            plai->stop();
    }

    void silence() {
        stopAndReset();
        em->start();
    }
    void setMasterVolume(int) {
        syn->volume() = static_cast<sbSample>(ui->volumeSlider->value())/ui->volumeSlider->maximum();
    }

    void setSampleRate(int which) {
        ui->playButton->setChecked(false);
        em->setSamplingRate(sb::sampling_rates[which]);
        arch->buildSynth(syn,blu);
        em->start();
    }
    void closePopups() {
        /*The purpose of this is to close all the popups that do not block the interface.
        All non-blocking popups take care of freeing themselves.*/
    }

    void setGenType(int which) {
        switch (which) {
        case 1:
            gtd = new GenTypeDialog(&blu->gener[1],&blu->gener_data[1],ui->gener1Label,ui->gen1SettButton);
            break;
        case 2:
            gtd = new GenTypeDialog(&blu->gener[2],&blu->gener_data[2],ui->gener2Label,ui->gen2SettButton);
            break;
        case 3:
            gtd = new GenTypeDialog(&blu->gener[3],&blu->gener_data[3],ui->gener3Label,ui->gen3SettButton);
            break;
        case 4:
            gtd = new GenTypeDialog(&blu->gener[4],&blu->gener_data[4],ui->gener4Label,ui->gen4SettButton);
            break;
        default:
            return;
        }
        stopAndReset();
        gtd->exec();
        delete gtd;
        arch->buildSynth(syn,blu);
        em->start();
    }
    void setGenSett(int which) {
        stopAndReset();
        genSetts(which);
        arch->buildSynth(syn,blu);
        em->start();
    }

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
