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

    void BiquadFx::Lowpass::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::Lowpass::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::Highpass::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::Highpass::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::BandpassQ::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::BandpassQ::computeCoefficients(float ef0, float dbgain, float crit) {}


    void BiquadFx::BandpassZero::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::BandpassZero::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::Notch::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::Notch::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::Allpass::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::Allpass::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::PeakingEQ::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::PeakingEQ::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::LowShelf::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::LowShelf::computeCoefficients(float ef0, float dbgain, float crit) {}

    void BiquadFx::HighShelf::tick(float* sample, size_t chans, bool left) {}
    void BiquadFx::HighShelf::computeCoefficients(float ef0, float dbgain, float crit) {}
}
