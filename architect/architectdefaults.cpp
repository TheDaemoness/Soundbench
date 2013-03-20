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
    void Architect::planAllDefaults(Blueprint* blu) {
        for (size_t ac = 0; ac < InternalChannels; ++ac) {
            blu->gener[ac] = NoGener;
            planDefaultBasicGen(blu, ac);
            for (size_t ed = 0; ed < FxPerChannel; ++ed) {
                blu->eff[ac][ed] = NoFx; //Indeed.
                planDefaultBiquadFx(blu,ac,ed);
            }
        }
    }

    void Architect::planDefaultBasicGen(Blueprint* blu, size_t chan_index) {
        blu->gener_data[chan_index][GenBasicNoteBias] = makeParamfromInt(0);
        blu->gener_data[chan_index][GenBasicWave] = makeParamfromInt(SineWave);
        blu->gener_data[chan_index][GenBasicAmp] = makeParamfromSample(1.0);
        blu->gener_data[chan_index][GenBasicPhase] = makeParamfromSample(0);
        blu->gener_data[chan_index][GenerAttackTime] = makeParamfromFloat(0.1);
        blu->gener_data[chan_index][GenerReleaseTime] = makeParamfromFloat(0.5);

        blu->gener_data[chan_index][GenBasicNoteBias].type = sb::ParameterPosInt;
        blu->gener_data[chan_index][GenBasicWave].type = sb::ParameterPosInt;
        blu->gener_data[chan_index][GenBasicAmp].type = sb::ParameterSample;
        blu->gener_data[chan_index][GenBasicPhase].type = sb::ParameterSample;
        blu->gener_data[chan_index][GenerAttackTime].type = sb::ParameterDecim;
        blu->gener_data[chan_index][GenerReleaseTime].type = sb::ParameterDecim;
    }

    void Architect::planDefaultBiquadFx(Blueprint* blu, size_t chan_index, size_t fx_num) {
        blu->eff_data[chan_index][fx_num][FxBiquadType] = makeParamfromInt(BiquadLowpass);
        blu->eff_data[chan_index][fx_num][FxBiquadFreq] = makeParamfromFloat(global_srate/3);
        blu->eff_data[chan_index][fx_num][FxBiquadGain] = makeParamfromFloat(-12.0);
        blu->eff_data[chan_index][fx_num][FxBiquadQ] = makeParamfromFloat(3.0);
        blu->eff_data[chan_index][fx_num][FxBiquadBW] = makeParamfromFloat(2.0);
        blu->eff_data[chan_index][fx_num][FxBiquadS] = makeParamfromFloat(1.0);

        blu->eff_data[chan_index][fx_num][FxBiquadA0] = makeParamfromFloat(0.0);
        blu->eff_data[chan_index][fx_num][FxBiquadA1] = makeParamfromFloat(0.0);
        blu->eff_data[chan_index][fx_num][FxBiquadA2] = makeParamfromFloat(0.0);
        blu->eff_data[chan_index][fx_num][FxBiquadB0] = makeParamfromFloat(0.0);
        blu->eff_data[chan_index][fx_num][FxBiquadB1] = makeParamfromFloat(0.0);
        blu->eff_data[chan_index][fx_num][FxBiquadB2] = makeParamfromFloat(0.0);
    }

    void Architect::planDefaultSamplerGen(Blueprint* blu, size_t chan_index) {
        blu->gener_data[chan_index][GenSamplerPath] = makeParamfromString("");
    }
}
