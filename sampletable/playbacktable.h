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

#ifndef ASRTABLE_H
#define ASRTABLE_H

#include "util/intticker.h"
#include "sampletable/sampletablebase.h"

namespace sb {
    class PlaybackSampleTable : public SampleTableBase {
    public:
        PlaybackSampleTable();

        SbSample tick(size_t pos = 0);
        void setIterator(float freq, SbSample amp, size_t pos) {

        }

        void setIteratorOffset(size_t loc, size_t offset = 0) {
            if(loc < tickles.size())
                tickles[loc].setPos(offset);
        }

        inline size_t getIteratorCount() {
            return this->tickles.size();
        }
        inline size_t getTableLength() {
            return samples.size();
        }

        void resetIterator(size_t pos) {
            tickles[pos].reset();
        }

        void setSample(size_t loc, SbSample val) {
            if(loc < samples.size())
                samples[loc] = val;
        }

        void setSampleTable(const std::vector<SbSample>& samps) {
            samples = samps;
        }

        SbSample getSample(size_t loc);

    private:
        std::vector<SbSample> samples;
        std::vector<IntTicker> tickles;
        std::vector<SbSample> amps;
    };
}
#endif // ASRTABLE_H
