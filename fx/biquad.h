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
\
#ifndef BIQUAD_H
#define BIQUAD_H


#include "soundbases.h"

namespace sb {

    class BiquadFx : public FxBase {
    private:
        class BiquadFormula {
        public:
            virtual void tick(float* sample, size_t chans, bool left = true) = 0;
            virtual void computeCoefficients(float, float, float) = 0;
        protected:
            float alpha, w0, A; //Intermediate variables.
            float a0, a1, a2, b0, b1, b2; //Coefficients.
        };
        class Lowpass : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class Highpass : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class BandpassQ : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class BandpassZero : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class Notch : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class Allpass : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class PeakingEQ : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class LowShelf : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
        class HighShelf : public BiquadFormula {
            void tick(float* sample, size_t chans, bool left = true);
            void computeCoefficients(float, float, float);
        };
    public:
        BiquadFx();
    private:
        BiquadFormula* trans;
    };

}
#endif // BIQUAD_H
