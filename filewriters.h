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

#ifndef FILEWRITERS_H
#define FILEWRITERS_H

#include "common.h"
#include <fstream>

namespace sb {
    template <typename Stream, typename Arrai>
    void bitArrayWrite (Stream& river, Arrai str) {
        for(size_t i = 0; i < str.length() - 1; ++i)
            river.put(str[i] | Bit1);
        river.put(str.back());
    }

    template <typename Stream, typename Inte, size_t limit = 8>
    void vliWrite (Stream& river, Inte inp) {
        static_assert(limit <= 8, "VLIs of a size larger than 8 are not supported.\n");

        uint64_t towrite = static_cast<uint64_t>(inp);
        char* ptr = reinterpret_cast<char*>(&towrite);
        bool write = false;
        char wbuffer;
        for(size_t i = 0; i < sizeof(towrite) && i < limit; ++i, towrite <<= 7) {
            if (ptr[0] == 0 && !write) {
                towrite <<= 1; //Makes a full shift on iteration.
                continue;
            }
            write = true;
            wbuffer = ptr[0];
            wbuffer >>= 1;
            if (i < sizeof(towrite)-1 && i < limit-1)
                wbuffer |= Bit1;
            river.put(wbuffer);
        }
    }
}

#endif // FILEWRITERS_H
