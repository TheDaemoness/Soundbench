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

#include "architect.h"
#include "fileparsers.h"

namespace sb {

    PresetMeta Architect::loadPreset(std::string path, std::string presetdir, Blueprint* blu) {
        planAllDefaults(blu);
        std::ifstream in(presetdir+'/'+path,std::ios_base::binary);
        PresetMeta premet = getMetadata(path,presetdir,&in);

        if (premet.path.empty()) //Oops.
            return PresetMeta();

        uint8_t munch; //Bite, chew, masticate, etc.

        //Deal with the channel section.
        for (auto& e : blu->gener)
            e = static_cast<GenerType>(in.get());
        for (auto& chan : blu->eff) {
            for (auto& fx : chan) {
                munch = in.get();
                fx = static_cast<FxType>(munch & ~Bit1);
                if(munch & Bit1)
                    break;
            }
        }

        //Deal with the arguments section.
        if (in.eof()) {
            std::cerr << "The preset ended prematurely. Resetting the blueprint and aborting...\n";
            planAllDefaults(blu);
            return PresetMeta();
        }
        munch = in.get();
        while(munch != 0) {
            uint8_t channum = (munch & Nibble1) >> 4;
            if (channum == 0 || channum > 4) {
                std::cerr << "Invalid channel nibble (offset=" << in.tellg() << "). Resetting the blueprint and aborting...\n";
                planAllDefaults(blu);
                return PresetMeta();
            }
            if ((munch & Nibble2) > 4) {
                std::cerr << "Invalid element nibble (offset=" << in.tellg() << "). Resetting the blueprint and aborting...\n";
                planAllDefaults(blu);
                return PresetMeta();
            }
            if ((munch & Nibble2) > 0)
                std::cerr << "Warning: This version of Soundbench ignores effect data (offset=" << in.tellg() << ").\n";

            //Get the data field.
            size_t param = vliParse<8>(in);
            uint8_t typefield = in.get();

            //Get the data depending on the type field.
            ParameterValue val;
            val.type = static_cast<ParameterValueType>(typefield);
            switch(val.type) {
            case ParameterByte:
                val.pod.value = in.get();
                break;
            case ParameterPosInt:
                val.pod.value = vliParse<8>(in);
                break;
            case ParameterNegInt:
                val.pod.value = -vliParse<8>(in);
                break;
            case ParameterStr:
                val.str = bitArrayParse(in);
                break;
            case ParameterDecim:
                for(size_t i = 0; i < sizeof(float); ++i)
                    *(reinterpret_cast<uint8_t*>(&(val.pod.decim))+i) = in.get();
                break;
            case ParameterSample:
                float sample;
                for(size_t i = 0; i < sizeof(float); ++i)
                    *(reinterpret_cast<uint8_t*>(&sample)+i) = in.get();
                if (sample > 1.0 || sample < -1.0) {
                    std::cerr << "Sample out of range (offset=" << static_cast<size_t>(in.tellg())-sizeof(float) << "). Resetting the blueprint and aborting...\n";
                    planAllDefaults(blu);
                    return PresetMeta();
                }
                val.pod.sample = sample;
                break;
            default:
                std::cerr << "Unsupported type field (offset=" << in.tellg() << "). Resetting the blueprint and aborting...\n";
                planAllDefaults(blu);
                return PresetMeta();
            }
            if ((munch & Nibble2) > 0)
                blu->eff_data[channum][(munch & Nibble2)-1][static_cast<ModuleParams>(param)] = val;
            else
                blu->gener_data[channum][static_cast<ModuleParams>(param)] = val;

            if (in.eof()) {
                std::cerr << "The preset ended prematurely. Resetting the blueprint and aborting...\n";
                planAllDefaults(blu);
                return PresetMeta();
            }
            munch = in.get();
        }
        return premet;
    }
}
