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

#include "gener/basic.h"

namespace sb {

    BasicGen::BasicGen(size_t cracker) {
        notebias = 0;
        gen_amp = SbSampleMax;
        currpoly = cracker;
        curr_wav = SineWave;

        ocean = new PeriodicSampleTable(new Sine(global_srate),cracker);
        envelope.resize(cracker);
    }
    void BasicGen::ctrl(ModuleParams arg, ParameterValue val) {
        switch (arg) {
        case GenBasicWave:
            curr_wav = static_cast<SimpleWaveTypes>(val.pod.value);
            delete ocean;

            WaveBase* w;
            if (curr_wav == TriangleWave)
                w = new Triangle(global_srate);
            else if (curr_wav == SquareWave)
                w = new Square(global_srate);
            else if (curr_wav == SawtoothWave)
                w = new Sawtooth(global_srate);
            else if (curr_wav == OvalWave)
                w = new Oval(global_srate);
            else if (curr_wav == PeakWave)
                w = new Peak(global_srate);
            else
                w = new Sine(global_srate);
            ocean = new PeriodicSampleTable(w, currpoly);
            ocean->setOffsets(offset);
            break;
        case GenBasicAmp:
            gen_amp = val.pod.sample;
            break;
        case GenBasicNoteBias:
            notebias = val.pod.value;
            break;
        case GenBasicPhase:
            offset = val.pod.sample/SbSampleMax*global_srate;
            ocean->setOffsets(offset);
            break;
        case GenerAttackTime:
            for(Trapezoid& trap : envelope)
                trap.setAttackTime(val.pod.decim);
            break;
        case GenerReleaseTime:
            for(Trapezoid& trap : envelope)
                trap.setReleaseTime(val.pod.decim);
            break;
        default:
            //Ignore it. The blueprints have values for other generators too.
            break;
        }
    }
    void BasicGen::noteOn(int halfsteps, SbSample ampl, size_t pos) {
        if(pos >= envelope.size())
            return;
        ocean->setWave(getFrequencyFromNote(halfsteps+notebias),ampl,pos);
        envelope[pos].attack();
    }
    void BasicGen::noteOff(size_t pos) {
        if(pos >= envelope.size())
            return;
        envelope[pos].release();
    }
    void BasicGen::reset() {
        ocean->setOffsets(offset);
        for(Trapezoid& trap : envelope) {
            trap.reset();
        }
    }
    void BasicGen::setPolymorphism(size_t poly) {
        if (currpoly == poly)
            return;
        currpoly = poly;
        reset();
        envelope.resize(poly);
        ocean->setWaveCount(poly);
    }
    void BasicGen::tick(float *sample, size_t chans, bool) {
        //Cummulate all the samples from each active wave into the same float.
        sample[0] = 0.0f;
        for (size_t i = 0; i < currpoly; ++i) {
            if (envelope[i].tick() != SbSampleZero) {
                sample[0] += ocean->tick(i)*gen_amp*envelope[i].getLevel();
            }
        }

        //Bring the volume back in range.
        sample[0] /= currpoly;

        //Copy the samples to all the other channels.
        for (size_t i = 0; i < chans; ++i) {
            sample[i] = sample[0];
        }
    }
    void BasicGen::updateSamplingRate() {
        reset();
        ctrl(GenBasicWave,makeParamfromInt(curr_wav));
    }
}
