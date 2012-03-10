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
        for (size_t i = 0; i < channelcount; ++i)
            planDefaultBasicGen(blu, i);
    }

    void Architect::planDefaultBasicGen(Blueprint* blu, size_t chan_index) {
        blu->gener_data[chan_index][genBasic_wave] = makeParamfromInt(SineWave);
        blu->gener_data[chan_index][genBasic_amplutide] = makeParamfromSample(1.0);
        blu->gener_data[chan_index][genBasic_phase] = makeParamfromInt(0);
    }

    void Architect::planDefaultSynth(Blueprint* blu) {
        for (size_t ac = 0; ac < sb::channelcount; ++ac) {
            blu->gener[ac] = Blueprint::NoGener;
            for (size_t ed = 0; ed < sb::fxcount; ++ed)
                blu->eff[ac][ed] = Blueprint::NoFx; //Indeed.
        }
    }

    void Architect::buildSynth(Synth* syn,Blueprint* blu) {
        syn->inactivechans = 0;
        for (size_t ous = 0; ous < channelcount; ++ous) {
            switch (blu->gener[ous]) {
            default:
                std::cerr << "Unimplemented generator type requested. Defaulting to no generator.\n";
                syn->gener[ous] = NULL;
                ++(syn->inactivechans);
                continue;
            case sb::Blueprint::NoGener:
                syn->gener[ous] = NULL;
                ++(syn->inactivechans);
                break;
            case sb::Blueprint::generBasic:
                syn->gener[ous] = new BasicGen(sb::curr_srate);
                break;
            }
            for (std::pair<const sb::moduleParam,ParameterValue> dat : blu->gener_data[ous]) { //Humbug.
                if(syn->gener[ous] != NULL)
                    syn->gener[ous]->ctrl(dat.first,dat.second);
            }
            syn->reset(); //Do not remove.
        }
    }
}
