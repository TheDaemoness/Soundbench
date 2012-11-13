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

#ifndef PRESETENUMS_H
#define PRESETENUMS_H

namespace sb {
    enum GenerType {
        NoGener = 0,
        GenerBasic = 1,
        GenerAdditive = 2,
        GenerSampler = 3,
        GenerModal = 4,
        GenerWaveguide = 5
    };
    enum FxType {
        NoFx = 0,
        FxBiquad = 1
    };

    enum ModuleParams {
        GenerAttackTime = 0,
        GenerReleaseTime = 1,
        GenBasicWave = 2,
        GenBasicAmp = 3,
        GenBasicPhase = 4,
        GenBasicNoteBias = 5,
        EffectFeedback = 6,
        FxBiquadType = 7,
        FxBiquadFreq = 8,
        FxBiquadGain = 9,
        FxBiquadQ = 10,
        FxBiquadBW = 11,
        FxBiquadS = 12,
        FxBiquadA0 = 13,
        FxBiquadA1 = 14,
        FxBiquadA2 = 15,
        FxBiquadB0 = 16,
        FxBiquadB1 = 17,
        FxBiquadB2 = 18
    };

    enum BiquadTypes {
        BiquadManual = 0,
        BiquadLowpass = 1,
        BiquadHighpass = 2,
        BiquadBandpassQ = 3,
        BiquadBandpassZero = 4,
        BiquadNotch = 5,
        BiquadAllpass = 6,
        BiquadPeakingEQ = 7,
        BiquadLowShelf = 8,
        BiquadHighShelf = 9
    };

    enum SimpleWaveTypes {
        SineWave = 1,
        TriangleWave = 2,
        SquareWave = 3,
        SawtoothWave = 4,
        OvalWave = 5,
        PeakWave = 6
    };
}

#endif // PRESETENUMS_H
