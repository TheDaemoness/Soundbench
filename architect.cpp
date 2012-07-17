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

#include <fstream>

namespace sb {

    Architect::Architect() {
    }

    void Architect::planAllDefaults(Blueprint* blu) {
        for (size_t i = 0; i < InternalChannels; ++i)
            planDefaultBasicGen(blu, i);
        for (size_t ac = 0; ac < InternalChannels; ++ac) {
            blu->gener[ac] = NoGener;
            for (size_t ed = 0; ed < FxPerChannel; ++ed)
                blu->eff[ac][ed] = NoFx; //Indeed.
        }
    }

    PresetMeta Architect::getMetadata(std::string path, std::string presetdir) {
        if(path.empty())
            return PresetMeta();

        std::ifstream in(presetdir+'/'+path,std::ios_base::binary);
        std::string magicheader("SoundbenchPreset");

        if (!in.is_open()) {
            std::cerr << "Something wicked happened when sending the preset's location.\n";
            return PresetMeta();
        }

        for (char c : magicheader) {
            if(in.get() != c) {
                std::cerr << path << " is not a valid preset. Aborting...\n";
                return PresetMeta();
            }
        }

        uint16_t vnum = in.get();
        vnum <<= 8;
        vnum |= in.get();
        if (vnum > SB_PRESET_VERSION) {
            std::cerr << path << " uses a preset version later than this Soundbench can read.\n";
            std::cerr << "Preset version: " << vnum << '\n';
            std::cerr << "Maximum supported version: " << SB_PRESET_VERSION << '\n';
        }

        PresetMeta data;
        data.path = path;
        char chara;

        std::cerr << "Getting metadata for " << path << "...\n";

        for (uint8_t i = 0; i < 3; ++i) {
            bool cont = true;
            while (cont) {
                chara = in.get();
                cont = chara & Bit1;

                chara &= ~Bit1;

                if (chara == '\t')
                    chara = ' '; //To stop wiseguys from screwing up the indexer.

                if (i == 0)
                    data.name.push_back(chara);
                else if (i == 1)
                    data.arti.push_back(chara);
                else
                    data.desc.push_back(chara);
            }
        }

        in.close();
        return data;
    }

    void Architect::planDefaultBasicGen(Blueprint* blu, size_t chan_index) {
        blu->gener_data[chan_index][GenBasicWave] = makeParamfromInt(SineWave);
        blu->gener_data[chan_index][GenBasicAmp] = makeParamfromSample(1.0);
        blu->gener_data[chan_index][GenBasicPhase] = makeParamfromInt(0);
        blu->gener_data[chan_index][GenerAttackTime] = makeParamfromFloat(0.1);
        blu->gener_data[chan_index][GenerReleaseTime] = makeParamfromFloat(0.5);
    }

    void Architect::stripSynth(Synth* syn) {
        for (size_t alized = 0; alized < InternalChannels; ++alized) {
            if(syn->gener[alized] != nullptr) {
                delete syn->gener[alized];
                syn->gener[alized] = nullptr;
            }
        }
    }

    void Architect::buildSynth(Synth* syn, Blueprint* blu) {
        syn->inactivechans = 0;
        for (size_t ous = 0; ous < InternalChannels; ++ous) {
            switch (blu->gener[ous]) {
            case NoGener:
                if(syn->gener[ous] != nullptr)
                    delete syn->gener[ous];
                syn->gener[ous] = nullptr;
                ++(syn->inactivechans);
                break;
            case GenerBasic:
                if(syn->gener[ous] != nullptr)
                    delete syn->gener[ous];
                syn->gener[ous] = new BasicGen(sb::SampleRate);
                break;
            default:
                std::cerr << "Unimplemented generator type requested for channel " << ous+1 << ". Defaulting to no generator.\n";
                if(syn->gener[ous] != nullptr)
                    delete syn->gener[ous];
                syn->gener[ous] = nullptr;
                ++(syn->inactivechans);
                break;
            }
            for (size_t effous = 0; effous < FxPerChannel; ++effous) {
                switch (blu->eff[ous][effous]) {
                case NoFx:
                    if(syn->eff[ous][effous] != nullptr)
                        delete syn->eff[ous][effous];
                    syn->eff[ous][effous] = nullptr;
                    break;
                default:
                    std::cerr << "Unimplemented effect type requested for channel " << ous+1 <<  ", effect " << effous+1 << ". Defaulting to no effect.\n";
                    if(syn->eff[ous][effous] != nullptr)
                        delete syn->eff[ous][effous];
                    syn->eff[ous][effous] = nullptr;
                    break;
                }
            }
            for (std::pair<const sb::ModuleParams,ParameterValue> dat : blu->gener_data[ous]) { //Humbug.
                if(syn->gener[ous] != nullptr)
                    syn->gener[ous]->ctrl(dat.first,dat.second);
            }
            for (uint8_t parrot = 0; parrot < syn->currentpoly && syn->currentpoly != DefaultPolyphony; ++parrot) {
                if(syn->gener[parrot] != nullptr)
                    syn->gener[parrot]->setPolymorphism(parrot);
            }

        }
    }
}
