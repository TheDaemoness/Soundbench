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
        udata.mutt.lock();
        if (b) {
            if (!udata.recording) {
                udata.nodeiter = foist;
                udata.nodeiter->chainDestroy();
                udata.recording = true;
            }
        }
        else {
            if (udata.recording) {
                midi::nodes::MIDIEventNode* nod = new midi::nodes::EndNode;
                udata.nodeiter->attachNext(nod);
                udata.recording = false;
            }
        }
        udata.mutt.unlock();
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

    void RtMidiFrontend::callback(double, std::vector<unsigned char>* message, void* userdata) {
        RtUserData& rtd = *reinterpret_cast<RtUserData*>(userdata);
        uint8_t byte = (*message)[0];
        if(byte & Bit1) {
            rtd.eve.evtype = static_cast<midi::MidiEvents>(byte & Nibble1); //Mask out the channel bits.
            rtd.eve.chan = byte & Nibble2; //Mask out the event type bits.

            if (rtd.eve.evtype != midi::System) {
                rtd.eve.params.first = (*message)[1];
                if (rtd.eve.evtype != midi::ProgramChange && rtd.eve.evtype != midi::ChannelPressure)
                    rtd.eve.params.second = (*message)[2];
            }
        }
        else { //Running status.
            rtd.eve.params.first = byte;
            if (rtd.eve.evtype != midi::ProgramChange && rtd.eve.evtype != midi::ChannelPressure)
                rtd.eve.params.second = (*message)[1];
        }
        midi::doEvent(rtd.syn,rtd.eve);
        rtd.mutt.lock();
        if (rtd.recording) {
            rtd.nodeiter->attachNext(midi::nodes::makeNode(rtd.eve));
            rtd.nodeiter = rtd.nodeiter->returnNext();
        }
        rtd.mutt.unlock();
    }

#else
    bool RtMidiFrontend::instantiable() {
        return false;
    }
#endif
}
