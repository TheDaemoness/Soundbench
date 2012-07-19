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

    PresetMeta Architect::getMetadata(std::string path, std::string presetdir, std::ifstream* in) {
        if(path.empty())
            return PresetMeta();

        bool close;
        if (in == nullptr) {
            in = new std::ifstream(presetdir+'/'+path,std::ios_base::binary);
            close = true;
        }
        else
            close = false;

        std::string magicheader("SoundbenchPreset");
        if (!in->is_open()) {
            std::cerr << "Something wicked happened when loading the preset.\n";
            return PresetMeta();
        }

        for (char c : magicheader) {
            if(in->get() != c) {
                std::cerr << path << " is not a valid preset. Aborting...\n";
                return PresetMeta();
            }
        }

        uint16_t vnum;
        in->read(reinterpret_cast<char*>(&vnum),2);
        if (vnum > SB_PRESET_VERSION) {
            std::cerr << path << " uses a preset version later than this Soundbench can read.\n";
            std::cerr << "Preset version: " << vnum << '\n';
            std::cerr << "Maximum supported version: " << SB_PRESET_VERSION << '\n';
            return PresetMeta();
        }

        PresetMeta data;
        data.path = path;

        std::cerr << "Getting metadata for " << path << "...\n";

        for (uint8_t i = 0; i < 3; ++i) {
            std::string field = byteArrayParse(*in);
            for(char& tab : field) {
                if(tab == '\t')
                    tab = ' ';
            }
            if (i == 0)
                data.name = field;
            else if (i == 1)
                data.arti = field;
            else
                data.desc = field;
        }
        if (close)
            in->close();
        return data;
    }
}
