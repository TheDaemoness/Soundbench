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

#ifndef UTILITIES_H
#define UTILITIES_H

/*
    This file has been split off from and included in common.h.

    Should the MIDI file parser prove to be reliable, it may be released as a separate subproject.

    Utilities that *both* Soundbench and the midifile set of files might need will be put here.
    Anything that only Soundbench will need will be found in common.h
    Anything that only the subproject will need will be found in midifile.h

    If you are a project developer, DO NOT ADD ANYTHING TO THIS FILE WITHOUT CONSULTING THE PROJECT HEAD!
*/

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <vector>

namespace sb {

    enum simple_bitmasks { //Notice: These values are for use on single bytes and assume big endian format.
        Bit8 = 1,
        Bit7 = 2,
        Bit6 = 4,
        Bit5 = 8,
        Bit4 = 16,
        Bit3 = 32,
        Bit2 = 64,
        Bit1 = 128
    };

    enum simple_bytemasks { //Byte1 is the least significant for the purposes of supporting many different integer sizes, up to uint32_t.
        Byte1 = 0xFF,
        Byte2 = 0xFF00,
        Byte3 = 0xFF0000,
        Byte4 = 0xFF000000
    };

    template <typename outType, typename inType>
    outType lexical_cast(inType in) {
        std::stringstream strm;
        strm.str(std::string());
        strm << in;
        outType out;
        strm >> out;
        return out;
    }

    template <typename inType> //We know that function template specializations are evil.
    std::string lexical_cast(inType in) {
        std::stringstream strm;
        strm.str(std::string());
        strm << in;
        return strm.str();
    }
}

#endif // UTILITIES_H
