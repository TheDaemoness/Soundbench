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

#include "waves/waves.h"

namespace sb {

    class Sine : public WaveBase {
    public:
        explicit Sine (int sample_r);
        SbSample getRaw(float radians);

    };

    class Sawtooth : public WaveBase {
    public:
        explicit Sawtooth (int sample_r);
        SbSample getRaw(float radians);
    };

    class Square : public WaveBase {
    public:
        explicit Square (int sample_r);
        SbSample getRaw(float radians);
    };

    class Triangle : public WaveBase {
    public:
        explicit Triangle (int sample_r);
        SbSample getRaw(float radians);
    };

    class Oval : public WaveBase {
    public:
        explicit Oval (int sample_r);
        SbSample getRaw(float radians);
    };

    class Peak : public WaveBase {
    public:
        explicit Peak (int sample_r);
        SbSample getRaw(float radians);
    };

}

#endif // BASICWAVES_H
