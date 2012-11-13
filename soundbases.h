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

#ifndef MODULES_H
#define MODULES_H

#include "trapezoid.h"
#include "waves/basicwaves.h"

#include <cstring>

namespace sb {

    class SoundBase {
    public:
        SoundBase()  {}
        virtual ~SoundBase()  {}

        virtual void tick(float* sample, size_t chans) = 0;
        virtual void ctrl(ModuleParams arg, ParameterValue val) = 0; //See the documentation for notes about this function.
        virtual void reset() = 0;
        virtual void updateSamplingRate() = 0;
    };


    class GenBase : public SoundBase {
    public:
        virtual void noteOn(int halfsteps, SbSample amp, size_t pos) = 0;
        virtual void noteOff(size_t pos) = 0;
        virtual void setPolymorphism(size_t) = 0;
        virtual bool finished(size_t pos) = 0;
    };

    class FxBase : public SoundBase {
    public:
        FxBase() {
            feedback = 0;
            for(size_t i = 0; i < OutChannels; ++i)
                prevframe[i] = SbSampleZero;
        }
        virtual ~FxBase() {}

        void tick(float* sample, size_t chans) {
            for(size_t i = 0; i < chans; ++i)
                sample[i] = prevframe[i]*feedback/SbSampleMax + sample[i]*(SbSampleMax-feedback)/SbSampleMax;
            internal_tick(sample,chans);
            for(size_t i = 0; i < chans; ++i)
                prevframe[i] = sample[i];
        }

        virtual void ctrl(ModuleParams arg, ParameterValue val) = 0; //See the documentation for notes about this function.
        virtual void reset() = 0;
        virtual void updateSamplingRate() = 0;
    protected:
        SbSample feedback;
        virtual void internal_tick(float* sample, size_t chans) = 0;
    private:
        SbSample prevframe[OutChannels];
    };

}

#endif // MODULES_H
