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

void SoundbenchMain::genSetts(size_t i) {
    if (blu->gener[i] == sb::GenerBasic) {
        gsd.basic = new BasicGenerSettings(i,blu);
        gsd.basic->exec();
        delete gsd.basic;
    }
    else
        std::cerr << "A request for an unimplemented generator's settings was made. Ignoring...\n";
}

SoundbenchMain::~SoundbenchMain() {
    writePresetRecord();

    delete arch;
    delete plai;
    delete em;
    if (syn != nullptr)
        delete syn;
    delete blu;

    delete met;

    delete sett_sigmap;
    delete type_sigmap;
    delete rate_sigmap;

    delete ui;
}

void SoundbenchMain::updateToBlueprint() {
    ui->gener1TypeBox->setCurrentIndex(static_cast<int>(blu->gener[0]));
    ui->gener2TypeBox->setCurrentIndex(static_cast<int>(blu->gener[1]));
    ui->gener3TypeBox->setCurrentIndex(static_cast<int>(blu->gener[2]));
    ui->gener4TypeBox->setCurrentIndex(static_cast<int>(blu->gener[3]));

    ui->gen1SettButton->setEnabled(blu->gener[0]!=sb::NoGener);
    ui->gen2SettButton->setEnabled(blu->gener[1]!=sb::NoGener);
    ui->gen3SettButton->setEnabled(blu->gener[2]!=sb::NoGener);
    ui->gen4SettButton->setEnabled(blu->gener[3]!=sb::NoGener);
}
