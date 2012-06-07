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

#include "gener_basic.h"

namespace sb {

    BasicGen::BasicGen(size_t srate, size_t cracker) {
        notes = 0;
        notebias = 0;
        sample_rate = srate;
        gen_amp = SbSampleMax;
        ocean.reserve(cracker);
        envelope.resize(cracker);
        for (size_t i = 0; i < cracker; ++i)
            ocean.push_back(new Sine(sample_rate));
    }
    void BasicGen::ctrl(ModuleParams arg, ParameterValue val) {
        switch (arg) {
        case GenBasicWave:
            curr_wav = static_cast<SimpleWaveTypes>(val.value);
            for (auto i : ocean) {
                delete i;

                if (static_cast<SimpleWaveTypes>(val.value) == TriangleWave)
                    i = new Triangle(SampleRate);
                else if (static_cast<SimpleWaveTypes>(val.value) == SquareWave)
                    i = new Square(SampleRate);
                else if (static_cast<SimpleWaveTypes>(val.value) == SawtoothWave)
                    i = new Sawtooth(SampleRate);
                else if (static_cast<SimpleWaveTypes>(val.value) == OvalWave)
                    i = new Oval(SampleRate);
                else
                    i = new Sine(SampleRate);
            }
            break;
        case GenBasicAmp:
            gen_amp = val.sample;
            break;
        case GenBasicNoteBias:
            notebias = val.value;
            break;
        case GenBasicPhase:
            for (auto i : ocean)
                i->setOffset(val.sample*2*Pi);
            break;
        case GenerAttackTime:
            for(Trapezoid& trap : envelope)
                trap.setAttackTime(val.decim);
            break;
        case GenerReleaseTime:
            for(Trapezoid& trap : envelope)
                trap.setReleaseTime(val.decim);
            break;
        default:
            //Ignore it. The blueprints have values for other generators too.
            break;
        }
    }
    void BasicGen::noteOn(int halfsteps, SbSample ampl, size_t pos) {
        if(pos >= ocean.size())
            return;
        ocean[pos]->setFrequency(getFrequencyFromNote(halfsteps + notebias));
        ocean[pos]->setAmplitude(ampl);
        envelope[pos].attack();
        ++notes;
    }
    void BasicGen::noteOff(size_t pos) {
        if(pos >= ocean.size())
            return;
        envelope[pos].release();
        --notes;
    }
    void BasicGen::reset() {
        for (auto& i : ocean) {
            i->reset();
        }
        for(Trapezoid trap : envelope)
            trap.reset();
    }
    void BasicGen::setPolymorphism(size_t poly) {
        if (ocean.size() == poly)
            return;
        for (auto i : ocean)
            delete i;
        for (Trapezoid& trap : envelope)
            trap.reset();

        envelope.resize(poly);
        ocean.resize(poly);

        for (auto i : ocean) {
            if (curr_wav == TriangleWave)
                i = new Triangle(SampleRate);
            else if (curr_wav == SquareWave)
                i = new Square(SampleRate);
            else if (curr_wav == SawtoothWave)
                i = new Sawtooth(SampleRate);
            else if (curr_wav == OvalWave)
                i = new Oval(SampleRate);
            else
                i = new Sine(SampleRate);
        }
    }
    void BasicGen::tick(float *sample, size_t chans) {
        //Cummulate all the samples from each active wave into the same float.
        sample[0] = 0.0f;
        for (size_t i = 0; i < ocean.size(); ++i) {
            if (envelope[i].tick() != SbSampleZero) {
                sample[0] += (ocean[i]?ocean[i]->tick():0.0)*gen_amp*envelope[i].getLevel();
            }
        }

        //Bring the volume back in range.
        if (notes > 0)
            sample[0] /= notes;

        //std::cerr << sample[0] << '\t';

        //Copy the samples to all the other channels.
        for (size_t i = 0; i < chans; ++i) {
            sample[i] = sample[0];
        }
    }
}
