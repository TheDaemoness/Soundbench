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
        for (size_t i = 0; i < cracker; ++i)
            ocean.push_back(std::make_pair(false,new Sine(sample_rate)));
    }
    void BasicGen::ctrl(ModuleParams arg, ParameterValue val) {
        switch (arg) {
        case GenBasicWave:
            curr_wav = static_cast<SimpleWaveTypes>(val.value);
            for (BoolWavePair& i : ocean) {
                delete i.second;

                if (static_cast<SimpleWaveTypes>(val.value) == TriangleWave)
                    i.second = new Triangle(SampleRate);
                else if (static_cast<SimpleWaveTypes>(val.value) == SquareWave)
                    i.second = new Square(SampleRate);
                else if (static_cast<SimpleWaveTypes>(val.value) == SawtoothWave)
                    i.second = new Sawtooth(SampleRate);
                else if (static_cast<SimpleWaveTypes>(val.value) == OvalWave)
                    i.second = new Oval(SampleRate);
                else
                    i.second = new Sine(SampleRate);
            }
            break;
        case GenBasicAmp:
            gen_amp = val.sample;
            break;
        case GenBasicNoteBias:
            notebias = val.value;
            break;
        case GenBasicPhase:
            for (BoolWavePair& i : ocean)
                i.second->setOffset(val.sample*2*Pi);
            break;
        default:
            //Ignore it. The blueprints have values for other generators too.
            break;
        }
    }
    void BasicGen::noteOn(int halfsteps, SbSample ampl, size_t pos) {
        if(pos >= ocean.size())
            return;
        ocean[pos].second->setFrequency(getFrequencyFromNote(halfsteps + notebias));
        ocean[pos].second->setAmplitude(ampl);
        ocean[pos].first = true;
        ++notes;
    }
    void BasicGen::noteOff(size_t pos) {
        if(pos >= ocean.size())
            return;
        ocean[pos].first = false;
        --notes;
    }
    void BasicGen::reset() {
        for (BoolWavePair& i : ocean) {
            i.first = false;
            i.second->reset();
        }
    }
    void BasicGen::setPolymorphism(size_t poly) {
        if (ocean.size() == poly)
            return;
        for (BoolWavePair i : ocean)
            delete i.second;

        ocean.resize(poly);

        for (BoolWavePair& i : ocean) {
            i.first = false;
            if (curr_wav == TriangleWave)
                i.second = new Triangle(SampleRate);
            else if (curr_wav == SquareWave)
                i.second = new Square(SampleRate);
            else if (curr_wav == SawtoothWave)
                i.second = new Sawtooth(SampleRate);
            else if (curr_wav == OvalWave)
                i.second = new Oval(SampleRate);
            else
                i.second = new Sine(SampleRate);
        }

    }
    void BasicGen::tick(float *sample, size_t chans) {
        //Cummulate all the samples from each active wave into the same float.
        sample[0] = 0.0f;
        for (size_t i = 0; i < ocean.size(); ++i)
            sample[0] += (ocean[i].first?ocean[i].second->tick():0.0)*gen_amp;

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
