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

#include "biquadfx.h"
#include "ui_biquadfx.h"

BiquadFxSettings::BiquadFxSettings(size_t chan, size_t num, sb::Blueprint* blu, size_t srate, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BiquadFxSettings) {

    this->blu = blu;
    this->chan = chan;
    this->num = num;
    ui->setupUi(this);

    if(blu->eff_data[chan][num][sb::FxBiquadType].pod.value == sb::BiquadManual) {
        ui->tabs->setCurrentIndex(1);
        ui->filterTypeBox->setCurrentIndex(0);
    }
    else {
        ui->tabs->setCurrentIndex(0);
        ui->filterTypeBox->setCurrentIndex(blu->eff_data[chan][num][sb::FxBiquadType].pod.value-1);
    }

    typechanged(blu->eff_data[chan][num][sb::FxBiquadType].pod.value-1);
    ui->freqSpinBox->setMaximum(srate/2);
    ui->freqSpinBox->setSingleStep(srate/60);

    ui->freqSpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadFreq].pod.decim);
    ui->qSpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadQ].pod.decim);
    ui->bwSpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadBW].pod.decim);
    ui->eqSpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadGain].pod.decim);
    ui->sSpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadS].pod.decim);

    ui->a0SpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadA0].pod.decim);
    ui->a1SpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadA1].pod.decim);
    ui->a2SpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadA2].pod.decim);
    ui->b0SpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadB0].pod.decim);
    ui->b1SpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadB1].pod.decim);
    ui->b2SpinBox->setValue(blu->eff_data[chan][num][sb::FxBiquadB2].pod.decim);

    connect(ui->filterTypeBox,SIGNAL(currentIndexChanged(int)),SLOT(typechanged(int)));
    connect(ui->applyButton,SIGNAL(clicked()),SLOT(affect()));
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(close()));
}

void BiquadFxSettings::affect() {
    if(ui->tabs->currentIndex()==1)
        blu->eff_data[chan][num][sb::FxBiquadType] = sb::makeParamfromInt(sb::BiquadManual);
    else
        blu->eff_data[chan][num][sb::FxBiquadType] = sb::makeParamfromInt(ui->filterTypeBox->currentIndex()+1);

    blu->eff_data[chan][num][sb::FxBiquadFreq] = sb::makeParamfromFloat(ui->freqSpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadQ] = sb::makeParamfromFloat(ui->qSpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadBW] = sb::makeParamfromFloat(ui->bwSpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadGain] = sb::makeParamfromFloat(ui->eqSpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadS] = sb::makeParamfromFloat(ui->sSpinBox->value());

    blu->eff_data[chan][num][sb::FxBiquadA0] = sb::makeParamfromFloat(ui->a0SpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadA1] = sb::makeParamfromFloat(ui->a1SpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadA2] = sb::makeParamfromFloat(ui->a2SpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadB0] = sb::makeParamfromFloat(ui->b0SpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadB1] = sb::makeParamfromFloat(ui->b1SpinBox->value());
    blu->eff_data[chan][num][sb::FxBiquadB2] = sb::makeParamfromFloat(ui->b2SpinBox->value());

    close();
}

void BiquadFxSettings::typechanged(int neotype) {
    neotype += 1;
    switch (static_cast<sb::BiquadTypes>(neotype)) {
    case sb::BiquadLowpass:
    case sb::BiquadHighpass:
    case sb::BiquadAllpass:
        ui->qSpinBox->setEnabled(true);
        ui->bwSpinBox->setEnabled(false);
        ui->eqSpinBox->setEnabled(false);
        ui->sSpinBox->setEnabled(false);
        break;
    case sb::BiquadBandpassQ:
    case sb::BiquadBandpassZero:
    case sb::BiquadNotch:
    case sb::BiquadPeakingEQ:
        ui->qSpinBox->setEnabled(false);
        ui->bwSpinBox->setEnabled(true);
        ui->eqSpinBox->setEnabled(false);
        ui->sSpinBox->setEnabled(false);
        break;
    case sb::BiquadHighShelf:
    case sb::BiquadLowShelf:
        ui->qSpinBox->setEnabled(false);
        ui->bwSpinBox->setEnabled(false);
        ui->eqSpinBox->setEnabled(true);
        ui->sSpinBox->setEnabled(true);
        break;
    default:
        break;
    }
}

BiquadFxSettings::~BiquadFxSettings() {
    delete ui;
}
