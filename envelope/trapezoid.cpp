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
        if (level > SbSample_MAX) {
            top = true;
            level = SbSample_MAX;
        }
        else if (level < SbSample_ZERO) {
            bottom = true;
            level = SbSample_ZERO;
        }

        return level;
    }
    void Trapezoid::attack() {
        up = true;
        rate = 1.0/(timeup*global_srate)*SbSample_MAX;
        top = false;
    }
    void Trapezoid::release() {
        up = false;
        rate = 1.0/(timedown*global_srate)*SbSample_MAX;
        bottom = false;
    }
}
