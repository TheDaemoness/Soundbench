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

#ifndef BASICWAVES_H
#define BASICWAVES_H

#include "waves/wavebase.h"

namespace sb {

    class BasicWave : public WaveBase {
    public:
        explicit BasicWave (int sample_r) : WaveBase(sample_r) {}

        virtual SbSample getRaw(float radians) = 0;
        SbSample tick();
        size_t getWaveLen() {
            return this->sample_rate;
        }
    };

    class Sine : public BasicWave {
    public:
        explicit Sine (int sample_r);
        SbSample getRaw(float radians);

    };

    class Sawtooth : public BasicWave {
    public:
        explicit Sawtooth (int sample_r);
        SbSample getRaw(float radians);
    };

    class Square : public BasicWave {
    public:
        explicit Square (int sample_r);
        SbSample getRaw(float radians);
    };

    class Triangle : public BasicWave {
    public:
        explicit Triangle (int sample_r);
        SbSample getRaw(float radians);
    };

    class Oval : public BasicWave {
    public:
        explicit Oval (int sample_r);
        SbSample getRaw(float radians);
    };

    class Peak : public BasicWave {
    public:
        explicit Peak (int sample_r);
        SbSample getRaw(float radians);
    };

}

#endif // BASICWAVES_H
