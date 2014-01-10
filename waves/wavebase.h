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

#ifndef WAVES_H
#define WAVES_H

#include "sbutilities.h"
#include "presetenums.h"

namespace sb {

    class WaveBase {
    public:
        explicit WaveBase(int sample_r) {
            reset();
            sample_rate = sample_r;
        }

        virtual ~WaveBase() {}

        virtual size_t getWaveLen() = 0;
        virtual SbSample tick() = 0;

        void reset() {
            samp_pos = -1;
        }

    protected:
        size_t samp_pos; //WARNING: samp_pos is in samples, and it must start at -1!
        int sample_rate;
    };
}

#endif // WAVES_H

