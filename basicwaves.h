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

#include "common.h"

namespace sb {

    class waveBase
    {
    public:
        virtual ~waveBase() {}

        inline void setFrequency(double new_freq) {
            frequency = new_freq;
        }
        inline void setAmplitude(sbSample new_amp) {
            amplitude = new_amp;
        }
        inline sbSample getAmplitude() {
            return amplitude;
        }
        inline bool isActive() {
            return amplitude != sbSampleZero;
        }
        inline void reset() {
            pos = -1;
        }
        inline virtual sbSample tick() {
            ++pos;
            return getRaw(frequency/sample_rate * pos);
        }

        virtual sbSample getRaw(float radians) = 0;

    protected:
        size_t pos; //WARNING: Pos is in samples, and it must start at -1!
        int sample_rate;
        double frequency;
        sbSample amplitude;
    };

    class Sine : public waveBase {
    public:
        explicit Sine (int sample_r);
        sbSample getRaw(float radians);

    };

    class Sawtooth : public waveBase {
    public:
        explicit Sawtooth (int sample_r);
        sbSample getRaw(float radians);
    };

    class Square : public waveBase {
    public:
        explicit Square (int sample_r);
        sbSample getRaw(float radians);
    };

    class Triangle : public waveBase {
    public:
        explicit Triangle (int sample_r);
        sbSample getRaw(float radians);
    };

}

#endif // BASICWAVES_H
