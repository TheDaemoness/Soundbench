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


#include "synth.h"

namespace sb {
    void Synth::tick(SbSample* frames) {
        for (size_t ing = 0; ing < InternalChannels; ++ing) {
            channel_tick(ing,buffer[ing]); //NOOOOO!
            for (size_t acid = 0; acid < OutChannels; ++acid) { //Loop per outbound channel.
                if(!ing) //Only reset the value of frames once.
                    frames[ing] = SbSampleZero;
                frames[ing] += buffer[ing][acid]; //Chemistry failure.
            }
        }
        if (inactivechans != InternalChannels) {
            for (size_t out = 0; out < OutChannels; ++out) {
                frames[out] /= (InternalChannels-inactivechans); //Correctly average the signal from the running channels.
                frames[out] *= vol; //Apply the master volume.
            }
        }
    }

    void Synth::channel_tick(uint8_t ic, SbSample *frame) {
        if (gener[ic] != nullptr)
            gener[ic]->tick(frame, OutChannels);
        else {
            for (size_t z = 0; z < OutChannels; ++z)
                frame[z] = SbSampleZero;
        }
        for (size_t ient = 0; ient < FxPerChannel; ++ient) { //Loop per effect.
            if (eff[ic][ient] != nullptr)
                eff[ic][ient]->tick(frame,OutChannels);
        }
    }
}
