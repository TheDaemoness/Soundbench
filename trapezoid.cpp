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

#include "trapezoid.h"

namespace sb {
    SbSample Trapezoid::tick() {
        if((up && top) || (!up && bottom))
            return level;

        if (up && !top)
            level += rate;
        else if (!up && !bottom)
            level -= rate;

        //Deal with potential overshooting.
        if (level > SbSampleMax) {
            top = true;
            level = SbSampleMax;
        }
        else if (level < SbSampleZero) {
            bottom = true;
            level = SbSampleZero;
        }

        return level;
    }
    void Trapezoid::attack() {
        up = true;
        rate = 1.0/(timeup*SampleRate)*SbSampleMax;
        top = false;
    }
    void Trapezoid::release() {
        up = false;
        rate = 1.0/(timedown*SampleRate)*SbSampleMax;
        bottom = false;
    }
}
