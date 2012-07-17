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

#include "ui/sbmain/soundbenchmain.h"

SoundbenchMain::SoundbenchMain(QWidget *parent) : QMainWindow(parent) {
    arch = new sb::Architect;
    blu = new sb::Blueprint;
    em = nullptr;
    syn = nullptr;

    ui = new Ui::SoundbenchMain();
    ui->setupUi(this);
    sett_sigmap = new QSignalMapper;
    type_sigmap = new QSignalMapper;
    rate_sigmap = new QSignalMapper;

    arch->planAllDefaults(blu);
}

void SoundbenchMain::delayedConstructor() {
    sb::SampleRate = sb::SupportedRates[1];

    syn = new sb::Synth;
    arch->buildSynth(syn,blu);
    em = new sb::Emitter(syn);
    met = new CpuMeter(ui->cpuMeter,ui->cpuLabel);
    plai = new sb::Player(syn,ui->songsTracksList,met);

    syn->volume() = static_cast<SbSample>(ui->volumeSlider->value())/ui->volumeSlider->maximum();

    std::map<sb::EmitterType,bool> backend_apis = em->getSupportedAPIs(); //TODO: Deal with this.

    //Set up the directories if they're not already in place.
    QDir sbdata(QDir::home());
#if defined(SB_ENV_MACOS)
    sbdata.cd("Library");
    if (!sbdata.exists("Soundbench")) {
        std::cerr << "Setting up directories...\n";
        sbdata.mkdir("Soundbench");
    }
    sbdata.cd("Soundbench");
#elif defined(SB_ENV_WNDOS)
    sbdata.cd("AppData");
    if (!sbdata.exists("Soundbench")) {
        std::cerr << "Setting up directories...\n";
        sbdata.mkdir("Soundbench");
    }
    sbdata.mkdir("Soundbench");
#else
    if (!sbdata.exists(".soundbench")) {
        std::cerr << "Setting up directories...\n";
        sbdata.mkdir(".soundbench");
    }
    sbdata.cd(".soundbench");
#endif
    if(!sbdata.exists("presets")) {
        std::cerr << "Setting up presets directory.\n";
        sbdata.mkdir("presets");
    }

    loadPorts();
    loadDevices();

    type_sigmap->setMapping(ui->gener1TypeBox,0);
    type_sigmap->setMapping(ui->gener2TypeBox,1);
    type_sigmap->setMapping(ui->gener3TypeBox,2);
    type_sigmap->setMapping(ui->gener4TypeBox,3);

    sett_sigmap->setMapping(ui->gen1SettButton,0);
    sett_sigmap->setMapping(ui->gen2SettButton,1);
    sett_sigmap->setMapping(ui->gen3SettButton,2);
    sett_sigmap->setMapping(ui->gen4SettButton,3);

    rate_sigmap->setMapping(ui->button441Sampling,0);
    rate_sigmap->setMapping(ui->button48Sampling,1);
    rate_sigmap->setMapping(ui->button882Sampling,2);
    rate_sigmap->setMapping(ui->button96Sampling,3);
    rate_sigmap->setMapping(ui->button176Sampling,4);
    rate_sigmap->setMapping(ui->button192Sampling,5);

    //Check off the sampling rate being used.
    if (em->getSampleRate() == sb::SupportedRates[0])
        ui->button441Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::SupportedRates[1])
        ui->button48Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::SupportedRates[2])
        ui->button882Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::SupportedRates[3])
        ui->button96Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::SupportedRates[4])
        ui->button176Sampling->setChecked(true);
    else if (em->getSampleRate() == sb::SupportedRates[5])
        ui->button192Sampling->setChecked(true);

    //Disable the sampling rates that the platform does not support. I'm looking at you, 192k...
    for(std::pair<const size_t,bool> elem : em->getSupportedRates()) {
        if(!elem.second) {
            if(elem.first == sb::SupportedRates[0])
                ui->button441Sampling->setEnabled(false);
            else if (elem.first == sb::SupportedRates[1])
                ui->button48Sampling->setEnabled(false);
            else if (elem.first == sb::SupportedRates[2])
                ui->button882Sampling->setEnabled(false);
            else if (elem.first == sb::SupportedRates[3])
                ui->button96Sampling->setEnabled(false);
            else if (elem.first == sb::SupportedRates[4])
                ui->button176Sampling->setEnabled(false);
            else if (elem.first == sb::SupportedRates[5])
                ui->button192Sampling->setEnabled(false);
        }
    }

    //Connect the main page widgets.
    connect(ui->silenceButton,SIGNAL(clicked()),SLOT(silence()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),SLOT(setMasterVolume(int)));
    connect(ui->playButton,SIGNAL(toggled(bool)),SLOT(playSynth(bool)));

    //Conncet the Player page widgets and disable a few buttons.
    connect(ui->holdA4Button,SIGNAL(toggled(bool)),SLOT(testSynth(bool)));
    connect(ui->recordButton,SIGNAL(toggled(bool)),SLOT(record(bool)));
    connect(ui->importButton,SIGNAL(clicked()),SLOT(importOpen()));
    connect(ui->exportButton,SIGNAL(clicked()),SLOT(exportOpen()));
    connect(ui->songsTracksList,SIGNAL(clicked(QModelIndex)),SLOT(setTrack()));
    connect(ui->tempoBox,SIGNAL(valueChanged(int)),SLOT(setTempo(int)));
    ui->exportButton->setDisabled(true);
