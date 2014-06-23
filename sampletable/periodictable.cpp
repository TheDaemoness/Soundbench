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

#include "periodictable.h"

namespace sb {
    PeriodicSampleTable::PeriodicSampleTable(WaveBase* wav, size_t mendelev, bool autodelete) {
        if (wav == nullptr) {
            samples.resize(global_srate,SbSample_ZERO);
            return;
        }
        size_t wavlen = wav->getWaveLen();
        samples.reserve(wavlen);
        for (size_t i = 0; i < wavlen; ++i)
            samples.push_back(wav->tick());
        if (autodelete)
            delete wav;
        setWaveCount(mendelev);
    }

    void PeriodicSampleTable::setWave(float freq, SbSample amp, size_t pos){
        if (pos >= iters.size())
            return;
        iters[pos].setSpeed(freq);
        amps[pos] = amp;
    }

    SbSample PeriodicSampleTable::tick(size_t pos) {
        FloatTicker& it = iters[pos];
        float num = it.getValue();
        it.tick();
        while (it.getValue() >= samples.size())
            it.setPos(it.getValue()-samples.size());
        return samples[static_cast<size_t>(num)]*amps[pos];
    }
}
