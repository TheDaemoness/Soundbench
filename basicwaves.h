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

#include "sbutilities.h"
#include "presetenums.h"

namespace sb {

    class waveBase
    {
    public:
        explicit waveBase(int sample_r) {
            samp_pos = -1;
            sample_rate = sample_r;
            rad_offset = 0.0f;
        }

        virtual ~waveBase() {}

        void setFrequency(double new_freq) {
            frequency = new_freq;
        }
        void setAmplitude(SbSample new_amp) {
            amplitude = new_amp;
        }
        void setOffset(float phase) {
            rad_offset = phase;
        }

        void reset() {
            samp_pos = -1;
        }
        virtual SbSample tick() {
            ++samp_pos;
            float rad = (frequency/sample_rate)*samp_pos;
            return getRaw(rad + rad_offset);
        }

        virtual SbSample getRaw(float radians) = 0;

    protected:
        size_t samp_pos; //WARNING: samp_pos is in samples, and it must start at -1!
        int sample_rate;
        double frequency;
        float rad_offset;
        SbSample amplitude;
    };

    class Sine : public waveBase {
    public:
        explicit Sine (int sample_r);
        SbSample getRaw(float radians);

    };

    class Sawtooth : public waveBase {
    public:
        explicit Sawtooth (int sample_r);
        SbSample getRaw(float radians);
    };

    class Square : public waveBase {
    public:
        explicit Square (int sample_r);
        SbSample getRaw(float radians);
    };

    class Triangle : public waveBase {
    public:
        explicit Triangle (int sample_r);
        SbSample getRaw(float radians);
    };

    class Oval : public waveBase {
    public:
        explicit Oval (int sample_r);
        SbSample getRaw(float radians);
    };

}

#endif // BASICWAVES_H
