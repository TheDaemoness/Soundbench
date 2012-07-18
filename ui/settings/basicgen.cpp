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

    if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].pod.value) == sb::TriangleWave)
        ui->waveTypeBox->setCurrentIndex(1);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].pod.value) == sb::SquareWave)
        ui->waveTypeBox->setCurrentIndex(2);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].pod.value) == sb::SawtoothWave)
        ui->waveTypeBox->setCurrentIndex(3);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].pod.value) == sb::OvalWave)
        ui->waveTypeBox->setCurrentIndex(4);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].pod.value) == sb::PeakWave)
        ui->waveTypeBox->setCurrentIndex(5);
    else
        ui->waveTypeBox->setCurrentIndex(0);

    connect(ui->applyButton,SIGNAL(clicked()),SLOT(affect()));
    connect(ui->closeButton,SIGNAL(clicked()),SLOT(close()));
}

void BasicGenerSettings::affect() {
    affectedblu->gener_data[affectedchan][sb::GenBasicAmp] = sb::makeParamfromSample(ui->ampBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicPhase] = sb::makeParamfromSample(ui->phaseBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicNoteBias] = sb::makeParamfromInt(ui->noteBiasBox->value());
    affectedblu->gener_data[affectedchan][sb::GenerAttackTime] = sb::makeParamfromFloat(ui->attackBox->value());
    affectedblu->gener_data[affectedchan][sb::GenerReleaseTime] = sb::makeParamfromFloat(ui->releaseBox->value());
    switch(ui->waveTypeBox->currentIndex()) {
    case 1:
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::TriangleWave);
        break;
    case 2:
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::SquareWave);
        break;
    case 3:
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::SawtoothWave);
        break;
    case 4:
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::OvalWave);
        break;
    case 5:
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::PeakWave);
        break;
    default:
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::SineWave);
        break;
    }
    close();
}

BasicGenerSettings::~BasicGenerSettings()
{
    delete ui;
}
