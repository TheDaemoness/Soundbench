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

    Copyright 2014  Amaya S.
*/

#include "cpumeter.h"

CpuMeter::CpuMeter(QProgressBar* bare, QLabel* tex) {
    teimer = new QTimer;
    warning = false;
    alternate = false;
    progressmode = false;

    affectedbar = bare;
    affectedtex = tex;

    metup = new MeterUpdater(affectedbar);
    connect(teimer,SIGNAL(timeout()),metup,SLOT(update()));
    connect(teimer,SIGNAL(timeout()),SLOT(restyleCPUMeter()));
}
CpuMeter::~CpuMeter() {
    teimer->stop();
    delete metup;
    delete teimer;
}

void CpuMeter::startMeter() {
    metup->reset();
    affectedbar->setStyleSheet("");
    affectedtex->setText("CPU Time");
    progressmode = false;
    teimer->start(10);
}

void CpuMeter::pauseMeter() {
    teimer->stop();
    affectedtex->setText("Paused");
    affectedbar->setValue(0);
}

void CpuMeter::setProgress(int val) {
    if (!progressmode) {
        teimer->stop();
        affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                   "background-image: url(:/assets/cpumeter/cpumeter_progress.png);\n"
                                   "background-position: center left;\n"
                                   "background-repeat: 0;\n"
                                   "}");
        affectedtex->setText("Progress");
        progressmode = true;
    }
    affectedbar->setValue(val);
    affectedbar->repaint();
}

void CpuMeter::restyleCPUMeter() {
    if((affectedbar->value() >= 750) && !warning && !alternate) {
        affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                   "background-image: url(:/assets/cpumeter/cpumeter_warn.png);\n"
                                   "background-position: center left;\n"
                                   "background-repeat: 0;\n"
                                   "}");
        warning = true;
        alternate = false;
    }
    else if ((affectedbar->value() <= 700) && warning) {
        affectedbar->setStyleSheet("");
        warning = false;
        alternate = false;
    }
    else if ((affectedbar->value() >= 900) && warning && !alternate) {
        affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                   "background-image: url(:/assets/cpumeter/cpumeter_red.png);\n"
                                   "background-position: center left;\n"
                                   "background-repeat: 0;\n"
                                   "}");alternate = true;
    }
    else if ((affectedbar->value() <= 850) && warning && alternate) {
        affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                   "background-image: url(:/assets/cpumeter/cpumeter_warn.png);\n"
                                   "background-position: center left;\n"
                                   "background-repeat: 0;\n"
                                   "}");
        alternate = false;
    }
}
