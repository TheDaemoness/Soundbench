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

#ifndef INTTICKER_H
#define INTTICKER_H

#include "sbutilities.h"

namespace sb {
    class IntTicker {
    private:
        double pos;
        float speed;
    public:
        IntTicker() {
            pos = 0;
            speed = 0;
        }
        void setPos(size_t p) {
            pos = static_cast<double>(p);
        }
        void setSpeed(float s) {
            speed = s;
        }
        size_t tick() {
            pos += speed;
            return static_cast<size_t>(pos);
        }
        size_t getValue() {
            return static_cast<size_t>(pos);
        }
    };
}

#endif // INTTICKER_H
