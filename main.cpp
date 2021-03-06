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

#include <QApplication>
#include <QStyleFactory>
#include <QIcon>

#include "metadata.h"
#include "soundbench.h"
#include "config/config.h"
#include "printhelp.h"
#include "ui/sbuimain.h"

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

	sb::ConfigManager* cfg = new sb::ConfigManager;
	sb::Soundbench* sb = cfg->initSoundbench();
	SoundbenchUI sbui(sb, cfg);
	sbui.show();
	std::cerr << "Soundbench loaded.\n";
	int ret = a.exec();
	delete sb;
	delete cfg;
	return ret;
}

