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

    bool JackAudioBackend::instantiable() {
#ifndef NO_JACK
        if (cli == nullptr)
            cli = jack_client_open("Soundbench",JackNoStartServer,&stat); //May remove the JackNoStartServer option in the future.
        if (stat & JackFailure) {
            if (stat & JackServerFailed)
                std::cerr << "Could not find a running JACK server.\nNote: Soundbench does not start the server if it's not present.\n";
            if (stat & JackServerError)
                std::cerr << "Could not communicate with the JACK server.\n";
        }
        return !(stat & JackFailure);
#else
        return false;
#endif
    }

#ifndef NO_JACK
    JackAudioBackend::JackAudioBackend(Synth* s, size_t& srate, std::map<size_t, bool>& srates, size_t) {
        ready = false;
        if (cli == nullptr)
            cli = jack_client_open("Soundbench",static_cast<jack_options_t>(0),&stat);

        if (cli == nullptr) {
            std::cerr << "Something went horribly wrong initializing the JACK backend.";
            return;
        }

        udata.lport = jack_port_register(cli,"Audio Output - Left", JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,srate/100);
        udata.rport = jack_port_register(cli,"Audio Output - Right", JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,srate/100);

        srate = jack_get_sample_rate(cli);

        for(size_t i = 0; i < SupportedRatesCount; ++i)
            srates[SupportedRates[i]] = false; //JACK is in control of sample rate switching here.

        udata.synref = s;
        udata.cliref = cli;
        udata.buffsize = 0;

        sampling_rate = srate;
        syn = s;
        ready = true;
    }
    JackAudioBackend::~JackAudioBackend() {
        stop();
        if (cli != nullptr) {
            jack_client_close(cli);
            cli = nullptr;
        }
    }

    void JackAudioBackend::start() {
        if (cli != nullptr) {
            int err = jack_activate(cli);
            if (err != 0) {
                //Error handling stuff here.
            }
        }
    }

    void JackAudioBackend::stop() {
        if (cli != nullptr) {
            int err = jack_deactivate(cli);
            if (err != 0) {
                //Error handling stuff here.
            }
        }
    }

    bool JackAudioBackend::usesPorts() {
        return true;
    }
    std::vector<std::string> JackAudioBackend::getPorts() {
        if (cli == nullptr)
            return std::vector<std::string>();

        const char** portes = jack_get_ports(cli,NULL,NULL,static_cast<unsigned long>(JackPortIsInput));

        std::vector<std::string> ret;
        for (size_t i = 0; portes[i] != NULL; ++i)
            ret.push_back(portes[i]);

        if (portes != NULL)
            jack_free(portes);
        return ret;
    }
    std::vector<std::size_t> JackAudioBackend::getConnections(bool rightport) {
        if (cli == nullptr)
            return std::vector<size_t>();

        const char** connlist = jack_port_get_connections(rightport?udata.rport:udata.lport);
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
        if (cli == nullptr)
            return false;

        int e = 0;
        if (conn) {
            e = jack_connect(cli,
                             jack_port_name(rightport?udata.rport:udata.lport),
                             jack_port_name(jack_port_by_id(cli,portid)));
        }
        else {
            e = jack_disconnect(cli,
                                jack_port_name(rightport?udata.rport:udata.lport),
                                jack_port_name(jack_port_by_id(cli,portid)));
        }
        if (e != 0) {
            //Error handling stuff here.
            return false;
        }
        return true;
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
    std::vector<std::string> JackAudioBackend::getDevices() {
        return std::vector<std::string>();
    }

    void JackAudioBackend::setSamplingRate(size_t) {
        std::cerr << "The sampling rate cannot be changed while using JACK.\n";
    }
#endif
}

