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

    ui->outputsList->setEnabled(em->isRtAvailable() && em->getUsedBackendType() != sb::JACK_O);
    ui->outputReload->setEnabled(em->isRtAvailable() && em->getUsedBackendType() != sb::JACK_O);
    if (em->doesBackendUsePorts()) {
        ui->outputsList->addItem("Backend uses ports.");
        ui->outputsList->addItem("Please use the 'Connect' interface.");
    }

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

    connect(ui->button441Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button48Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button882Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button96Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button176Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->button192Sampling,SIGNAL(clicked()),rate_sigmap,SLOT(map()));
    connect(ui->polyphonyBox,SIGNAL(valueChanged(int)),SLOT(setPoly(int)));
    ui->polyphonyBox->setValue(sb::DefaultPolyphony);
}
