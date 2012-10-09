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

#include "sampletable.h"

namespace sb {
    PeriodicSampleTable::PeriodicSampleTable(WaveBase* wav, bool autodelete) {
        if (wav == nullptr) {
            samples.resize(global_srate,0.0);
            return;
        }
        samples.reserve(global_srate);
        float factor = Pi*2.0/global_srate;
        for (uint32_t i = 0; i < global_srate; ++i)
            samples.push_back(wav->getRaw(factor*i));
        if (autodelete)
            delete wav;
    }

    void PeriodicSampleTable::setWave(float freq, SbSample amp, size_t pos){
        if (pos >= iters.size())
            return;
        iters[pos].setSpeed(freq);
        amps[pos] = amp;
    }

    SbSample PeriodicSampleTable::tick(size_t pos) {
        float num = iters[pos].tick();
        if (num >= samples.size()) {
            num = std::fmod(num,samples.size());
            iters[pos].setPos(num);
        }
        return samples[static_cast<size_t>(num)]*amps[pos];
    }
}
