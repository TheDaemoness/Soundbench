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
#include <QStyleFactory>
#include <QIcon>

#include "metadata.h"
#include "synth/synth.h"
#include "player/midichain.h"
#include "config/config.h"
#include "printhelp.h"

size_t sb::global_srate;
sb::Synth* sb::midi::nodes::MIDIEventNode::synref;

#ifndef NO_JACK
jack_client_t* sb::JackBase::cli = nullptr;
jack_status_t sb::JackBase::stat = static_cast<jack_status_t>(0);
#endif

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/sbicon.png"));
    a.setStyle(QStyleFactory::create("Fusion"));
	a.setApplicationName("Soundbench");
	a.setApplicationDisplayName("Soundbench");
	a.setApplicationVersion(QString::fromStdString(std::to_string(SBVERSION_MAJOR)+'.'+std::to_string(SBVERSION_MINOR)+'.'+std::to_string(SBVERSION_PATCH)));

    //Parse some arguments.
    std::string arg;
    if (argc == 2) {
        arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printhelp();
            return 0;
        }
    }
	std::cerr << "Soundbench " << SBVERSION << " (" << SBVERSION_CODENAME << ")\n";
	std::cerr << "Version Series: " << SBVERSION_MAJOR << '.' << SBVERSION_MINOR << ".x\n";
    if (arg == "--version" || arg == "-v")
        return 0;

	ConfigManager mgr;

	std::cerr << "Soundbench loaded.\n";

    return a.exec();
}

