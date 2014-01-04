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

#ifndef TICKER_H
#define TICKER_H

#include "sbutilities.h"

namespace sb {
    class FloatTicker {
    private:
        float pos;
        float speed;
    public:
        FloatTicker() {
            pos = 0;
            speed = 0;
        }
        void setPos(float p) {
            pos = p;
        }
        void setSpeed(float s) {
            speed = s;
        }
        float tick() {
            pos += speed;
            return pos;
        }
        float getValue() {
            return pos;
        }
        void reset() {
            pos = 0;
        }
    };
}

#endif // TICKER_H
