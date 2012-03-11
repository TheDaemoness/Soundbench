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

#include "synth.h"

namespace sb {

    Synth::Synth() {
        inactivechans = 0;
        notes.resize(default_poly);
        for (size_t i = 0; i < default_poly; ++i) {
            notes[i].first = 0.0;
            notes[i].second = 0;
        }
    }

    void Synth::noteOn(int halfsteps, sbSample amp) {
        if (amp <= 0.0)
            return;
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].second == sbSampleZero) {
                notes[i].first = halfsteps;
                notes[i].second = amp;
                for (size_t ation = 0; ation < channelcount; ++ation) {
                    if (gener[ation] != NULL) //Older programmers: This does NOT unconditionally evaluate to false.
                        gener[ation]->noteOn(halfsteps,amp,i);
                }
                break;
            }
        }
    }
    void Synth::noteOff(int halfsteps) {
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].first == halfsteps && notes[i].second != sbSampleZero) {
                notes[i].second = sbSampleZero;
                for (size_t al = 0; al < channelcount; ++al) {
                    if (gener[al] != NULL)
                        gener[al]->noteOff(i);
                }
            }
        }
    }
    void Synth::reset() {
        for (size_t i = 0; i < notes.size(); ++i) {
            if(notes[i].second != sbSampleZero) {
                notes[i].second = sbSampleZero;
                for (size_t able = 0; able < channelcount; ++able) {
                    if (gener[able] != NULL)
                        gener[able]->noteOff(i);
                }
            }
        }
        for (size_t ec = 0; ec < channelcount; ++ec) { //Loop per internal channel.
            if (gener[ec] != NULL)
                gener[ec]->reset();
            /*for (size_t tive = 0; tive < fxcount; ++tive) { //Loop per effect.
                if (eff[ec][tive] != NULL) //I could make these jokes all day.
                    eff[ec][tive]->reset();
            }*/
        }
    }
    void Synth::tick(sbSample* frames, size_t chans) {
        for (size_t ic = 0; ic < channelcount; ++ic) { //Loop per internal channel.
            if (gener[ic] != NULL)
                gener[ic]->tick(buffer[ic], chans);
            else {
                for (size_t i = 0; i < chans; ++i)
                    buffer[ic][i] = sbSampleZero;
            }
            for (size_t ient = 0; ient < fxcount; ++ient) { //Loop per effect.
                if (eff[ic][ient] != NULL)
                    eff[ic][ient]->tick(buffer[ient],chans);
            }
            for (size_t acid = 0; acid < chans; ++acid) { //Loop per outbound channel.
                if (!ic)
                    frames[acid] = sbSampleZero;
                frames[acid] += buffer[ic][acid]; //Bufferic acid. What an incorrect name that'd be.
            }
        }
        if (inactivechans != channelcount) {
            for (size_t out = 0; out < chans; ++out) {
                frames[out] /= (channelcount-inactivechans); //Correctly average the signal from the running channels.
                frames[out] *= vol; //Apply the master volume.
            }
        }

    }
}
