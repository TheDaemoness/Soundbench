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

#include "midichain.h"

namespace sb {
    namespace midi {
        namespace nodes {

            MIDIEventNode* MIDIEventNode::makeNode(MidiEvent miditem) {
                switch(miditem.evtype) {
                case NoteOn:
                    if (miditem.params.second != 0)
                        return new NoteOnEventNode(miditem.params.first-69, /*69 is the MIDI number for A4*/
                                                                            static_cast<SbSample>(miditem.params.second)/127,
                                                                            miditem.delay);
                    else
                        return new NoteOffEventNode(miditem.params.first-69, miditem.delay);
                case NoteOff:
                    return new NoteOffEventNode(miditem.params.first-69, miditem.delay);

                case Controller:
                    switch (miditem.params.first) {
                    case HoldPedal:
                        return new HoldPedalEventNode(miditem.params.second, miditem.delay);
                    case SustenutoPedal:
                        return new SustenutoEventNode(miditem.params.second, miditem.delay);
                    default:
                        return new DelayNode(miditem.delay);
                    }
                case ProgramChange:
                case ChannelPressure:
                case PitchBend:
                    return new DelayNode(miditem.delay);
                case Aftertouch:
                    return new DelayNode(miditem.delay);
                case SystemEvent:
                    switch (miditem.meta) {
                    case MetaTempo:
                    case MetaTrackName:
                        return new DelayNode(miditem.delay);
                    default:
                        return new DelayNode(miditem.delay);
                    }
                case EndOfTrack:
                    return new EndNode(miditem.delay);
                default:
                    return nullptr;
                }
            }

        }
    }
}
