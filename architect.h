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

#include "blueprint.h"
#include "gener/basic.h"
#include "synth.h"

#include <QDir>

namespace sb {

    struct PresetMeta {
        std::string path;
        std::string name;
        std::string arti;
        std::string desc;
        std::string tags;
    };

    class Architect
    {
    public:
        Architect();
        //Planning functions.
        PresetMeta getMetadata(std::string, std::string presetdir);
        void planAllDefaults(Blueprint*);
        void planDefaultBasicGen(Blueprint*,size_t chan_index = 0);

        //Building functions
        void stripSynth(Synth*);
        void buildSynth(Synth*,Blueprint* blu = nullptr);

    private:
    };
}

#endif // ARCHITECT_H
