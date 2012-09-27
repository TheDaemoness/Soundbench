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

#include <QApplication>
#include <QCleanlooksStyle>

#include "ui/sbmain/soundbenchmain.h"
#include "printhelp.h"

size_t sb::SampleRate;
sb::Synth* sb::midi::nodes::MIDIEventNode::synref;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyle(new QCleanlooksStyle);

    //Parse some arguments.
    std::string arg;
    if (argc == 2) {
        arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printhelp();
            return 0;
        }
    }
    std::cout << "Soundbench " << SBVERSION << '\n';
    std::cout << "Version Series: " << SBVERSION_MAJOR << '.' << SBVERSION_MINOR << ".x\n";
    if (arg == "--version" || arg == "-v")
        return 0;

    SoundbenchMain w;
    w.delayedConstructor();

    std::cerr << "Soundbench loaded.\n";

    w.show();
    return a.exec();
}

