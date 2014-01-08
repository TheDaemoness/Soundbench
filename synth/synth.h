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

#include "synthbase.h"
#include <chrono>
#include <thread>

namespace sb {

    class Architect;

    class Synth : public SynthBase {
    public:
        Synth();
        void noteOn(int halfsteps, SbSample amp);
        void noteOff(int halfsteps);
        void pedal(SupportedPedals which, bool val = true);
        void reset();
        void tick(SbSample* frame);
        void channel_tick(uint8_t chanval, SbSample* frame);
        void interleaved_block(SbSample* frames, size_t framecount);
        void uninterleaved_blocks(SbSample* lframes, SbSample* rframes, size_t framecount);

        void setPolyphony(uint8_t);
        void updateSamplingRate();

        uint8_t channel_count() {return sb::InternalChannels;}

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

        uint8_t currentpoly;
        size_t inactivechans;
    };
}

#endif // SYNTH_H
