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

    ui->ampBox->setValue(blu->gener_data[chan][sb::GenBasicAmp].sample);
    ui->phaseBox->setValue(blu->gener_data[chan][sb::GenBasicPhase].sample);
    ui->noteBiasBox->setValue(blu->gener_data[chan][sb::GenBasicNoteBias].value);
    ui->noteBiasBox->setValue(blu->gener_data[chan][sb::GenerAttackTime].decim);
    ui->noteBiasBox->setValue(blu->gener_data[chan][sb::GenerReleaseTime].decim);

    if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].value) == sb::TriangleWave)
        ui->triangleWave->setChecked(true);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].value) == sb::SquareWave)
        ui->squareWave->setChecked(true);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].value) == sb::SawtoothWave)
        ui->sawtoothWave->setChecked(true);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::GenBasicWave].value) == sb::OvalWave)
        ui->ovalWave->setChecked(true);
    else
        ui->sineWave->setChecked(true);

    connect(ui->applyButton,SIGNAL(clicked()),SLOT(affect()));
    connect(ui->closeButton,SIGNAL(clicked()),SLOT(close()));
}

void BasicGenerSettings::affect() {
    affectedblu->gener_data[affectedchan][sb::GenBasicAmp] = sb::makeParamfromSample(ui->ampBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicPhase] = sb::makeParamfromSample(ui->phaseBox->value());
    affectedblu->gener_data[affectedchan][sb::GenBasicNoteBias] = sb::makeParamfromInt(ui->noteBiasBox->value());
    affectedblu->gener_data[affectedchan][sb::GenerAttackTime] = sb::makeParamfromFloat(ui->attackBox->value());
    affectedblu->gener_data[affectedchan][sb::GenerReleaseTime] = sb::makeParamfromFloat(ui->releaseBox->value());
    if (ui->sineWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::SineWave);
    else if (ui->triangleWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::TriangleWave);
    else if (ui->squareWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::SquareWave);
    else if (ui->sawtoothWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::SawtoothWave);
    else if (ui->ovalWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::GenBasicWave] = sb::makeParamfromInt(sb::OvalWave);
    close();
}

BasicGenerSettings::~BasicGenerSettings()
{
    delete ui;
}
