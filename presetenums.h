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
        GenerSampler = 1,
        GenerBasic = 2,
        GenerAdditive = 3,
        GenerModal = 4,
        GenerWaveguide = 5
    };
    enum FxType {
        NoFx = 0
    };

    enum ModuleParams {
        GenerAttackTime = 0,
        GenerReleaseTime = 1,
        GenBasicWave = 2,
        GenBasicAmp = 3,
        GenBasicPhase = 4,
        GenBasicNoteBias = 5
    };

    enum SimpleWaveTypes {
        SineWave = 1,
        TriangleWave = 2,
        SquareWave = 3,
        SawtoothWave = 4,
        OvalWave = 5
    };
}

#endif // PRESETENUMS_H
