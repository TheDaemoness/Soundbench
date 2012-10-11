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

#ifndef SYNTH_H
#define SYNTH_H

#include "gener/basic.h"
#include <chrono>

namespace sb {

    class Architect;

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
        Synth();
        void noteOn(int halfsteps, SbSample amp);
        void noteOff(int halfsteps);
        void pedal(SupportedPedals which, bool val = true);
        void reset();
        void tick(SbSample* frame, size_t chans, bool left = true);
        void setPolyphony(uint8_t);
        void updateSamplingRate();

        inline SbSample& volume() {return vol;}
        friend class Architect;

    private:
        struct NoteInfo {
            NoteInfo() {
                noteoffset = 0;
                amp = SbSampleZero;
                pedal = NoPedal;
            }

            int8_t noteoffset;
            SbSample amp;
            SupportedPedals pedal; //For any pedals that affect this note.
        };

        SbSample buffer[InternalChannels][OutChannels];
        GenBase* gener[InternalChannels];
        FxBase* eff[InternalChannels][FxPerChannel];
        std::vector<NoteInfo> notes;

        bool holdped, sustped;

        SbSample vol;
        uint8_t currentpoly;
        size_t inactivechans;
    };
}

#endif // SYNTH_H
