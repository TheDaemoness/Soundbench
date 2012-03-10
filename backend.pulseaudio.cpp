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
#include "backend.pulseaudio.h"
#include <pulse/error.h>

bool pulseaudio_backend::instantiable() {
    pa_sample_spec specs;
    specs.channels = 1;
    specs.format = sample_format;
    specs.rate = 44100;
    int e;
    pa_simple* simpsons = pa_simple_new(NULL, "Soundbench", PA_STREAM_PLAYBACK, NULL, "Instantiability Test", &specs, NULL, NULL, &e);
    if (e == PA_OK) {
        pa_simple_free(simpsons);
        return true;
    }
    else {
        return false;
    }
}

pulseaudio_backend::pulseaudio_backend(sb::Synth* s, size_t& sample_rate, std::map<size_t, bool>&, size_t chans) {
    sampling_rate = sample_rate;
    tread = NULL;
    syn = s;

    run = new bool;

    specs = new pa_sample_spec;
    specs->channels = chans;
    specs->format = sample_format;
    specs->rate = sample_rate;
}

pulseaudio_backend::~pulseaudio_backend() {
    stop();
    delete run;
    delete specs;
    pa_simple_free(river);
}

void pulseaudio_backend::setSamplingRate(size_t neorate) {
    stop();
    delete run;
    pa_simple_free(river);
    specs->rate = neorate;
}

size_t pulseaudio_backend::returnSuggestedBufferSize() {
    if (river != NULL) {
        int e;
        size_t lat = pa_simple_get_latency(river,&e);
        if (lat != 0 && e == PA_OK)
            return sampling_rate*lat/1000;
        else if (e != PA_OK)
            return sampling_rate/10;
        else if (lat == 0)
            std::cerr << "Faster-than-light electrons detected in your computer (estimated PulseAudio latency = 0).\nResorting to default buffer size...\n";
    }
    return sampling_rate/10;
}

int pulseaudio_backend::executable(pa_simple* river, bool* run, sb::Synth* syn, size_t srate) {
    int e;
    size_t len, lat = pa_simple_get_latency(river,&e);
    if (lat != 0 && e == PA_OK)
        len = srate*lat/1000;
    else
        len = srate*100;
    float* frames = new float[len*2];

    while(*run) {
        pa_simple_drain(river,&e);
        for (float* walker = frames; walker < (frames + len*2); walker += 2)
            syn->tick(walker,2);
        pa_simple_write(river,reinterpret_cast<float*>(frames),len*2*sizeof(float),&e);
    }

    delete [] frames;
    return 0;
}

void pulseaudio_backend::start() {
    int e;
    river = pa_simple_new(NULL, "Soundbench", PA_STREAM_PLAYBACK, NULL, "Audio Output", specs, NULL, NULL, &e);
    if (e != PA_OK)
        throw sbError("emitter::backend::pulseaudio",pa_strerror(e));

    *run = true;
    tread = new std::thread(pulseaudio_backend::executable,river,run,this->syn,sampling_rate);
}

void pulseaudio_backend::stop() {
    *run = false;
    if (tread != NULL)
        tread->join();
}
