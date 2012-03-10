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

GenTypeDialog::GenTypeDialog(sb::Blueprint::gener_type* tochange, sb::Blueprint::ModuleDataType* tofill, QLabel* text, QPushButton* butt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenTypeDialog)
{
    ui->setupUi(this);
    affectedgen = tochange;
    affectedargs = tofill;
    affectedlabel = text;
    affectedbutt = butt;
    if(*affectedgen == sb::Blueprint::NoGener)
        ui->disabledButton->setChecked(true);
    else if(*affectedgen == sb::Blueprint::generSampler)
        ui->sampleButton->setChecked(true);
    else if(*affectedgen == sb::Blueprint::generBasic)
        ui->basicButton->setChecked(true);
    else if(*affectedgen == sb::Blueprint::generAdditive)
        ui->additiveButton->setChecked(true);
    else if(*affectedgen == sb::Blueprint::generModal)
        ui->modalButton->setChecked(true);
    else if(*affectedgen == sb::Blueprint::generWaveguide)
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
        *affectedgen = sb::Blueprint::NoGener;
    }
    else if(ui->sampleButton->isChecked()) {
        affectedlabel->setText("Sampler");
        *affectedgen = sb::Blueprint::generSampler;
    }
    else if(ui->basicButton->isChecked()) {
        affectedlabel->setText("Basic");
        *affectedgen = sb::Blueprint::generBasic;
        //TODO: sb::Architect should have something to deal with basic settings.
    }
    else if(ui->additiveButton->isChecked()) {
        affectedlabel->setText("Additive");
        *affectedgen = sb::Blueprint::generAdditive;
    }
    else if(ui->modalButton->isChecked()) {
        affectedlabel->setText("Modal");
        *affectedgen = sb::Blueprint::generModal;
    }
    else if(ui->waveguideButton->isChecked()) {
        affectedlabel->setText("Banded Waveguide");
        *affectedgen = sb::Blueprint::generWaveguide;
    }
    this->close();
}

GenTypeDialog::~GenTypeDialog()
{
    delete ui;
}
