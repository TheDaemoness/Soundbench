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

#include "soundbenchmain.h"

SoundbenchMain::SoundbenchMain(QWidget *parent) :
    QMainWindow(parent)
{
    arch = new sb::Architect;
    blu = new sb::Blueprint;
    arch->planDefaultSynth(blu);
    em = NULL;
    syn = NULL;
    ui = new Ui::SoundbenchMain();
    ui->setupUi(this);
    for (char i = 0; i < 4; ++i)
        arch->planDefaultBasicGen(blu,i);
}

void SoundbenchMain::delayedConstructor() {
    sb::curr_srate = sb::sampling_rates[1];
    syn = new sb::Synth;
    arch->buildSynth(syn,blu);

    em = new sb::Emitter(syn);

    syn->volume() = static_cast<sbSample>(ui->volumeSlider->value())/ui->volumeSlider->maximum();

    std::map<sb::emitter_type,bool> backend_apis = em->getSupportedAPIs(); //TODO: Deal with this.

    //Check off the sampling rate being used.
    if (em->getSampleRate() == sb::sampling_rates[0])
        ui->button441Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::sampling_rates[1])
        ui->button48Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::sampling_rates[2])
        ui->button882Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::sampling_rates[3])
        ui->button96Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::sampling_rates[4])
        ui->button176Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::sampling_rates[5])
        ui->button192Sampling->setChecked(true);

    //Disable the sampling rates that the platform does not support. I'm looking at you, 192k...
    for(std::pair<const size_t,bool> elem : em->getSupportedRates()) {
        if(!elem.second) {
            if(elem.first == sb::sampling_rates[0])
                ui->button441Sampling->setEnabled(false);
            else if (elem.first == sb::sampling_rates[1])
                ui->button48Sampling->setEnabled(false);
            else if (elem.first == sb::sampling_rates[2])
                ui->button882Sampling->setEnabled(false);
            else if (elem.first == sb::sampling_rates[3])
                ui->button96Sampling->setEnabled(false);
            else if (elem.first == sb::sampling_rates[4])
                ui->button176Sampling->setEnabled(false);
            else if (elem.first == sb::sampling_rates[5])
                ui->button192Sampling->setEnabled(false);
            }
        }

    //Connect the main page widgets.
    connect(ui->silenceButton,SIGNAL(clicked()),SLOT(silence()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),SLOT(setMasterVolume(int)));

    //Connect the Channels widgets
    connect(ui->gen1TypeButton,SIGNAL(clicked()),SLOT(setGenType1()));
    connect(ui->gen2TypeButton,SIGNAL(clicked()),SLOT(setGenType2()));
    connect(ui->gen3TypeButton,SIGNAL(clicked()),SLOT(setGenType3()));
    connect(ui->gen4TypeButton,SIGNAL(clicked()),SLOT(setGenType4()));

    connect(ui->gen1SettButton,SIGNAL(clicked()),SLOT(setGenSett1()));
    connect(ui->gen2SettButton,SIGNAL(clicked()),SLOT(setGenSett2()));
    connect(ui->gen3SettButton,SIGNAL(clicked()),SLOT(setGenSett3()));
    connect(ui->gen4SettButton,SIGNAL(clicked()),SLOT(setGenSett4()));


    //Connect the Settings page widgets.
    connect(ui->button441Sampling,SIGNAL(clicked()),SLOT(setSampleRate1()));
    connect(ui->button48Sampling,SIGNAL(clicked()),SLOT(setSampleRate2()));
    connect(ui->button882Sampling,SIGNAL(clicked()),SLOT(setSampleRate3()));
    connect(ui->button96Sampling,SIGNAL(clicked()),SLOT(setSampleRate4()));
    connect(ui->button176Sampling,SIGNAL(clicked()),SLOT(setSampleRate5()));
    connect(ui->button192Sampling,SIGNAL(clicked()),SLOT(setSampleRate6()));

    connect(ui->playButton,SIGNAL(toggled(bool)),SLOT(testSynth(bool)));

    show();

    em->start();
}

void SoundbenchMain::setGenSett(size_t i) {
    if (blu->gener[i] == sb::Blueprint::generBasic) {
        stopAndReset();
        gsd.basic = new BasicGenerSettings(i,blu);
        gsd.basic->exec();
        delete gsd.basic;
    }
    else
        std::cerr << "Please rebuild Soundbench with the appropriate generator settings buttons *disabled*.\n\n";
}

SoundbenchMain::~SoundbenchMain() {
    delete arch;
    if (em != NULL)
        delete em;
    if (syn != NULL)
        delete syn;
    delete blu;
    delete ui;
}
