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

#ifndef VLIPARSER_H
#define VLIPARSER_H

#include "common.h"
#include <fstream>

namespace sb {
    template <size_t len, typename Stream>
    uint64_t vliParse(Stream& river) {
        uint64_t field = 0;
        uint8_t byte;
        for(uint8_t i = 0; i < len; ++i) {
            field <<= 7; //Assure space for the next seven bits (1 byte - the next-byte-exists bit).
            byte = river.get();
            field |= byte & ~Bit1; //Everything excpet the next-byte-exists bit.
            if (!(byte & Bit1)) //The next-byte-exists bit isn't set? Stop looping!
                break;
        }
        return field;
    }

    template <typename stream>
    std::string byteArrayParse(stream& river) {
        std::string outstr;
        while (true) {
            char charin = river.get();
            outstr.push_back(charin & ~Bit1);
            if(!(charin & Bit1))
                break;
        }
        return outstr;
    }
}

#endif // VLIPARSER_H
