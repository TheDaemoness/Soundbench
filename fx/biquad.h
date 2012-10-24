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
        class BiquadFormula {};
        class Highpass : public BiquadFormula {};
        class Lowpass : public BiquadFormula {};
        class BandpassQ : public BiquadFormula {};
        class BandpassZero : public BiquadFormula {};
        class Notch : public BiquadFormula {};
        class Allpass : public BiquadFormula {};
        class PeakingEQ : public BiquadFormula {};
        class LowShelf : public BiquadFormula {};
        class HighShelf : public BiquadFormula {};
    public:
        BiquadFx();
    private:
        BiquadFormula* trans;
    };

}
#endif // BIQUAD_H
