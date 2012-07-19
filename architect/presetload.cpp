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
        if (in.eof()) {
            std::cerr << "The preset ended during or at the end of the metadata section. Resetting the blueprint and aborting...\n";
            planAllDefaults(blu);
            return PresetMeta();
        }

        if (premet.path.empty()) //Oops.
            return PresetMeta();

        //Deal with the channel section.
        for (size_t alities = 0; alities < InternalChannels; ++alities)
            blu->gener[alities] = static_cast<GenerType>(in.get());

        int munch; //Bite, chew, masticate, etc.
        for (size_t i = 0; i < InternalChannels; ++i) {
            for (size_t j = 0; j < FxPerChannel; ++j) {
                munch = in.get();
                blu->eff[i][j] = static_cast<FxType>(munch);
            }
        }
        if (in.eof()) {
            std::cerr << "The preset ended during or at the end of the channel section. Resetting the blueprint and aborting...\n";
            planAllDefaults(blu);
            return PresetMeta();
        }

        //Deal with the arguments section.
        munch = in.get();
        while(munch != 0) {
            uint8_t channum = (munch & Nibble1) >> 4;
            if (channum == 0 || channum > 4) {
                std::cerr << "Invalid channel nibble (offset=" << in.tellg() << ", value=" << static_cast<uint16_t>(channum) << "). Resetting the blueprint and aborting...\n";
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
            uint16_t whichparam = 0;
            in.read(reinterpret_cast<char*>(&whichparam),2);

            std::cerr << "Which channel: " << static_cast<int>(channum) << ".\n";

            //Get the data depending on the type field.
            ParameterValue val;
            val.type = static_cast<ParameterValueType>(in.get());
            switch(val.type) {
            case ParameterPosByte:
                val.pod.value = static_cast<uint8_t>(in.get());
                break;
            case ParameterSigByte:
                val.pod.value = static_cast<int8_t>(in.get());
                break;
            case ParameterPosInt:
                uint16_t unsigs;
                in.read(reinterpret_cast<char*>(&unsigs),2);
                val.pod.value = unsigs;
                break;
            case ParameterSigInt:
                int16_t sigs;
                in.read(reinterpret_cast<char*>(&sigs),2);
                val.pod.value = sigs;
                break;
            case ParameterByteArray:
                val.str = byteArrayParse(in);
                break;
            case ParameterDecim:
                float value;
                in.read(reinterpret_cast<char*>(&value),sizeof(float));
                val.pod.decim = value;
                break;
            case ParameterSample:
                SbSample sample;
                in.read(reinterpret_cast<char*>(&sample),sizeof(SbSample));
                if (sample > 1.0 || sample < -1.0) {
                    std::cerr << "Sample out of range (offset=" << static_cast<size_t>(in.tellg())-sizeof(SbSample) << "). Resetting the blueprint and aborting...\n";
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

            std::cerr << "Type: " << val.type << "\tValue: " << val.pod.value << "\tSample: " << val.pod.sample << "\tDecimal: " << val.pod.decim << "\n";

            if ((munch & Nibble2) > 0)
                blu->eff_data[channum-1][(munch & Nibble2)-1][static_cast<ModuleParams>(whichparam)] = val;
            else
                blu->gener_data[channum-1][static_cast<ModuleParams>(whichparam)] = val;

            if (in.eof()) {
                std::cerr << "The preset ended in the channel arguments section. Resetting the blueprint and aborting...\n";
                planAllDefaults(blu);
                return PresetMeta();
            }
            munch = in.get();
        }
        return premet;
    }
}
