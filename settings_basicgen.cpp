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

#include "settings_basicgen.h"
#include "ui_basicgenersettings.h"

BasicGenerSettings::BasicGenerSettings(size_t chan, sb::Blueprint* blu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicGenerSettings)
{
    affectedblu = blu;
    affectedchan = chan;
    ui->setupUi(this);

    ui->ampBox->setValue(blu->gener_data[chan][sb::genBasic_amplutide].sample);
    ui->phaseBox->setValue(blu->gener_data[chan][sb::genBasic_phase].sample);
    ui->noteBiasBox->setValue(blu->gener_data[chan][sb::genBasic_notebias].value);

    if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::genBasic_wave].value) == sb::TriangleWave)
        ui->triangleWave->setChecked(true);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::genBasic_wave].value) == sb::SquareWave)
        ui->squareWave->setChecked(true);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::genBasic_wave].value) == sb::SawtoothWave)
        ui->sawtoothWave->setChecked(true);
    else if (static_cast<sb::SimpleWaveTypes>(blu->gener_data[chan][sb::genBasic_wave].value) == sb::OvalWave)
        ui->ovalWave->setChecked(true);
    else
        ui->sineWave->setChecked(true);

    connect(ui->applyButton,SIGNAL(clicked()),SLOT(affect()));
    connect(ui->closeButton,SIGNAL(clicked()),SLOT(close()));
}

void BasicGenerSettings::affect() {
    affectedblu->gener_data[affectedchan][sb::genBasic_amplutide] = sb::makeParamfromSample(ui->ampBox->value());
    affectedblu->gener_data[affectedchan][sb::genBasic_phase] = sb::makeParamfromSample(ui->phaseBox->value());
    affectedblu->gener_data[affectedchan][sb::genBasic_notebias] = sb::makeParamfromInt(ui->noteBiasBox->value());
    if (ui->sineWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::genBasic_wave] = sb::makeParamfromInt(sb::SineWave);
    else if (ui->triangleWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::genBasic_wave] = sb::makeParamfromInt(sb::TriangleWave);
    else if (ui->squareWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::genBasic_wave] = sb::makeParamfromInt(sb::SquareWave);
    else if (ui->sawtoothWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::genBasic_wave] = sb::makeParamfromInt(sb::SawtoothWave);
    else if (ui->ovalWave->isChecked())
        affectedblu->gener_data[affectedchan][sb::genBasic_wave] = sb::makeParamfromInt(sb::OvalWave);
    close();
}

BasicGenerSettings::~BasicGenerSettings()
{
    delete ui;
}
