
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

#include "waves/basicwaves.h"
#include <cmath>

namespace sb {

    Sine::Sine(int sample_r) : BasicWave(sample_r) {}
    Sawtooth::Sawtooth(int sample_r) : BasicWave(sample_r) {}
    Square::Square(int sample_r) : BasicWave(sample_r) {}
    Triangle::Triangle(int sample_r) : BasicWave(sample_r) {}
    Oval::Oval(int sample_r) : BasicWave(sample_r) {}
    Peak::Peak(int sample_r) : BasicWave(sample_r) {}

    SbSample BasicWave::tick() {
        ++samp_pos;
        float rad = (static_cast<float>(samp_pos)/static_cast<float>(getWaveLen())*2.0F*sb::Pi);
        return getRaw(rad);
    }

    SbSample Sine::getRaw(float x) {
        return std::sin(x);
    }

    SbSample Sawtooth::getRaw(float rads) {
        return std::fmod(rads+Pi,2.0f*Pi)/Pi - 1.0;
    }

    SbSample Square::getRaw(float rads) {
        rads = std::fmod(rads,2.0f*Pi);
        return (rads < Pi?1.0:-1.0);
    }

    SbSample Triangle::getRaw(float rads) {
        rads = std::fmod(rads,2.0f*Pi);
        if (rads < 2.0*Pi/4.0)
            return rads*4.0/(2.0*Pi);
        else if ((rads >= 2.0*Pi/4.0) && (rads < 6.0*Pi/4.0))
            return 1.0 - (rads-2.0*Pi/4.0)*4.0/(2.0*Pi);
        else
            return (rads-6.0*Pi/4.0)*4.0/(2.0*Pi)-1.0;
    }

    SbSample Oval::getRaw(float rads) {
        rads = std::fmod(rads,2.0f*Pi);
        bool invert = rads > Pi;
        rads = std::fmod(rads,Pi);
        return (invert?-2.0:2.0)/Pi*std::sqrt(Pi*rads-rads*rads);
    }

    SbSample Peak::getRaw(float rads) {
        rads = std::fmod(rads,2.0f*Pi);
        bool invert = rads >= Pi;
        rads = std::fmod(rads,Pi);
        if(rads >= Pi/2.0)
            return (invert?SbSampleMin:SbSampleMax)*(SbSampleMax - 2.0/Pi*std::sqrt(Pi*rads-rads*rads));
        else
            return (invert?SbSampleMin:SbSampleMax)*(SbSampleMin + 2.0/Pi*std::sqrt(Pi*rads-rads*rads));
    }
}
