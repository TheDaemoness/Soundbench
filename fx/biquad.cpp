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

#include "biquad.h"

namespace sb {
    BiquadFx::BiquadFx() {
        typpe = BiquadLowpass;
        f0 = global_srate/4;
        dbgain = -3;
        q = 1;
        bw = 2;
        s = 1;

        reset();
        recalc();
    }

    void BiquadFx::updateSamplingRate() {
        recalcVars();
    }

    void BiquadFx::tick(float *sample, size_t chans, bool left) {

    }

    void BiquadFx::reset() {
        prevl[0] = SbSampleZero;
        prevr[0] = SbSampleZero;
        prevl[1] = SbSampleZero;
        prevr[1] = SbSampleZero;
    }

    void BiquadFx::recalc() {
        a = std::pow(10.0,(dbgain/40.0));
        w0 = 2.0*Pi*f0/global_srate;

        //Calculate the intermediate variables.
        switch (typpe) {
        case BiquadBandpassQ:
        case BiquadBandpassZero:
        case BiquadNotch:
        case BiquadPeakingEQ:
            alpha = std::sin(w0)*std::sinh(std::log(2.0)/2.0 * bw * w0/std::sin(w0));
            break;
        case BiquadHighShelf:
        case BiquadLowShelf:
            alpha = sin(w0)/2.0 * std::sqrt((a + 1.0/a)*(1.0/s - 1.0) + 2.0);
            break;
        case BiquadLowpass:
        case BiquadHighpass:
        case BiquadAllpass:
            alpha = sin(w0)/(2.0*q);
            break;
        }

        //Calculate the actual coefficients.
        switch (typpe) {
        case BiquadLowpass:
        case BiquadHighpass:
        case BiquadBandpassZero:
        case BiquadBandpassQ:
        case BiquadNotch:
        case BiquadAllpass:
        case BiquadPeakingEQ:
        case BiquadLowShelf:
        case BiquadHighShelf:
        }
}