#ifndef NO_MIDIFRONTEND
    ui->inputRetry->setDisabled(plai->isRtAvailable());

    ui->inputsList->setEnabled(plai->isRtAvailable());
    ui->inputReload->setEnabled(plai->isRtAvailable());
    ui->recordButton->setEnabled(plai->isRtAvailable());
    ui->inputVirtual->setEnabled(plai->setVirtualPort(false)); //Effectively a support check.

    connect(ui->inputsList,SIGNAL(clicked(QModelIndex)),SLOT(setPort()));
    connect(ui->inputVirtual,SIGNAL(toggled(bool)),SLOT(useVPort(bool)));
    connect(ui->inputReload,SIGNAL(clicked()),SLOT(loadPorts()));
    connect(ui->inputRetry,SIGNAL(clicked()),SLOT(reloadPlayer()));

    if(!plai->isRtAvailable())
        ui->inputsList->addItem("Frontend initialization failed.");
#else
    ui->inputsList->addItem("Compiled without any frontends.");
    ui->inputsList->setDisabled(true);
    ui->recordButton->setDisabled(true);
    ui->inputVirtual->setDisabled(true);
    ui->inputReload->setDisabled(true);
#endif
#ifndef NO_AUDIOBACKEND
    ui->outputRetry->setDisabled(em->isRtAvailable());

    ui->outputsList->setEnabled(em->isRtAvailable());
    ui->outputReload->setEnabled(em->isRtAvailable());

    connect(ui->outputRetry,SIGNAL(clicked()),SLOT(reloadEmitter()));
    connect(ui->outputReload,SIGNAL(clicked()),SLOT(loadDevices()));

    if(!em->isRtAvailable())
        ui->outputsList->addItem("Backend initialization failed.");
#else
    ui->outputsList->addItem("Compiled without any backends.");
    ui->outputsList->setDisabled(true);
    ui->outputReload->setDisabled(true);
#endif

    //Connect the Channels page widgets.
    connect(ui->gener1TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));
    connect(ui->gener2TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));
    connect(ui->gener3TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));
    connect(ui->gener4TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));

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
    connect(ui->polyphonyBox,SIGNAL(valueChanged(int)),SLOT(setPoly(int)));
    ui->polyphonyBox->setValue(sb::DefaultPolyphony);

    //Connect the sigmaps.
    connect(sett_sigmap,SIGNAL(mapped(int)),SLOT(setGenSett(int)));
    connect(type_sigmap,SIGNAL(mapped(int)),SLOT(setGenType(int)));
    connect(rate_sigmap,SIGNAL(mapped(int)),SLOT(setSampleRate(int)));

    ui->versionLabel->setText(SBVERSION);

    em->start();
    plai->startRt();
    met->startMeter();
}
