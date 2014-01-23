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

#ifndef CPUMETER_H
#define CPUMETER_H

#include <QProgressBar>
#include <QLabel>
#include <QTimer>

#include "meterupdater.h"

class CpuMeter : public QObject {
    Q_OBJECT
public:
    explicit CpuMeter(QProgressBar* bare, QLabel* tex);
    ~CpuMeter();

public slots:
    void startMeter();
    void pauseMeter();

    void setProgress(int val);

private slots:
    void restyleCPUMeter();

private:
    QProgressBar* affectedbar;
    QLabel* affectedtex;

    QTimer* teimer;
    bool warning, alternate, progressmode;
    MeterUpdater* metup;
};

#endif // CPUMETER_H
