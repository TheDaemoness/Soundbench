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

#ifndef ARCHITECT_H
#define ARCHITECT_H

#include "gener/basic.h"
#include "gener/sampler.h"

#include "fx/biquad.h"

#include "blueprint.h"
#include "synth/synth.h"

#include <fstream>

namespace sb {

    struct PresetMeta {
        std::string path;
        std::string name;
        std::string arti;
        std::string desc;
        std::string tags;
    };

    class Architect {
    public:
        Architect();
        //Planning functions.
        PresetMeta getMetadata(std::string, std::string presetdir, std::ifstream* in = nullptr);
        PresetMeta loadPreset(std::string path, std::string presetdir, Blueprint* blu);
        PresetMeta savePreset(std::string path, std::string presetdir, Blueprint* blu, std::string name = "", std::string arti = "", std::string desc = "");

        void planAllDefaults(Blueprint*);

        void planDefaultBasicGen(Blueprint*,size_t chan_index = 0);
        void planDefaultSamplerGen(Blueprint*,size_t chan_index = 0);

        void planDefaultBiquadFx(Blueprint*,size_t chan_index = 0, size_t fx_num = 0);

        //Building functions
        void stripSynth(Synth*);
        void buildSynth(Synth*,Blueprint* blu = nullptr);

    private:
    };
}

#endif // ARCHITECT_H
