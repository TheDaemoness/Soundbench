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

#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "sbutilities.h"

//This is a high-performance trapezoidal envelope.

namespace sb {
    class Trapezoid {
    public:
        Trapezoid() {
            reset();
        }
        inline void setAttackTime(float ups) {
            timeup = ups;
        }
        inline void setReleaseTime(float downs) {
            timedown = downs;
        }
        inline void reset() {
            level = SbSampleZero;
            up = true;
            top = true;
            bottom = true;
        }
        SbSample getLevel() {
            return level;
        }

        SbSample tick();
        void attack();
        void release();
    private:
        float timeup, timedown, rate;
        SbSample level;
        bool up, top, bottom;
    };
}

#endif // TRAPEZOID_H
