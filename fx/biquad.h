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
            float f0, dbgain, q, bw, s;
            float alpha, w0, a; //Intermediate variables.
            float a0, a1, a2, b0, b1, b2; //Coefficients.
            SbSample x[OutChannels][2], y[OutChannels][2];
    public:
        BiquadFx();
        void ctrl(ModuleParams arg, ParameterValue val);
        void updateSamplingRate();
        void reset();
    protected:
        void internal_tick(float *sample, size_t chans);
    private:
        void recalc();
        BiquadTypes typpe;
    };

}
#endif // BIQUAD_H
