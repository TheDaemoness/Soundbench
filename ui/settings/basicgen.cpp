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

#include "ui/settings/basicgen.h"
#include "ui_basicgen.h"

BasicGenerSettings::BasicGenerSettings(size_t chan, sb::Blueprint* blu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicGenerSettings)
{
    affectedblu = blu;
    affectedchan = chan;
    ui->setupUi(this);

    ui->ampBox->setValue(blu->gener_data[chan][sb::GenBasicAmp].pod.sample);
    ui->phaseBox->setValue(blu->gener_data[chan][sb::GenBasicPhase].pod.sample);
    ui->noteBiasBox->setValue(blu->gener_data[chan][sb::GenBasicNoteBias].pod.value);
    ui->attackBox->setValue(blu->gener_data[chan][sb::GenerAttackTime].pod.decim);
    ui->releaseBox->setValue(blu->gener_data[chan][sb::GenerReleaseTime].pod.decim);
    ui->waveTypeBox->setCurrentIndex(blu->gener_data[chan][sb::GenBasicWave].pod.value-1);

    connect(ui->applyButton,SIGNAL(clicked()),SLOT(affect()));
    connect(ui->closeButton,SIGNAL(clicked()),SLOT(close()));
}

void BasicGenerSettings::affect() {
    affectedblu->gener_data[affectedchan][sb::GenBasicAmp] = sb::makeParamfromSample(ui->ampBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicPhase] = sb::makeParamfromSample(ui->phaseBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicNoteBias] = sb::makeParamfromInt(ui->noteBiasBox->value());
    affectedblu->gener_data[affectedchan][sb::GenerAttackTime] = sb::makeParamfromFloat(ui->attackBox->value());
    affectedblu->gener_data[affectedchan][sb::GenerReleaseTime] = sb::makeParamfromFloat(ui->releaseBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(ui->waveTypeBox->currentIndex()+1);
    close();
}

BasicGenerSettings::~BasicGenerSettings()
{
    delete ui;
}
