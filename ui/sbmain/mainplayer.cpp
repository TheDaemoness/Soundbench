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


void SoundbenchMain::importOpen() {
    stopAndReset();
    met->pauseMeter();
    QString chosenfile = QFileDialog::getOpenFileName(
                NULL,"Choose a file to import...", "~/", "Standard MIDI Files (*.mid *.smf);;All Files (*)");

    if (!chosenfile.size()) {
        em->start();
        met->startMeter();
        return;
    }
    plai->setFile(chosenfile.toStdString());
    if(!plai->loadFile()) {
        em->start();
        met->startMeter();
        return;
    }
    ui->exportButton->setEnabled(true);
    ui->playButton->setEnabled(em->isRtAvailable());
    ui->songsTracksList->setCurrentRow(0);

    ui->tempoBox->setValue(plai->getTempo());
    if (plai->getTempo()) {
        ui->tempoBox->setEnabled(true);
        ui->tempoBox->setToolTip("Change the starting tempo of the current track.");
    }
    else {
        ui->tempoBox->setDisabled(true);
        ui->tempoBox->setToolTip("This MIDI file does not represent time in terms of beats, and thus the tempo box has been disabled.");
    }

    em->start();
    met->startMeter();
}

void SoundbenchMain::exportOpen() {
    if(plai->empty())
        return;
    stopAndReset();
    met->setProgress(0);
    QString chosenfile = QFileDialog::getSaveFileName(
                NULL,"Choose a file to export to...", "~/",
                "Wave File (*.wav);;AIFF File (*.aiff);;Free Lossless Audio Codec File (*.flac);;Headerless RAW file (*.raw);;MAT5 Binary Data File (*.mat)"); //Put .mid back in here in version 0.3.0.
    if (!chosenfile.size()) {
        em->start();
        met->startMeter();
        return;
    }

    plai->setFile(chosenfile.toStdString());
    plai->writeFile();
    syn->reset();
    met->startMeter();
    em->start();
}

void SoundbenchMain::setTempo(int tiem) {
    plai->setTempo(tiem);
    plai->loadTrack(ui->songsTracksList->currentRow());
}

void SoundbenchMain::playSynth(bool b) {
    if (plai->empty() || !em->isRtAvailable()) //No point running this on an empty Player or a dead Emitter.
        return;
    if (b)
        plai->startPlay();
    else {
        plai->stopPlay();
        syn->reset();
    }
}

void SoundbenchMain::loadPorts() {
    if (!plai->isRtAvailable())
        return;
    ui->inputsList->clear();
    for(std::string dev : plai->getPorts())
        ui->inputsList->addItem(dev.c_str());
    ui->inputsList->setCurrentRow(plai->getCurrentPort());
}
void SoundbenchMain::loadDevices() {
    if (!em->isRtAvailable())
        return;
    ui->outputsList->clear();
    for(std::string dev : em->getDevices())
        ui->outputsList->addItem(dev.c_str());
    ui->outputsList->setCurrentRow(em->getCurrentDevice());
}

void SoundbenchMain::setPort() {
    if (!plai->isRtAvailable())
        return;
    plai->stopRt();
    plai->setPort(ui->inputsList->currentRow());
    plai->startRt();
}

void SoundbenchMain::useVPort(bool use) {
    if (!plai->isRtAvailable())
        return;
    if (use) {
        ui->inputsList->clear();
        plai->stopRt();
        plai->setVirtualPort(true);
        plai->startRt();
    }
    else {
        plai->stopRt();
        plai->setVirtualPort(false);
        loadPorts();
        setPort();
        plai->startRt();
    }
}

void SoundbenchMain::setTrack() {
    if(!plai->isOpen())
        return;
    if(!plai->loadTrack(static_cast<uint16_t>(ui->songsTracksList->currentRow())))
        return;
    ui->tempoBox->setValue(plai->getTempo());
    met->startMeter();
}

void SoundbenchMain::record(bool rec) {
    if (!plai->isRtAvailable())
        return;

    if (rec) {
        std::cerr << "Began recording.\n";
        plai->startRec();
    }
    else {
        ui->songsTracksList->clear();
        if (plai->isOpen())
            plai->close();

        plai->stopRec();

        if(!plai->empty()) {
            ui->songsTracksList->addItem("Recorded Track");
            ui->tempoBox->setDisabled(true);
            ui->playButton->setEnabled(em->isRtAvailable());
            ui->tempoBox->setToolTip("Soundbench does not allow the user to use the tempo box with their own tracks at this time.");
        }
        else {
            ui->tempoBox->setDisabled(true);
            ui->playButton->setDisabled(true);
            ui->tempoBox->setToolTip("Change the starting tempo of the current track.");
        }
        ui->exportButton->setEnabled(!plai->empty());
    }
}


void SoundbenchMain::reloadEmitter() {
    plai->stopRt();
    stopAndReset();
    em->~Emitter();
    new (em) sb::Emitter(syn);
    em->start();
    plai->startRt();

    ui->outputRetry->setDisabled(em->isRtAvailable());

    ui->outputReload->setEnabled(em->isRtAvailable());
    ui->outputsList->setEnabled(em->isRtAvailable());
    ui->playButton->setEnabled(em->isRtAvailable() && !plai->empty());

    if(em->isRtAvailable())
        loadDevices();
}

void SoundbenchMain::reloadPlayer() {
    plai->stopRt();
    stopAndReset();
    plai->~Player();
    new (plai) sb::Player(syn,ui->songsTracksList,met);
    em->start();
    plai->startRt();

    ui->inputRetry->setDisabled(plai->isRtAvailable());

    ui->inputsList->setEnabled(plai->isRtAvailable());
    ui->inputReload->setEnabled(plai->isRtAvailable());
    ui->recordButton->setEnabled(plai->isRtAvailable());
    ui->inputVirtual->setEnabled(plai->setVirtualPort(false));

    if(plai->isRtAvailable())
       loadPorts();
}
