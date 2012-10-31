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
        case FxBiquadType:
            break;
        case FxBiquadFreq:
            break;
        case FxBiquadGain:
            break;
        case FxBiquadQ:
            break;
        case FxBiquadBW:
            break;
        case FxBiquadS:
            break;
        default:
            break;
        }
    }
}
