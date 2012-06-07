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

#include "gentypedialog.h"
#include "ui_gentypedialog.h"

GenTypeDialog::GenTypeDialog(sb::GenerType* tochange, sb::Blueprint::ModuleDataType* tofill, QLabel* text, QPushButton* butt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenTypeDialog)
{
    ui->setupUi(this);
    affectedgen = tochange;
    affectedargs = tofill;
    affectedlabel = text;
    affectedbutt = butt;
    if(*affectedgen == sb::NoGener)
        ui->disabledButton->setChecked(true);
    else if(*affectedgen == sb::GenerSampler)
        ui->sampleButton->setChecked(true);
    else if(*affectedgen == sb::GenerBasic)
        ui->basicButton->setChecked(true);
    else if(*affectedgen == sb::GenerAdditive)
        ui->additiveButton->setChecked(true);
    else if(*affectedgen == sb::GenerModal)
        ui->modalButton->setChecked(true);
    else if(*affectedgen == sb::GenerWaveguide)
        ui->waveguideButton->setChecked(true);
    connect(ui->applyButton,SIGNAL(clicked()),SLOT(apply()));
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(close()));
}

void GenTypeDialog::apply() {

    affectedbutt->setToolTip("Change this generator's settings.");
    affectedbutt->setEnabled(true);
    if(ui->disabledButton->isChecked()) {
        affectedbutt->setEnabled(false);
        affectedbutt->setToolTip("Disabled generators have no settings.");
        affectedlabel->setText("Disabled");
        *affectedgen = sb::NoGener;
    }
    else if(ui->sampleButton->isChecked()) {
        affectedlabel->setText("Sampler");
        *affectedgen = sb::GenerSampler;
    }
    else if(ui->basicButton->isChecked()) {
        affectedlabel->setText("Basic");
        *affectedgen = sb::GenerBasic;
        //TODO: sb::Architect should have something to deal with basic settings.
    }
    else if(ui->additiveButton->isChecked()) {
        affectedlabel->setText("Additive");
        *affectedgen = sb::GenerAdditive;
    }
    else if(ui->modalButton->isChecked()) {
        affectedlabel->setText("Modal");
        *affectedgen = sb::GenerModal;
    }
    else if(ui->waveguideButton->isChecked()) {
        affectedlabel->setText("Banded Waveguide");
        *affectedgen = sb::GenerWaveguide;
    }
    this->close();
}

GenTypeDialog::~GenTypeDialog()
{
    delete ui;
}
