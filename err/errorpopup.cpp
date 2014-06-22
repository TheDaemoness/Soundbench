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

#include "err/errorpopup.h"
#include "ui_errorpopup.h"

const char ErrorPopup::fixcount;

ErrorPopup::ErrorPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorPopup)
{
    ui->setupUi(this);
    whichfix = 0;
    wasfixed = false;
    for(unsigned char i = 0; i < fixcount; ++i)
        fixes[i] = nullptr;
    ui->fix1Choice->setDisabled(true);
    ui->fix2Choice->setDisabled(true);
    ui->fix3Choice->setDisabled(true);
    ui->fix4Choice->setDisabled(true);
    ui->fix5Choice->setDisabled(true);
    ui->fix6Choice->setDisabled(true);
    ui->fix7Choice->setDisabled(true);
    connect(ui->applyButton,SIGNAL(clicked()),SLOT(takeAction()));
}

void ErrorPopup::takeAction() {
    close();
    if (ui->fix1Choice->isChecked())
        wasfixed = fixes[0]->runFix();
    else if (ui->fix2Choice->isChecked())
        wasfixed = fixes[1]->runFix();
    else if (ui->fix3Choice->isChecked())
        wasfixed = fixes[2]->runFix();
    else if (ui->fix4Choice->isChecked())
        wasfixed = fixes[3]->runFix();
    else if (ui->fix5Choice->isChecked())
        wasfixed = fixes[4]->runFix();
    else if (ui->fix6Choice->isChecked())
        wasfixed = fixes[5]->runFix();
    else if (ui->fix7Choice->isChecked())
        wasfixed = fixes[6]->runFix();
}

void ErrorPopup::setErrorText(std::string txt) {
    ui->errorText->setText(txt.c_str());
}

void ErrorPopup::setInfoText(std::string txt) {
    ui->infoText->setText(txt.c_str());
}

void ErrorPopup::addFix(sb::errs::fixes::ProblemFix* thefix, std::string desc, std::string comments) {
    if (desc.empty())
        desc = thefix->getDesc();
    if (comments.empty())
        comments = thefix->getComments();
    unsigned char i = 0;
    for(;i < fixcount ; ++i) {
        if (fixes[i] == nullptr) {
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
        if (fixes[i] != nullptr)
            delete fixes[i];
    }
    delete ui;
}
