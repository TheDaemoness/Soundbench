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

#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include "gener_basic.h"

//libstdc++11
#include <vector>

namespace sb {

    struct Blueprint {
        typedef std::map<moduleParam,ParameterValue> ModuleDataType;
        enum gener_type {NoGener, generSampler, generBasic, generAdditive, generModal, generWaveguide} gener[sb::InternalChannels];
        enum fx_type {NoFx, fxFIR} eff[sb::InternalChannels][sb::FxPerChannel];
        ModuleDataType gener_data[sb::InternalChannels];
        ModuleDataType eff_data[sb::InternalChannels][sb::FxPerChannel];
    };

}

#endif // BLUEPRINT_H
