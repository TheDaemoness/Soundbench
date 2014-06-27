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

#ifndef COMMON_H
#define COMMON_H

/*
	This file is part of Soundbench.

	Should the MIDI file parser prove to be reliable, it may be released as a subproject.

	Utilities that *both* Soundbench and the midifile set of files might need will be put here.
	Anything that only Soundbench will need will be found in common.h
	Anything that only the subproject will need will be found in midifile.h

	If you are a project developer, DO NOT ADD ANYTHING TO THIS FILE WITHOUT CONSULTING THE PROJECT HEAD!
*/

#include <iostream>
#include <sstream>
#include <vector>

namespace sb {

	enum SimpleBitmasks { //Notice: These values are for use on single bytes.
		AllBits = 255,
		NoBits = 0,
		Bit8 = 1,
		Bit7 = 2,
		Bit6 = 4,
		Bit5 = 8,
		Bit4 = 16,
		Bit3 = 32,
		Bit2 = 64,
		Bit1 = 128
	};

	enum SimpleByteMasks {
		Nibble1 = Bit1 | Bit2 | Bit3 | Bit4,
		Nibble2 = Bit5 | Bit6 | Bit7 | Bit8
	};
}

#endif // COMMON_H
