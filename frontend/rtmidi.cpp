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

#include "frontend/rtmidi.h"

namespace sb {
#ifndef NO_RTMIDI
    bool RtMidiFrontend::instantiable() {
        RtMidiIn inne;
        return (inne.getPortCount() > 0);
    }

    RtMidiFrontend::RtMidiFrontend(Synth *s) : MidiFrontend(s) {
        udata.syn = s;
        whichport = 0;
        udata.nodeiter= foist;
        udata.record = false;
        udata.recording = false;
        try {
            rtm = new RtMidiIn(RtMidi::UNSPECIFIED,"Soundbench");
        }
        catch (RtError& e) {
            ready = false;
            return;
        }
        rtm->setCallback(callback,reinterpret_cast<void*>(&udata));
        ready = true;
    }

    void RtMidiFrontend::record(bool b) {
        udata.record = b;
    }
    void RtMidiFrontend::stop() {
        rtm->closePort();
        running = false;
    }
    void RtMidiFrontend::start() {
        if (usevport)
            rtm->openVirtualPort("Soundbench MIDI Input");
        else
            rtm->openPort(whichport,"Soundbench MIDI Input");
        running = true;
    }

    void RtMidiFrontend::callback(double timestamp, std::vector<unsigned char>* message, void* userdata) {
        RtUserData& rtd = *reinterpret_cast<RtUserData*>(userdata);
    }

#else
    bool RtMidiFrontend::instantiable() {
        return false;
    }
#endif
}
