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

#ifndef CHAINS_H
#define CHAINS_H

#include "soundbases.h"
#include "sampletable.h"

namespace sb {

    class BasicGen : public GenBase {
    private:
        PeriodicSampleTable* ocean;
        SimpleWaveTypes curr_wav;
        SbSample gen_amp;
        int16_t notebias;
        uint8_t currpoly;
        size_t offset;
    public:
        explicit BasicGen(size_t cracker = DefaultPolyphony);
        ~BasicGen() {
            delete ocean;
        }
        bool finished(size_t pos) {
            return (envelope[pos].getLevel() == SbSampleZero);
        }

        void ctrl(ModuleParams arg, ParameterValue val);
        void noteOn(int halfsteps, SbSample amp, size_t pos);
        void noteOff(size_t pos);
        void reset();
        void setPolymorphism(size_t poly);
        void tick(float *sample, size_t chans, bool left = true);
        void updateSamplingRate();
    };
}

#endif // CHAINS_H
