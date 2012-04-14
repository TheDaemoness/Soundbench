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
        Synth* MIDIEventNode::synref;
        time_t* MIDIEventNode::clockmicrosecs;

        MIDIEventIterator::MIDIEventIterator(PlayerStartNode* master) {
            star = master;
            auto lamb = [](MIDIEventNode* first) {
                    MIDIEventNode* nowde = first;
                    while (nowde != NULL) {
                        if (nowde->getDelay() > 0)
                            std::this_thread::sleep_for(std::chrono::microseconds(first->accessMIDIClockLen()*nowde->getDelay()));
                        nowde->doevent();
                        nowde = nowde->returnNext();
                    }
            };
            tread = new std::thread(lamb,star->returnNext());
            tread->join();
            delete tread;
            star->stop();
            delete this;
        }
    }
}
