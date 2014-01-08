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
    gen_sett_sigmap = new QSignalMapper;
    gen_type_sigmap = new QSignalMapper;
    rate_sigmap = new QSignalMapper;
    emit_sigmap = new QSignalMapper;
    fx_num_sigmap = new QSignalMapper;
    fx_sett_sigmap = new QSignalMapper;
    fx_type_sigmap = new QSignalMapper;
    fx_fb_sigmap = new QSignalMapper;

    sc_new = new QShortcut(QKeySequence("Ctrl+N"),this);
    sc_open = new QShortcut(QKeySequence("Ctrl+O"),this);
    sc_save = new QShortcut(QKeySequence("Ctrl+S"),this);
    sc_save_as = new QShortcut(QKeySequence("Ctrl+Shift+S"),this);
    sc_import = new QShortcut(QKeySequence("Ctrl+I"),this);
}

void SoundbenchMain::delayedConstructor() {
    sb::global_srate = sb::SupportedRates[1];

    syn = new sb::Synth;
    std::cerr << "Threading Level: ";
    if(syn->getThreadLevel() == sb::ThreadingPartial)
        std::cerr << "Partial\n";
    else if(syn->getThreadLevel() == sb::ThreadingChannelwise)
        std::cerr << "Channelwise\n";
    else
        std::cerr << "None\n";

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
    gen_type_sigmap->setMapping(ui->gener1TypeBox,0);
    gen_type_sigmap->setMapping(ui->gener2TypeBox,1);
    gen_type_sigmap->setMapping(ui->gener3TypeBox,2);
    gen_type_sigmap->setMapping(ui->gener4TypeBox,3);

    gen_sett_sigmap->setMapping(ui->gen1SettButton,0);
    gen_sett_sigmap->setMapping(ui->gen2SettButton,1);
    gen_sett_sigmap->setMapping(ui->gen3SettButton,2);
    gen_sett_sigmap->setMapping(ui->gen4SettButton,3);

    fx_num_sigmap->setMapping(ui->fxNumberBox1,0);
    fx_num_sigmap->setMapping(ui->fxNumberBox2,1);
    fx_num_sigmap->setMapping(ui->fxNumberBox3,2);
    fx_num_sigmap->setMapping(ui->fxNumberBox4,3);

    fx_sett_sigmap->setMapping(ui->fx1SettButton,0);
    fx_sett_sigmap->setMapping(ui->fx2SettButton,1);
    fx_sett_sigmap->setMapping(ui->fx3SettButton,2);
    fx_sett_sigmap->setMapping(ui->fx4SettButton,3);

    fx_type_sigmap->setMapping(ui->fx1TypeBox,0);
    fx_type_sigmap->setMapping(ui->fx2TypeBox,1);
    fx_type_sigmap->setMapping(ui->fx3TypeBox,2);
    fx_type_sigmap->setMapping(ui->fx4TypeBox,3);

    fx_fb_sigmap->setMapping(ui->fx1Feedback,0);
    fx_fb_sigmap->setMapping(ui->fx2Feedback,1);
    fx_fb_sigmap->setMapping(ui->fx3Feedback,2);
    fx_fb_sigmap->setMapping(ui->fx4Feedback,3);

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
    connect(sc_save_as,SIGNAL(activated()),SLOT(exportPreset()));

    //Connect and setup the Setup page widgets.
    initSetupPage();

    //Connect and setup the Channels page widgets.
    initChannelsPage();

    ui->versionLabel->setText(SBVERSION);

    em->start();
    plai->startRt();
    met->startMeter();
}
