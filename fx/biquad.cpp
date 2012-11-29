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
        f0 = global_srate/3;
        dbgain = -12;
        q = 3;
        bw = 2;
        s = 1;

        reset();
        recalc();
    }

    void BiquadFx::updateSamplingRate() {
        recalc();
    }

    void BiquadFx::internal_tick(float *sample, size_t chans) {
        for(size_t i = 0; i < chans; ++i) {
            x[i][1] = x[i][0];
            x[i][0] = sample[i];
            sample[i] = (b0/a0)*sample[i] + (b1/a0)*x[i][0] + (b2/a0)*x[i][1] - (a1/a0)*y[i][1] - (a2/a0)*y[i][2];
            y[i][1] = y[i][0];
            y[i][0] = sample[i];
        }
    }

    void BiquadFx::reset() {
        for(size_t i = 0; i < OutChannels; ++i) {
            for(size_t j = 0; j < 2; ++j) {
                x[i][j] = SbSampleZero;
                y[i][j] = SbSampleZero;
            }
        }
    }

    void BiquadFx::recalc() {
        if(typpe == BiquadManual)
            return;
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
        default:
            break;
        }

        //Calculate the actual coefficients.
        switch (typpe) {
        case BiquadLowpass:
            b0 =    (1.0 - std::cos(w0))/2.0;
            b1 =    1.0 - std::cos(w0);
            b2 =    (1.0 - std::cos(w0))/2.0;
            a0 =    1.0 + alpha;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha;
        case BiquadHighpass:
            b0 =    (1.0 + std::cos(w0))/2.0;
            b1 =    -(1.0 + std::cos(w0));
            b2 =    (1.0 + std::cos(w0))/2.0;
            a0 =    1.0 + alpha;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha;
        case BiquadBandpassZero:
            b0 =    alpha;
            b1 =    0.0;
            b2 =    -alpha;
            a0 =    1.0 + alpha;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha;
        case BiquadBandpassQ:
            b0 =    std::sin(w0)/2.0;
            b1 =    0.0;
            b2 =    -std::sin(w0)/2.0;
            a0 =    1.0 + alpha;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha;
        case BiquadNotch:
            b0 =    1.0;
            b1 =    -2.0*std::cos(w0);
            b2 =    1.0;
            a0 =    1.0 + alpha;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha;
        case BiquadAllpass:
            b0 =    1.0 - alpha;
            b1 =    -2.0*std::cos(w0);
            b2 =    1.0 + alpha;
            a0 =    1.0 + alpha;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha;
        case BiquadPeakingEQ:
            b0 =    1.0 + alpha*a;
            b1 =    -2.0*std::cos(w0);
            b2 =    1.0 - alpha*a;
            a0 =    1.0 + alpha/a;
            a1 =    -2.0*std::cos(w0);
            a2 =    1.0 - alpha/a;
        case BiquadLowShelf:
            b0 =    a*((a+1.0) - (a-1.0)*std::cos(w0) + 2.0*std::sqrt(a)*alpha);
            b1 =    2.0*a*((a-1.0) - (a+1.0)*std::cos(w0));
            b2 =    a*((a+1.0) - (a-1.0)*std::cos(w0) - 2.0*std::sqrt(a)*alpha);
            a0 =    (a+1.0) + (a-1.0)*std::cos(w0) + 2.0*std::sqrt(a)*alpha;
            a1 =    -2.0*((a-1.0) + (a+1.0)*std::cos(w0));
            a2 =    (a+1.0) + (a-1.0)*std::cos(w0) - 2.0*std::sqrt(a)*alpha;
        case BiquadHighShelf:
            b0 =    a*((a+1.0) + (a-1.0)*std::cos(w0) + 2.0*std::sqrt(a)*alpha);
            b1 =    -2.0*a*((a-1.0) + (a+1.0)*std::cos(w0));
            b2 =    a*((a+1.0) + (a-1.0)*std::cos(w0) - 2.0*std::sqrt(a)*alpha);
            a0 =    (a+1.0) - (a-1.0)*std::cos(w0) + 2.0*std::sqrt(a)*alpha;
            a1 =    2.0*((a-1.0) - (a+1.0)*std::cos(w0));
            a2 =    (a+1.0) - (a-1.0)*std::cos(w0) - 2.0*std::sqrt(a)*alpha;
            break;
        default:
            break;
        }
    }
}
