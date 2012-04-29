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

namespace sb {

    class BasicGen : public genBase {
    private:
        typedef std::pair<bool,waveBase*> BoolWavePair;

        std::vector<BoolWavePair> ocean;
        SimpleWaveTypes curr_wav;
        sbSample gen_amp;
        int notebias;
        uint8_t notes;
    public:
        explicit BasicGen(size_t srate, size_t cracker = default_poly);
        ~BasicGen() {
            for(auto i : ocean)
                delete i.second;
        }

        void ctrl(moduleParam arg, ParameterValue val);
        void noteOn(int halfsteps, sbSample amp, size_t pos);
        void noteOff(size_t pos);
        void reset();
        void setPolymorphism(size_t poly);
        void tick(float *sample, size_t chans);
    };
}

#endif // CHAINS_H
