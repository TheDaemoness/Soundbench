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

#ifndef SAMPLER_H
#define SAMPLER_H

#include "soundbases.h"
#include "util/basicticker.h"

namespace sb {

    class SamplerGen : public GenBase {
    public:
        SamplerGen(uint8_t costco_shopper);
        ~SamplerGen();
        bool finished(size_t pos) {
            return (envelope[pos].getLevel() == SbSampleZero);
        }

        void ctrl(ModuleParams arg, ParameterValue val);
        void noteOn(int halfsteps, SbSample amp, size_t pos);
        void noteOff(size_t pos);
        void reset();
        void setPolymorphism(size_t poly);
        void tick(float *sample, size_t chans);
        void updateSamplingRate();
    private:
        std::vector<Trapezoid> envelope;
        std::vector<SbSample> sampletable;
        std::vector<IntTicker> tickers;
        size_t sustainmarker, releasemarker;
    };

}

#endif // SAMPLER_H
