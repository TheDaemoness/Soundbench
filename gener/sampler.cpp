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

#include "sampler.h"

namespace sb {
    SamplerGen::SamplerGen(uint8_t costco_shopper) {
        envelope.resize(costco_shopper);
        sampletable.resize(1,SbSampleZero);
        this->releasemarker = 0;
        this->sustainmarker = 0;
    }

    SamplerGen::~SamplerGen() {
    }

    void SamplerGen::ctrl(ModuleParams arg, ParameterValue val) {
    }

    void SamplerGen::noteOn(int halfsteps, SbSample amp, size_t pos) {
    }

    void SamplerGen::noteOff(size_t pos) {
    }

    void SamplerGen::reset() {
    }

    void SamplerGen::setPolymorphism(size_t poly) {
    }

    void SamplerGen::tick(float *sample, size_t chans) {
    }

    void SamplerGen::updateSamplingRate() {
    }
}
