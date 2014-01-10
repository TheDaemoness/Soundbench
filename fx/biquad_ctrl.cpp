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
    void BiquadFx::ctrl(ModuleParams arg, ParameterValue val) {
        switch(arg) {
        case EffectFeedback:
            feedback = val.pod.decim;
            break;
        case FxBiquadType:
            typpe = static_cast<BiquadTypes>(val.pod.value);
            recalc();
            break;
        case FxBiquadFreq:
            if(typpe == BiquadManual)
                break;
            f0 = val.pod.sample;
            recalc();
            break;
        case FxBiquadGain:
            if(typpe == BiquadManual)
                break;
            dbgain = val.pod.decim;
            recalc();
            break;
        case FxBiquadQ:
            if(typpe == BiquadManual)
                break;
            q = val.pod.decim;
            recalc();
            break;
        case FxBiquadBW:
            if(typpe == BiquadManual)
                break;
            bw = val.pod.decim;
            recalc();
            break;
        case FxBiquadS:
            if(typpe == BiquadManual)
                break;
            s = val.pod.decim;
            recalc();
            break;
        case FxBiquadA0:
            if(typpe != BiquadManual)
                break;
            a0 = val.pod.decim;
            break;
        case FxBiquadA1:
            if(typpe != BiquadManual)
                break;
            a1 = val.pod.decim;
            break;
        case FxBiquadA2:
            if(typpe != BiquadManual)
                break;
            a2 = val.pod.decim;
        case FxBiquadB0:
            if(typpe != BiquadManual)
                break;
            b0 = val.pod.decim;
            break;
        case FxBiquadB1:
            if(typpe != BiquadManual)
                break;
            b1 = val.pod.decim;
            break;
        case FxBiquadB2:
            if(typpe != BiquadManual)
                break;
            b2 = val.pod.decim;
            break;
        default:
            break;
        }
    }
}
