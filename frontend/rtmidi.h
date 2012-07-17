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


#ifndef FRONTEND_RTMIDI_H
#define FRONTEND_RTMIDI_H

#include "frontend/base.h"
#include <mutex>

#ifndef NO_RTMIDI
#include <RtMidi.h>

namespace sb {
    class RtMidiFrontend : public MidiFrontend {
    public:
        static bool instantiable();

        explicit RtMidiFrontend(Synth* s, size_t porte = 0);
        ~RtMidiFrontend() {
            stop();
            if (foist != nullptr) {
                foist->chainDestroy();
                delete foist;
            }
            delete rtm;
        }

        void record(bool b);

        size_t getCurrentPort() {
            return portnum;
        }

        void stop();
        void start();

        bool isRecording() {
            return udata.recording;
        }

        bool supportsVirtualPorts() {
            auto api = rtm->getCurrentApi();
            return api == RtMidi::MACOSX_CORE || api == RtMidi::LINUX_ALSA;
        }

        void setVirtualPort(bool vport) {
            if(!supportsVirtualPorts()) {
                std::cerr << "Attempted to request a virtual MIDI port on a machine that does not support it.\n";
                return;
            }
            usevport = vport;
        }

        std::vector<std::string> getPorts() {
            std::vector<std::string> veci;
            for(size_t i = 0; i < rtm->getPortCount(); ++i)
                veci.push_back(rtm->getPortName(i));
            return veci;
        }

        static void callback(double, std::vector< unsigned char >*, void*);

    private:
        RtMidiIn* rtm;
        struct RtUserData {
            Synth* syn;
            midi::MidiEvent eve;
            midi::nodes::MIDIEventNode* nodeiter;
            std::chrono::time_point<std::chrono::high_resolution_clock> starttime;
            std::mutex mutt;
            bool recording;
        } udata;
    };
}
#else
namespace sb {
    class RtMidiFrontend : public MidiFrontend {
    public:
        static bool instantiable();

        explicit RtMidiFrontend(Synth* s) {}
        ~RtMidiFrontend() {}
        void stop() {};
        void start() {};
        size_t getCurrentPort() {return 0;}
        virtual std::vector<std::string> getPorts() {return std::vector<std::string>()};
    };
}
#endif

#endif // FRONTEND_RTMIDI_H
