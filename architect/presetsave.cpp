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
#include "filewriters.h"

namespace sb {
    PresetMeta Architect::savePreset(std::string path, std::string presetdir, Blueprint* blu, std::string name, std::string arti, std::string desc) {
        std::ofstream river(presetdir+'/'+path,std::ios_base::trunc);
        if (!river.is_open()) {
            std::cerr << "Something wicked happened when trying to open a file to save the preset.\n";
            return PresetMeta();
        }
        PresetMeta toreturn;
        river.write("SoundbenchPreset",16);

        uint16_t ver = SB_PRESET_VERSION;
        river.write(reinterpret_cast<char*>(&ver),2);

        bitArrayWrite(river, name);
        bitArrayWrite(river, arti);
        bitArrayWrite(river, desc);

        toreturn.name = name;
        toreturn.arti = arti;
        toreturn.desc = desc;

        for(GenerType t : blu->gener)
            river.put(static_cast<char>(t));

        for(const auto& a : blu->eff) {
            for(size_t i = 0; i < FxPerChannel - 1; ++i)
                river.put(static_cast<char>(a[i] | Bit1));
            river.put(static_cast<char>(a[FxPerChannel - 1]));
        }

        //Write the generator's parameters.
        for (size_t i = 0; i < InternalChannels; ++i) {
            for (auto& pear : blu->gener_data[i]) {
                char elem = i << 4; //The second nibble should be zero.
                river.put(elem);
                vliWrite(river,pear.first);
                river.put(static_cast<char>(pear.second.type));
                switch(pear.second.type) {
                case ParameterByte:
                    river.put(static_cast<char>(pear.second.pod.value));
                    break;
                case ParameterPosInt:
                    vliWrite(river,pear.second.pod.value);
                    break;
                case ParameterNegInt:
                    vliWrite(river,-pear.second.pod.value);
                    break;
                case ParameterStr:
                    bitArrayWrite(river,pear.second.str);
                    break;
                case ParameterDecim:
                    river.write(reinterpret_cast<char*>(&pear.second.pod.decim),sizeof(float));
                    break;
                case ParameterSample:
                    river.write(reinterpret_cast<char*>(&pear.second.pod.sample),sizeof(SbSample));
                    break;
                default:
                    std::cerr << "Unsupported/unused parameter value type. Please note that this preset will not load correctly.\n";
                }
            }
        }

        river.put(0); //Ending marker.
        river.close();

        toreturn.path = path;
        return toreturn;
    }
}
