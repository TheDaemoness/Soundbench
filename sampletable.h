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

#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include "basicwaves.h"

namespace sb {

    class PeriodicSampleTable {
    public:
        explicit PeriodicSampleTable(waveBase* wav = nullptr, bool autodelete = true) {
            if (wav == nullptr) {
                samples.resize(curr_srate,0.0);
                return;
            }

            samples.reserve(curr_srate);
            float factor = pi*2/curr_srate;
            for (uint32_t i = 0; i < curr_srate; ++i)
                samples.push_back(wav->getRaw(factor*i));

            if (autodelete)
                delete wav;
        }

    protected:
        std::vector<sbSample> samples;
    };

}

#endif //SAMPLETABLE_H
