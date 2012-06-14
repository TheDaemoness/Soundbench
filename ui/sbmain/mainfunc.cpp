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
        stopAndReset();
        gsd.basic = new BasicGenerSettings(i,blu);
        gsd.basic->exec();
        delete gsd.basic;
    }
    else
        std::cerr << "A request for an unimplemented generator's settings was made. Ignoring...\n";
}

SoundbenchMain::~SoundbenchMain() {

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
