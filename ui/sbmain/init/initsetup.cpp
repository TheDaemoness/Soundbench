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

void SoundbenchMain::initSetupPage() {
    rate_sigmap->setMapping(ui->button441Sampling,0);
    rate_sigmap->setMapping(ui->button48Sampling,1);
    rate_sigmap->setMapping(ui->button882Sampling,2);
    rate_sigmap->setMapping(ui->button96Sampling,3);
    rate_sigmap->setMapping(ui->button176Sampling,4);
    rate_sigmap->setMapping(ui->button192Sampling,5);

    emit_sigmap->setMapping(ui->jackAudioButton,static_cast<int>(sb::JACK_O));
    emit_sigmap->setMapping(ui->portAudioButton,static_cast<int>(sb::PortAudio_O));
    emit_sigmap->setMapping(ui->rtAudioButton,static_cast<int>(sb::RtAudio_O));

    connect(ui->holdA4Button,SIGNAL(toggled(bool)),SLOT(testSynth(bool)));
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
    connect(ui->recordButton,SIGNAL(toggled(bool)),SLOT(record(bool)));

    connect(ui->jackAudioButton,SIGNAL(clicked()),emit_sigmap,SLOT(map()));
    connect(ui->portAudioButton,SIGNAL(clicked()),emit_sigmap,SLOT(map()));
    connect(ui->rtAudioButton,SIGNAL(clicked()),emit_sigmap,SLOT(map()));

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

    loadDevices();

    connect(ui->outputRetry,SIGNAL(clicked()),SLOT(reloadEmitter()));
    connect(ui->outputReload,SIGNAL(clicked()),SLOT(loadDevices()));
    connect(ui->playButton,SIGNAL(toggled(bool)),SLOT(playSynth(bool)));
    connect(plai,SIGNAL(donePlaying()),ui->playButton,SLOT(toggle()));

    if(!em->isRtAvailable())
        ui->outputsList->addItem("Backend initialization failed.");
#else
    ui->outputsList->addItem("Compiled without any backends.");
    ui->outputsList->setDisabled(true);
    ui->outputReload->setDisabled(true);
#endif

    auto audio_apis = em->getSupportedAPIs();
    if(!audio_apis[sb::JACK_O])
        ui->jackAudioButton->setDisabled(true);
    if(!audio_apis[sb::PortAudio_O])
        ui->portAudioButton->setDisabled(true);
    if(!audio_apis[sb::RtAudio_O])
        ui->rtAudioButton->setDisabled(true);

    auto usedbackend = em->getCurrentAPI();
    if(usedbackend == sb::JACK_O)
        ui->jackAudioButton->setChecked(true);
    else if(usedbackend == sb::PortAudio_O)
        ui->portAudioButton->setChecked(true);
    else if(usedbackend == sb::RtAudio_O)
        ui->rtAudioButton->setChecked(true);

    if(plai->isRtAvailable())
        ui->rtMidiButton->setChecked(true);
    else
        ui->rtMidiButton->setDisabled(true);

    connect(ui->button441Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button48Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button882Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button96Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button176Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button192Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->polyphonyBox,SIGNAL(valueChanged(int)),SLOT(setPoly(int)));
    ui->polyphonyBox->setValue(sb::DefaultPolyphony);
}
