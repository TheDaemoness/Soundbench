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

#ifndef PRINTHELP_H
#define PRINTHELP_H

#include <iostream>

inline void printhelp() {
	std::cout << "Quick Help\n";
	std::cout << "For more information, please see the wiki and the in-program tooltips.\n\n";
	std::cout << "Tab Contents\n"
				 "\tSelect: Choose a preset.\n"
				 "\tPlayer: Route input and output, record, load, and play MIDI tracks, and export sound files.\n"
				 "\tChannels: Change the settings for the audio generators and effects.\n"
				 "\tSettings: Change the sample rate and polyphony settings.\n";
}

#endif // PRINTHELP_H
