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

    Copyright 2014  Amaya S.
*/

#ifndef SYNTHBASE_H
#define SYNTHBASE_H

#include "sbutilities.h"

namespace sb {

    /*
        WARNING: This enum is *very* close to conflicting with the one on midienums.h.

        Although they are in different namespaces, for the sake of avoiding confusion, please try to keep the names in each one different.
    */
    enum SupportedPedals {
        NoPedal,
        Hold,
        Sustenuto
    };

	class Synth {
    public:
        virtual void noteOn(int halfsteps, SbSample amp) = 0;
        virtual void noteOff(int halfsteps) = 0;
        virtual void pedal(SupportedPedals which, bool val = true) = 0;
        virtual void reset() = 0;
        virtual void tick(SbSample* frame) = 0;
        virtual void channel_tick(uint8_t chanval, SbSample* frame) = 0;
        virtual void interleaved_block(SbSample* frames, size_t framecount) = 0;
        virtual void uninterleaved_blocks(SbSample* lframes, SbSample* rframes, size_t framecount) = 0;
        virtual uint8_t channel_count() = 0;

        virtual void setPolyphony(uint8_t) = 0;
        virtual void updateSamplingRate() = 0;

        inline SbSample& volume() {return vol;}
        inline uint8_t getThreadLevel() {return tlevel;}

    protected:
        uint8_t tlevel;
        SbSample vol;
    };

}

#endif // SYNTHBASE_H
