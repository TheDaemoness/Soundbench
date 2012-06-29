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

#include "soundbenchmain.h"


void SoundbenchMain::setGenType(int which) {
    switch (which) {
    case 0:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener1TypeBox->currentIndex());
        if (blu->gener[which] != sb::NoGener)
            ui->gen1SettButton->setEnabled(true);
        else
            ui->gen1SettButton->setDisabled(true);
        break;
    case 1:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener2TypeBox->currentIndex());
        if (blu->gener[which] != sb::NoGener)
            ui->gen2SettButton->setEnabled(true);
        else
            ui->gen2SettButton->setDisabled(true);
        break;
    case 2:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener3TypeBox->currentIndex());
        if (blu->gener[which] != sb::NoGener)
            ui->gen3SettButton->setEnabled(true);
        else
            ui->gen3SettButton->setDisabled(true);
        break;
    case 3:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener4TypeBox->currentIndex());
        if (blu->gener[which] != sb::NoGener)
            ui->gen4SettButton->setEnabled(true);
        else
            ui->gen4SettButton->setDisabled(true);
        break;
    default:
        std::cerr << "A generator type-changing function was called by some random widget.\n";
        return;
    }
    stopAndReset();
    arch->buildSynth(syn,blu);
    em->start();
}

void SoundbenchMain::setGenSett(int which) {
    stopAndReset();
    met->pauseMeter();
    genSetts(which);
    arch->buildSynth(syn,blu);
    syn->reset();
    met->startMeter();
    em->start();
}
