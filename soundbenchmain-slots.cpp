#include "soundbenchmain.h"

void SoundbenchMain::importOpen() {
    QString chosenfile = QFileDialog::getOpenFileName(
                NULL,"Choose a file to import...", "~/", "Standard MIDI Files (*.mid *.smf);;All Files (*)");
    if (!chosenfile.size())
        return;
    plai->loadFile(chosenfile.toStdString());
}

void SoundbenchMain::exportOpen() {
    QString chosenfile = QFileDialog::getSaveFileName(
                NULL,"Choose a file to export to...", "~/",
                "Wave File (*.wav);;AIFF File (*.aiff);;Standard MIDI File (*.mid);;Headerless RAW (*.raw);;MAT5 Binary Data File (*.mat)");
    if (!chosenfile.size())
        return;
    plai->writeFile(chosenfile.toStdString());
}

void SoundbenchMain::restyleCPUMeter() {
    if(((metup->artificial_limit() && (ui->cpuMeter->value() >= 600)) || (ui->cpuMeter->value() >= 750)) && !cpumeter_orange) {
        ui->cpuMeter->setStyleSheet("QProgressBar::chunk {background-image: url(:/meters/cpumeter_warn.png); background-position: center left; background-repeat: 0;} QProgressBar {border-image: url(:/meters/cpumeter_off.png); color: black;}");
        cpumeter_orange = true;
    }
    else if (((metup->artificial_limit() && (ui->cpuMeter->value() <= 500)) || ui->cpuMeter->value() <= 600) && cpumeter_orange) {
        ui->cpuMeter->setStyleSheet("");
        cpumeter_orange = false;
    }

}

void SoundbenchMain::testSynth(bool b) {
    if (b)
        syn->noteOn(12.0,1.0);
    else
        syn->noteOff(12.0);
}

void SoundbenchMain::playSynth(bool b) {
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
