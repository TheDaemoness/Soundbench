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

#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include "waves/basicwaves.h"
#include "ticker.h"

namespace sb {

    class PeriodicSampleTable {
    public:
        explicit PeriodicSampleTable(WaveBase* wav = nullptr, size_t wavecount = DefaultPolyphony, bool autodelete = true);
        void setWaveCount (size_t its) {
            iters.resize(its);
            amps.resize(its);
        }
        void setWave(float freq, SbSample amp, size_t pos);
        void setOffsets(size_t offset) {
            for(Ticker& pronoun : iters)
                pronoun.setPos(static_cast<float>(offset));
        }

        void reset() {
            for(Ticker& ti : iters) {
                ti.setPos(0);
                ti.setSpeed(0);
            }
        }
        SbSample tick(size_t pos = 0);

    protected:
        std::vector<SbSample> samples;
        std::vector<Ticker> iters;
        std::vector<SbSample> amps;
    };

}

#endif //SAMPLETABLE_H
