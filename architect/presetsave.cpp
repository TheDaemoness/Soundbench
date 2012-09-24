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

        byteArrayWrite(river, name);
        byteArrayWrite(river, arti);
        byteArrayWrite(river, desc);

        toreturn.name = name;
        toreturn.arti = arti;
        toreturn.desc = desc;

        for(GenerType t : blu->gener)
            river.put(static_cast<unsigned char>(t));

        for(const auto& chan : blu->eff) {
            for(const auto& tard : chan)
                river.put(static_cast<unsigned char>(tard));
        }

        //Write the generator's parameters.
        for (size_t i = 0; i < InternalChannels; ++i) {
            for (const auto& pear : blu->gener_data[i]) {
                unsigned char bb = static_cast<unsigned char>(i+1);
                bb <<= 4; //The second nibble should be zero;
                river.put(bb);

                uint16_t whichparam = static_cast<uint16_t>(pear.first);
                river.write(reinterpret_cast<char*>(&whichparam),2);

                river.put(static_cast<char>(pear.second.type));

                switch(pear.second.type) {
                case ParameterPosByte:
                    river.put(static_cast<const unsigned char>(pear.second.pod.value & 255));
                    break;
                case ParameterSigByte:
                    river.put(static_cast<const char>(pear.second.pod.value & 255));
                    break;
                case ParameterPosInt:
                case ParameterSigInt:
                    river.write(reinterpret_cast<const char*>(&pear.second.pod.value),2);
                    break;
                case ParameterByteArray:
                    byteArrayWrite(river,pear.second.str);
                    break;
                case ParameterDecim:
                    river.write(reinterpret_cast<const char*>(&pear.second.pod.decim),sizeof(decltype(pear.second.pod.decim)));
                    break;
                case ParameterSample:
                    river.write(reinterpret_cast<const char*>(&pear.second.pod.sample),sizeof(SbSample));
                    break;
                default:
                    std::cerr << "Unsupported type field requested. This is likely an internal error.\n";
                }
            }
        }
        river.put(0); //Ending marker.
        river.close();

        toreturn.path = path;
        return toreturn;
    }
}
