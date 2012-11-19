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

#include "jackmidi.h"

namespace sb {
    bool JackMidiFrontend::instantiable() {
#ifndef NO_JACK
        if (cli == nullptr && stat == 0)
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
}
