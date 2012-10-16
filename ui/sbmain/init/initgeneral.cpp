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

    sc_new = new QShortcut(QKeySequence("Ctrl+N"),this);
    sc_open = new QShortcut(QKeySequence("Ctrl+O"),this);
    sc_save = new QShortcut(QKeySequence("Ctrl+S"),this);
    sc_export = new QShortcut(QKeySequence("Ctrl+Shift+S"),this);
}

void SoundbenchMain::delayedConstructor() {
    sb::global_srate = sb::SupportedRates[1];

    syn = new sb::Synth;
    em = new sb::Emitter(syn);
    met = new CpuMeter(ui->cpuMeter,ui->cpuLabel);
    plai = new sb::Player(syn,ui->songsTracksList,met);

    resetBlueprint();

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
    sbdata.cd("Soundbench");
#else
    if (!sbdata.exists(".soundbench")) {
        std::cerr << "Setting up directories...\n";
        sbdata.mkdir(".soundbench");
    }
    sbdata.cd(".soundbench");
#endif

    datadir = sbdata.absolutePath().toStdString();
    if(!sbdata.exists("presets")) {
        std::cerr << "Setting up presets directory.\n";
        sbdata.mkdir("presets");
    }

    //Set up the device, port, and preset lists.
    loadPresetList();
    displayPresets();

    loadPorts();
    loadDevices();

    //Set up the sigmaps.
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
    if (sb::global_srate == sb::SupportedRates[0])
        ui->button441Sampling->setChecked(true);
    else if (sb::global_srate == sb::SupportedRates[1])
        ui->button48Sampling->setChecked(true);
    else if (sb::global_srate == sb::SupportedRates[2])
        ui->button882Sampling->setChecked(true);
    else if (sb::global_srate == sb::SupportedRates[3])
        ui->button96Sampling->setChecked(true);
    else if (sb::global_srate == sb::SupportedRates[4])
        ui->button176Sampling->setChecked(true);
    else if (sb::global_srate == sb::SupportedRates[5])
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

    //Connect and setup the main page widgets and actions.
    connect(ui->silenceButton,SIGNAL(clicked()),SLOT(silence()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),SLOT(setMasterVolume(int)));
    connect(ui->playButton,SIGNAL(toggled(bool)),SLOT(playSynth(bool)));
    connect(ui->mainTabs,SIGNAL(currentChanged(int)),SLOT(holdKeyboard(int)));

    //Connect and setup the Presets page widgets.
    connect(ui->newButton,SIGNAL(clicked()),SLOT(resetBlueprint()));
    connect(ui->refreshPresets,SIGNAL(clicked()),SLOT(refreshPresets()));
    connect(ui->openButton,SIGNAL(clicked()),SLOT(loadExternalPreset()));
    connect(ui->exportPresetButton,SIGNAL(clicked()),SLOT(exportPreset()));
    connect(ui->saveButton,SIGNAL(clicked()),SLOT(savePreset()));
    connect(ui->deleteButton,SIGNAL(clicked()),SLOT(deletePreset()));
    connect(ui->presetList,SIGNAL(clicked(QModelIndex)),SLOT(loadInternalPreset()));
    connect(ui->editMetadata,SIGNAL(toggled(bool)),SLOT(editMetadata(bool)));
    connect(ui->presetLine,SIGNAL(textEdited(QString)),SLOT(displayPresets()));
    if (ui->mainTabs->currentIndex() == 0)
        ui->presetLine->grabKeyboard();
    connect(sc_new,SIGNAL(activated()),SLOT(resetBlueprint()));
    connect(sc_open,SIGNAL(activated()),SLOT(loadExternalPreset()));
    connect(sc_save,SIGNAL(activated()),SLOT(savePreset()));
    connect(sc_export,SIGNAL(activated()),SLOT(exportPreset()));

    //Connect and setup the Setup page widgets.
    initSetupPage();

    //Connect and setup the Channels page widgets.
    connect(ui->gener1TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));
    connect(ui->gener2TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));
    connect(ui->gener3TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));
    connect(ui->gener4TypeBox,SIGNAL(currentIndexChanged(int)),type_sigmap,SLOT(map()));

    connect(ui->gen1SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));
    connect(ui->gen2SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));
    connect(ui->gen3SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));
    connect(ui->gen4SettButton,SIGNAL(clicked()),sett_sigmap,SLOT(map()));

    //Connect the sigmaps.
    connect(sett_sigmap,SIGNAL(mapped(int)),SLOT(setGenSett(int)));
    connect(type_sigmap,SIGNAL(mapped(int)),SLOT(setGenType(int)));
    connect(rate_sigmap,SIGNAL(mapped(int)),SLOT(setSampleRate(int)));

    ui->versionLabel->setText(SBVERSION);

    em->start();
    plai->startRt();
    met->startMeter();
}
