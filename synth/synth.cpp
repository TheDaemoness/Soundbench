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
#include <QThread>

namespace sb {

    Synth::Synth() {
        inactivechans = 0;
        currentpoly = DefaultPolyphony;
        notes.resize(DefaultPolyphony);
        for (size_t i = 0; i < DefaultPolyphony; ++i) {
            notes[i].amp = 0.0;
            notes[i].noteoffset = 0;
            notes[i].pedal = NoPedal;
        }
        holdped = false;
        sustped = false;

        if(QThread::idealThreadCount() < 2)
            tlevel = ThreadingNone;
        else if(QThread::idealThreadCount() < InternalChannels)
            tlevel = ThreadingNone;
        else if(QThread::idealThreadCount() == InternalChannels)
            tlevel = ThreadingChannelwise;
        else if(QThread::idealThreadCount() > InternalChannels)
            tlevel = ThreadingChannelwise;

        for (size_t ate = 0; ate < InternalChannels; ++ate) {
            gener[ate] = nullptr;
            for (size_t nonsense = 0; nonsense < FxPerChannel; ++ nonsense)
                eff[ate][nonsense] = nullptr;
        }
    }

    void Synth::setPolyphony(uint8_t poly) {
        for (uint8_t ism = 0; ism < InternalChannels; ++ism) {
            if (gener[ism] != nullptr)
                gener[ism]->setPolymorphism(poly);
        }
        notes.resize(poly);
    }

    void Synth::updateSamplingRate() {
        for (size_t osity = 0; osity < InternalChannels; ++osity) { //Loop per internal channel.
            if (gener[osity] != nullptr)
                gener[osity]->updateSamplingRate();
            for (size_t is_not_a_word = 0; is_not_a_word < FxPerChannel; ++is_not_a_word) { //Loop per effect.
                if (eff[osity][is_not_a_word] != nullptr)
                    eff[osity][is_not_a_word]->updateSamplingRate();
            }
        }
    }

    void Synth::noteOn(int halfsteps, SbSample amp) {
        if (amp <= SbSampleZero)
            return;
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].amp == SbSampleZero && notes[i].pedal == NoPedal) {
                if (gener[i] != nullptr) {
                    if(!gener[i]->finished(i))
                        continue;
                }

                notes[i].noteoffset = halfsteps;
                notes[i].amp = amp;
                if(holdped)
                    notes[i].pedal = Hold;
                else
                    notes[i].pedal = NoPedal;
                for (size_t ation = 0; ation < InternalChannels; ++ation) {
                    if (gener[ation] != nullptr)
                        gener[ation]->noteOn(halfsteps,amp,i);
                }
                break;
            } //Possibly valid note detection.
        }
    }

    void Synth::noteOff(int halfsteps) {
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].noteoffset == halfsteps && notes[i].amp != SbSampleZero) {
                notes[i].amp = SbSampleZero;
                if (notes[i].pedal == NoPedal) {
                    for (size_t al = 0; (al < InternalChannels); ++al) {
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
                if (notes[i].amp != SbSampleZero || notes[i].pedal == Sustenuto)
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
                    if (notes[i].amp == SbSampleZero) {
                        for (size_t al = 0; al < InternalChannels; ++al) {
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
            if(notes[i].amp != SbSampleZero) {
                notes[i].amp = SbSampleZero;
                notes[i].pedal = NoPedal;
                for (size_t able = 0; able < InternalChannels; ++able) {
                    if (gener[able] != nullptr)
                        gener[able]->noteOff(i);
                }
            }
        }
        for (size_t ec = 0; ec < InternalChannels; ++ec) { //Loop per internal channel.
            if (gener[ec] != nullptr)
                gener[ec]->reset();
            /*for (size_t tive = 0; tive < fxcount; ++tive) { //Loop per effect.
                if (eff[ec][tive] != nullptr) //I could make these jokes all day.
                    eff[ec][tive]->reset();
            }*/
        }
    }
}

