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

#include "errorpopup.h"
#include "ui_errorpopup.h"

const char ErrorPopup::fixcount;

ErrorPopup::ErrorPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorPopup)
{
    ui->setupUi(this);
    whichfix = 0;
    fixed = false;
    for(unsigned char i = 0; i < fixcount; ++i)
        fixes[i] = NULL;
    connect(ui->applyButton,SIGNAL(clicked()),SLOT(takeAction()));
}

void ErrorPopup::takeAction() {
    close();
    if (ui->fix1Choice->isChecked())
        fixed = fixes[0]->fixit();
    else if (ui->fix2Choice->isChecked())
        fixed = fixes[1]->fixit();
    else if (ui->fix3Choice->isChecked())
        fixed = fixes[2]->fixit();
    else if (ui->fix4Choice->isChecked())
        fixed = fixes[3]->fixit();
    else if (ui->fix5Choice->isChecked())
        fixed = fixes[4]->fixit();
    else if (ui->fix6Choice->isChecked())
        fixed = fixes[5]->fixit();
    else if (ui->fix7Choice->isChecked())
        fixed = fixes[6]->fixit();
}

void ErrorPopup::setErrorText(std::string txt) {
    ui->errorText->setText(txt.c_str());
}

void ErrorPopup::setInfoText(std::string txt) {
    ui->infoText->setText(txt.c_str());
}

void ErrorPopup::addFix(sb::errs::ProblemFix* thefix, std::string desc, std::string comments) {
    unsigned char i = 0;
    for(;i < fixcount ; ++i) {
        if (fixes[i] == NULL) {
            fixes[i] = thefix;
            break;
        }
    }
    switch(i) {
    case 0:
        ui->fix1Choice->setText(desc.c_str());
        ui->fix1Choice->setToolTip(comments.c_str());
        ui->fix1Choice->setEnabled(true);
        break;
    case 1:
        ui->fix2Choice->setText(desc.c_str());
        ui->fix2Choice->setToolTip(comments.c_str());
        ui->fix2Choice->setEnabled(true);
        break;
    case 2:
        ui->fix3Choice->setText(desc.c_str());
        ui->fix3Choice->setToolTip(comments.c_str());
        ui->fix3Choice->setEnabled(true);
        break;
    case 3:
        ui->fix4Choice->setText(desc.c_str());
        ui->fix4Choice->setToolTip(comments.c_str());
        ui->fix4Choice->setEnabled(true);
        break;
    case 4:
        ui->fix5Choice->setText(desc.c_str());
        ui->fix5Choice->setToolTip(comments.c_str());
        ui->fix5Choice->setEnabled(true);
        break;
    case 5:
        ui->fix6Choice->setText(desc.c_str());
        ui->fix6Choice->setToolTip(comments.c_str());
        ui->fix6Choice->setEnabled(true);
        break;
    default:
        ui->fix7Choice->setText(desc.c_str());
        ui->fix7Choice->setToolTip(comments.c_str());
        ui->fix7Choice->setEnabled(true);
        break;
    }

}

ErrorPopup::~ErrorPopup()
{
    for(unsigned char i = 0; i < fixcount; ++i) {
        if (fixes[i] != NULL)
            delete fixes[i];
    }
    delete ui;
}
