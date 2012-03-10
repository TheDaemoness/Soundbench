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

#include "application.h"

SoundbenchApp::SoundbenchApp(int argc, char** argv) : QApplication(argc,argv) {
    sb = NULL;
}

void SoundbenchApp::newSoundbench(int flags) {
    try {
        if (flags == 0) {
            sb = new SoundbenchMain;
            sb->delayedConstructor();
        }
    }
    catch (sbError& e) {
        std::cerr << "Fault in " << e.module() << " - " << e.message() << ".\n";
    }
}

SoundbenchApp::~SoundbenchApp() {
    delete sb;
}
