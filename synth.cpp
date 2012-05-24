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
        currentpoly = default_poly;
        notes.resize(default_poly);
        for (size_t i = 0; i < default_poly; ++i) {
            notes[i].amp = 0.0;
            notes[i].noteoffset = 0;
            notes[i].pedal = NoPedal;
        }
        for (size_t i = 0; i < outchans; ++i)
            prevsample[i] = sbSampleZero;
        holdped = false;
        sustped = false;
    }

    void Synth::setPolyphony(uint8_t poly) {
        for (uint8_t cracker = 0; cracker < poly; ++cracker)
        if (gener[cracker] != nullptr)
            gener[cracker]->setPolymorphism(poly);
        notes.resize(poly);
    }

    void Synth::noteOn(int halfsteps, sbSample amp) {
        if (amp <= sbSampleZero)
            return;
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].amp == sbSampleZero && notes[i].pedal == NoPedal) {
                notes[i].noteoffset = halfsteps;
                notes[i].amp = amp;
                if(holdped)
                    notes[i].pedal = Hold;
                else
                    notes[i].pedal = NoPedal;
                for (size_t ation = 0; ation < internchannels; ++ation) {
                    if (gener[ation] != nullptr)
                        gener[ation]->noteOn(halfsteps,amp,i);
                }
                break;
            }
        }
    }

    void Synth::noteOff(int halfsteps) {
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].noteoffset == halfsteps && notes[i].amp != sbSampleZero) {
                notes[i].amp = sbSampleZero;
                if (notes[i].pedal == NoPedal) {
                    for (size_t al = 0; (al < internchannels); ++al) {
                        if (gener[al] != nullptr)
                            gener[al]->noteOff(i);
                    }
                }
            }
        }
    }

    void Synth::pedal(SupportedPedals which, bool val) {
        if (val) {
            if (which == Hold)
                holdped = true;
            else if (which == Sustenuto)
                sustped = true;
            for (size_t i = 0; i < notes.size(); ++i) {
                if (notes[i].amp != sbSampleZero || notes[i].pedal == Sustenuto)
                    notes[i].pedal = which;
            }
        }
        else {
            if (which == Hold)
                holdped = false;
            else if (which == Sustenuto)
                sustped = false;
            for (size_t i = 0; i < notes.size(); ++i) {
                if (notes[i].pedal == which) {
                    notes[i].pedal = NoPedal;
                    if (notes[i].amp == sbSampleZero) {
                        for (size_t al = 0; al < internchannels; ++al) {
                            if (gener[al] != nullptr)
                                gener[al]->noteOff(i);
                        }
                    }
                }
            }
        }
    }


    void Synth::reset() {
        for (size_t i = 0; i < notes.size(); ++i) {
            if(notes[i].amp != sbSampleZero) {
                notes[i].amp = sbSampleZero;
                notes[i].pedal = NoPedal;
                for (size_t able = 0; able < internchannels; ++able) {
                    if (gener[able] != nullptr)
                        gener[able]->noteOff(i);
                }
            }
        }
        for (size_t ec = 0; ec < internchannels; ++ec) { //Loop per internal channel.
            if (gener[ec] != nullptr)
                gener[ec]->reset();
            /*for (size_t tive = 0; tive < fxcount; ++tive) { //Loop per effect.
                if (eff[ec][tive] != nullptr) //I could make these jokes all day.
                    eff[ec][tive]->reset();
            }*/
        }
    }

    void Synth::tick(sbSample* frames, size_t chans) {
        for (size_t ic = 0; ic < internchannels; ++ic) { //Loop per internal channel.
            if (gener[ic] != nullptr)
                gener[ic]->tick(buffer[ic], chans);
            else {
                for (size_t i = 0; i < chans; ++i)
                    buffer[ic][i] = sbSampleZero;
            }
            for (size_t ient = 0; ient < fxcount; ++ient) { //Loop per effect.
                if (eff[ic][ient] != nullptr)
                    eff[ic][ient]->tick(buffer[ient],chans);
            }
            for (size_t acid = 0; acid < chans; ++acid) { //Loop per outbound channel.
                if (!ic)
                    frames[acid] = sbSampleZero;
                frames[acid] += buffer[ic][acid]; //What an incorrect name that'd be.
            }
        }
        if (inactivechans != internchannels) {
            for (size_t out = 0; out < chans; ++out) {
                frames[out] /= (internchannels-inactivechans); //Correctly average the signal from the running channels.
                frames[out] *= vol; //Apply the master volume.

                //Smooth the samples slightly to reduce clicking.
                frames[out] += prevsample[out];
                frames[out] /= 2;
                prevsample[out] = frames[out];
            }
        }
    }
}
