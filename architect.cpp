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

    void Architect::planAllDefaults(Blueprint* blu) {
        for (size_t i = 0; i < InternalChannels; ++i)
            planDefaultBasicGen(blu, i);
        for (size_t ac = 0; ac < sb::InternalChannels; ++ac) {
            blu->gener[ac] = Blueprint::NoGener;
            for (size_t ed = 0; ed < sb::FxPerChannel; ++ed)
                blu->eff[ac][ed] = Blueprint::NoFx; //Indeed.
        }
    }

    void Architect::planDefaultBasicGen(Blueprint* blu, size_t chan_index) {
        blu->gener_data[chan_index][genBasic_wave] = makeParamfromInt(SineWave);
        blu->gener_data[chan_index][genBasic_amplutide] = makeParamfromSample(1.0);
        blu->gener_data[chan_index][genBasic_phase] = makeParamfromInt(0);
    }

    void Architect::buildSynth(Synth* syn, Blueprint* blu) {
        syn->inactivechans = 0;
        for (size_t ous = 0; ous < InternalChannels; ++ous) {
            switch (blu->gener[ous]) {
            default:
                std::cerr << "Unimplemented generator type requested. Defaulting to no generator.\n";
            case Blueprint::NoGener:
                syn->gener[ous] = nullptr;
                ++(syn->inactivechans);
                break;
            case Blueprint::generBasic:
                syn->gener[ous] = new BasicGen(sb::SampleRate);
                break;
            }
            for (size_t effous = 0; effous < FxPerChannel; ++effous) {
                switch (blu->eff[ous][effous]) {
                    default:
                        std::cerr << "Unimplemented effect type requested. Defaulting to no effect.\n";
                    case Blueprint::NoFx:
                        syn->eff[ous][effous] = nullptr;
                    break;
                }
            }
            for (std::pair<const sb::moduleParam,ParameterValue> dat : blu->gener_data[ous]) { //Humbug.
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
