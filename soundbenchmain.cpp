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
    em = nullptr;
    syn = nullptr;
    leest = nullptr;
    cpumeter_orange = false;

    ui = new Ui::SoundbenchMain();
    ui->setupUi(this);
    sett_sigmap = new QSignalMapper;
    type_sigmap = new QSignalMapper;
    rate_sigmap = new QSignalMapper;

    arch->planAllDefaults(blu);
}

void SoundbenchMain::delayedConstructor() {
    sb::curr_srate = sb::sampling_rates[1];

    syn = new sb::Synth;
    arch->buildSynth(syn,blu);
    em = new sb::Emitter(syn);
    plai = new sb::Player(syn);

    syn->volume() = static_cast<sbSample>(ui->volumeSlider->value())/ui->volumeSlider->maximum();
    teimer = new QTimer;
    metup = new MeterUpdater(ui->cpuMeter);

    std::map<sb::emitter_type,bool> backend_apis = em->getSupportedAPIs(); //TODO: Deal with this.

    type_sigmap->setMapping(ui->gen1TypeButton,1);
    type_sigmap->setMapping(ui->gen2TypeButton,2);
    type_sigmap->setMapping(ui->gen3TypeButton,3);
    type_sigmap->setMapping(ui->gen4TypeButton,4);

    sett_sigmap->setMapping(ui->gen1SettButton,1);
    sett_sigmap->setMapping(ui->gen2SettButton,2);
    sett_sigmap->setMapping(ui->gen3SettButton,3);
    sett_sigmap->setMapping(ui->gen4SettButton,4);

    rate_sigmap->setMapping(ui->button441Sampling,0);
    rate_sigmap->setMapping(ui->button48Sampling,1);
    rate_sigmap->setMapping(ui->button882Sampling,2);
    rate_sigmap->setMapping(ui->button96Sampling,3);
    rate_sigmap->setMapping(ui->button176Sampling,4);
    rate_sigmap->setMapping(ui->button192Sampling,5);

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
    connect(ui->playButton,SIGNAL(toggled(bool)),SLOT(playSynth(bool)));
    connect(teimer,SIGNAL(timeout()),metup,SLOT(update()));
    connect(teimer,SIGNAL(timeout()),SLOT(restyleCPUMeter()));
    connect(ui->mainTabs,SIGNAL(currentChanged(int)),SLOT(closePopups()));

    //Conncet the Player page widgets.
    connect(ui->eventListButton,SIGNAL(clicked()),SLOT(openEventList()));
    connect(ui->holdA4Button,SIGNAL(toggled(bool)),SLOT(testSynth(bool)));

    //Connect the Channels page widgets.
    connect(ui->gen1TypeButton,SIGNAL(clicked()),type_sigmap,SLOT(map()));
    connect(ui->gen2TypeButton,SIGNAL(clicked()),type_sigmap,SLOT(map()));
    connect(ui->gen3TypeButton,SIGNAL(clicked()),type_sigmap,SLOT(map()));
    connect(ui->gen4TypeButton,SIGNAL(clicked()),type_sigmap,SLOT(map()));

    connect(ui->gen1SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));
    connect(ui->gen2SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));
    connect(ui->gen3SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));
    connect(ui->gen4SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));

    //Connect the Settings page widgets
    connect(ui->button441Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button48Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button882Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button96Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button176Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button192Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));

    //Connect the sigmaps.
    connect(sett_sigmap,SIGNAL(mapped(int)),SLOT(setGenSett(int)));
    connect(type_sigmap,SIGNAL(mapped(int)),SLOT(setGenType(int)));
    connect(rate_sigmap,SIGNAL(mapped(int)),SLOT(setSampleRate(int)));

    ui->versionLabel->setText(SBVERSION);

    show();

    teimer->start(10);
    em->start();
}

void SoundbenchMain::genSetts(size_t i) {

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
    teimer->stop();

    delete arch;
    delete plai;
    if (em != nullptr)
        delete em;
    if (syn != nullptr)
        delete syn;
    delete blu;

    delete sett_sigmap;
    delete type_sigmap;
    delete rate_sigmap;

    delete leest;

    delete teimer;

    delete ui;
}
