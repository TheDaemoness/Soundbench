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

namespace sb {

    Architect::Architect() {
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
            if(syn->gener[ous] != nullptr)
                delete syn->gener[ous];
            switch (blu->gener[ous]) {

            case GenerBasic:
                syn->gener[ous] = new BasicGen(syn->currentpoly);
                break;
            default:
                std::cerr << "Unimplemented generator type #" << blu->gener[ous] <<  " requested for channel " << ous+1 << ". Defaulting to no generator.\n";
            case NoGener:
                syn->gener[ous] = nullptr;
                ++(syn->inactivechans);
                break;
            }
            for (size_t effous = 0; effous < FxPerChannel; ++effous) {
                if(syn->eff[ous][effous] != nullptr)
                    delete syn->eff[ous][effous];
                switch (blu->eff[ous][effous]) {
                case FxBiquad:
                    syn->eff[ous][effous] = new BiquadFx;
                    break;
                default:
                    std::cerr << "Unimplemented effect type #" << blu->eff[ous][effous] <<  " requested for channel " << ous+1 <<  ", effect " << effous+1 << ". Defaulting to no effect.\n";
                case NoFx:
                    syn->eff[ous][effous] = nullptr;
                    break;
                }
            }
            for (std::pair<const sb::ModuleParams,ParameterValue> dat : blu->gener_data[ous]) { //Humbug.
                if(syn->gener[ous] != nullptr)
                    syn->gener[ous]->ctrl(dat.first,dat.second);
            }
            for(size_t ness = 0; ness < FxPerChannel; ++ness) {
                for (std::pair<const sb::ModuleParams,ParameterValue> dat : blu->eff_data[ous][ness]) {

                    if(syn->eff[ous][ness] != nullptr)
                        syn->eff[ous][ness]->ctrl(dat.first,dat.second);
                }
            }
        }
        for (uint8_t parrot = 0; parrot < syn->currentpoly && syn->currentpoly != DefaultPolyphony; ++parrot) {
            if(syn->gener[parrot] != nullptr)
                syn->gener[parrot]->setPolymorphism(parrot);
        }
    }
}
