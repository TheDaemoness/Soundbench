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

#include "backend/jackaudio.h"

#include <algorithm>

namespace sb {
#ifndef NO_JACK
    bool JackAudioBackend::instantiable() {
        return true; //There are no lightweight checks that can be performed here.
    }
    JackAudioBackend::JackAudioBackend(Synth* s, size_t& srate, std::map<size_t, bool>& srates, size_t) {
        cli = jack_client_open("Soundbench",static_cast<jack_options_t>(0),&stat);

        lport = jack_port_register(cli,"Soundbench Output - Left", JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,srate/100);
        rport = jack_port_register(cli,"Soundbench Output - Right", JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,srate/100);

        srate = jack_get_sample_rate(cli);

        for(size_t i = 0; i < SupportedRatesCount; ++i)
            srates[SupportedRates[i]] = false; //JACK is in control of sample rate switching here.

        sampling_rate = srate;
        syn = s;
    }
    JackAudioBackend::~JackAudioBackend() {
        stop();
        jack_client_close(cli);
    }

    void JackAudioBackend::start() {
        int err = jack_activate(cli);
        if (err != 0) {
            //Error handling stuff here.
        }
    }

    void JackAudioBackend::stop() {
        int err = jack_deactivate(cli);
        if (err != 0) {
            //Error handling stuff here.
        }
    }
    size_t JackAudioBackend::getDefaultDevice() {
        return 0;
    }
    size_t JackAudioBackend::getCurrentDevice() {
        return 0;
    }
    void JackAudioBackend::setDevice(uint32_t) {
        return;
    }

    bool JackAudioBackend::usesPorts() {
        return true;
    }
    std::vector<std::string> JackAudioBackend::getPorts() {
        const char** portes = jack_get_ports(cli,NULL,NULL,static_cast<unsigned long>(JackPortIsInput));

        std::vector<std::string> ret;
        for (size_t i = 0; portes[i] != NULL; ++i)
            ret.push_back(portes[i]);

        if (portes != NULL)
            jack_free(portes);
        return ret;
    }
    std::vector<std::size_t> JackAudioBackend::getConnections(bool rightport) {
        const char** connlist = jack_port_get_connections(rightport?rport:lport);
        std::vector<std::string> portlist = getPorts();
        std::vector<std::size_t> ret;

        for (size_t i = 0; connlist[i] != NULL; ++i) {
            auto loc = std::find(portlist.begin(),portlist.end(),connlist[i]);
            if(loc != portlist.end())
                ret.push_back(loc-portlist.begin());
        }

        if (connlist != NULL)
            jack_free(connlist);
        return ret;

    }
    bool JackAudioBackend::modifyConnection(bool rightport, size_t portid, bool conn) {
        int e = 0;
        if (conn) {
            e = jack_connect(cli,
                         jack_port_name(rightport?rport:lport),
                         jack_port_name(jack_port_by_id(cli,portid)));
        }
        else {
            e = jack_disconnect(cli,
                         jack_port_name(rightport?rport:lport),
                         jack_port_name(jack_port_by_id(cli,portid)));
        }
        if (e != 0) {
            //Error handling stuff here.
            return false;
        }
        return true;
    }

    void JackAudioBackend::setSamplingRate(size_t) {
        std::cerr << "The sampling rate cannot be changed while using JACK.\n";
    }

#else
    bool JackAudioBackend::instantiable() {
        return false;
    }
#endif
}

