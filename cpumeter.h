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

#ifndef CPUMETER_H
#define CPUMETER_H

#include <QProgressBar>
#include <QLabel>
#include <QTimer>

#include "meterupdater.h"

class CpuMeter : public QObject {
    Q_OBJECT
public:
    explicit CpuMeter(QProgressBar* bare, QLabel* tex) {
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
    ~CpuMeter() {
        teimer->stop();
        delete metup;
        delete teimer;
    }

public slots:
    void startMeter() {
        metup->reset();
        if (progressmode) {
            affectedbar->setStyleSheet("");
            affectedtex->setText("CPU Time");
            progressmode = false;
        }
        teimer->start(10);
    }

    void setProgress(int val) {
        if (!progressmode) {
            teimer->stop();
            affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                       "background-image: url(:/meters/cpumeter_progress.png);\n"
                                       "background-position: center left;\n"
                                       "background-repeat: 0;\n"
                                       "}");
            affectedtex->setText("Progress");
            progressmode = true;
        }
        affectedbar->setValue(val);
        affectedbar->repaint();
    }

private slots:
    void restyleCPUMeter() {
        if(((metup->artificial_limit() && affectedbar->value() >= 650) || affectedbar->value() >= 750) && !warning && !alternate) {
            affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                       "background-image: url(:/meters/cpumeter_warn.png);\n"
                                       "background-position: center left;\n"
                                       "background-repeat: 0;\n"
                                       "}");
            warning = true;
            alternate = false;
        }
        else if (((metup->artificial_limit() && affectedbar->value() <= 600) || affectedbar->value() <= 700) && warning) {
            affectedbar->setStyleSheet("");
            warning = false;
            alternate = false;
        }
        else if (((metup->artificial_limit() && affectedbar->value() >= 800) || affectedbar->value() >= 900) && warning && !alternate) {
            affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                       "background-image: url(:/meters/cpumeter_red.png);\n"
                                       "background-position: center left;\n"
                                       "background-repeat: 0;\n"
                                       "}");alternate = true;
        }
        else if (((metup->artificial_limit() && affectedbar->value() <= 750) || affectedbar->value() <= 850) && warning && alternate) {
            affectedbar->setStyleSheet("QProgressBar::chunk {\n"
                                       "background-image: url(:/meters/cpumeter_warn.png);\n"
                                       "background-position: center left;\n"
                                       "background-repeat: 0;\n"
                                       "}");
            alternate = false;
        }
    }

private:
    QProgressBar* affectedbar;
    QLabel* affectedtex;

    QTimer* teimer;
    bool warning, alternate, progressmode;
    MeterUpdater* metup;
};

#endif // CPUMETER_H
