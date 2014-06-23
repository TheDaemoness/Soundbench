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

#include "config.h"

#include <QDir>

#include <iostream>

ConfigManager::ConfigManager() {
	QDir sbdata(QDir::home());
	errored = false;
#if defined(SB_ENV_MACOS)
	errored |= !sbdata.cd("Library");
	errored |= !sbdata.cd("Application Support");
	if (!sbdata.exists("Soundbench")) {
		std::cerr << "Setting up directories...\n";
		errored |= !sbdata.mkdir("Soundbench");
	}
	errored |= !sbdata.cd("Soundbench");
#elif defined(SB_ENV_WNDOS)
	errored |= sbdata.cd("AppData");
	if (!sbdata.exists("Soundbench")) {
		std::cerr << "Setting up directories...\n";
		errored |= !sbdata.mkdir("Soundbench");
	}
	errored |= !sbdata.cd("Soundbench");
#else
	if (!sbdata.exists(".soundbench")) {
		std::cerr << "Setting up directories...\n";
		errored |= !sbdata.mkdir(".soundbench");
	}
	errored |= ! sbdata.cd(".soundbench");
#endif

	if(errored) {
		std::cerr << "Failed to access application data directory.\n";
		return;
	}
	datadir = sbdata.absolutePath().toStdString();
	if(!sbdata.exists("presets")) {
		std::cerr << "Setting up presets directory.\n";
		sbdata.mkdir("presets");
	}
}
