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
    QString chosenfile = QFileDialog::getOpenFileName(
                NULL,"Choose a file to import...", "~/", "Standard MIDI Files (*.mid *.smf);;All Files (*)");

    if (!chosenfile.size()) {
        em->start();
        met->startMeter();
        return;
    }
    plai->setFile(chosenfile.toStdString());
    plai->loadFile();
    ui->exportButton->setEnabled(true);
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
}

void SoundbenchMain::exportOpen() {
    if(!plai->ready())
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
    std::cerr << "Tempo: " << tiem << '\n';
    plai->setTempo(tiem);
    plai->loadTrack(ui->songsTracksList->currentRow());
}

void SoundbenchMain::testSynth(bool b) {
    if (b)
        syn->noteOn(0,1.0);
    else
        syn->noteOff(0);
}

void SoundbenchMain::playSynth(bool b) {
    if (!plai->ready())
        return;
    if (b)
        plai->play();
    else
        plai->stop();
}

void SoundbenchMain::silence() {
    stopAndReset();
    em->start();
}

void SoundbenchMain::setMasterVolume(int) {
    syn->volume() = static_cast<sbSample>(ui->volumeSlider->value())/ui->volumeSlider->maximum();
}

void SoundbenchMain::setSampleRate(int which) {
    ui->playButton->setChecked(false);
    em->setSamplingRate(sb::sampling_rates[which]);
    arch->buildSynth(syn,blu);
    em->start();
}

void SoundbenchMain::closePopups() {
    /*The purpose of this is to close all the popups that do not block the interface.
        All non-blocking popups take care of freeing themselves.*/
}

void SoundbenchMain::setGenType(int which) {
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

void SoundbenchMain::setGenSett(int which) {
    stopAndReset();
    genSetts(which);
    arch->buildSynth(syn,blu);
    syn->reset();
    em->start();
}

void SoundbenchMain::setTrack(int e) {
    plai->loadTrack(static_cast<uint16_t>(e));
    ui->tempoBox->setValue(plai->getTempo());
    met->startMeter();
}

void SoundbenchMain::setPoly(int tracks) {
    stopAndReset();
    syn->setPolyphony(tracks);
    em->start();
}
