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

#include "geners.h"
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
        void noteOn(int halfsteps, sbSample amp);
        void noteOff(int halfsteps);
        void pedal(SupportedPedals which, bool val = true);
        void reset();
        void tick(sbSample* frame, size_t chans);

        inline sbSample& volume() {return vol;}
        friend class Architect;

    private:
        struct NoteInfo {
            int8_t noteoffset;
            sbSample amp;
            SupportedPedals pedal; //For any pedals that affect this note.
        };

        sbSample buffer[channelcount][outchans];
        genBase* gener[channelcount];
        fxBase* eff[channelcount][fxcount];
        std::vector<NoteInfo> notes;

        bool holdped, sustped;

        sbSample vol;
        size_t inactivechans;
    };
}

#endif // SYNTH_H
