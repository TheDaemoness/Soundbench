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

#include "geners.h"

namespace sb {

    BasicGen::BasicGen(size_t srate, size_t cracker) {
        wav = new Sine(srate);
        notes = 0;
        sample_rate = srate;
        gen_amp = sbSampleMax;
        noteamps.reserve(default_poly);
        noteposs.reserve(default_poly);
        notespds.reserve(default_poly);
        for (size_t i = 0; i < cracker; ++i) {
            noteamps.push_back(sbSampleZero);
            noteposs.push_back(0.0);
            notespds.push_back(0);
        }
    }
    void BasicGen::ctrl(moduleParam arg, ParameterValue val) {
         switch (arg) {
         case genBasic_wave:
             delete wav;
             if (static_cast<SimpleWaveTypes>(val.value) == TriangleWave)
                 wav = new Triangle(curr_srate);
             else if (static_cast<SimpleWaveTypes>(val.value) == SquareWave)
                wav = new Square(curr_srate);
             else if (static_cast<SimpleWaveTypes>(val.value) == SawtoothWave)
                wav = new Sawtooth(curr_srate);
             else
                wav = new Sine(curr_srate);
             break;
         case genBasic_amplutide:
             gen_amp = val.sample;
             break;
         case genBasic_notebias:
             notebias = val.value;
             break;
         case genBasic_phase:
             for (double& ntepos: noteposs)
                 ntepos = val.sample*2.0*pi;
             break;
         default:
             //Ignore it. The blueprints have values for other generators too.
             break;
         }
     }
     void BasicGen::noteOn(int halfsteps, sbSample ampl, size_t pos) {
         if(pos < notespds.size()) {
             notespds[pos] = getFrequencyFromNote(halfsteps+notebias);
             noteamps[pos] = ampl;
         }
         ++notes;
     }
     void BasicGen::noteOff(size_t pos) {
         if(pos < notespds.size()) {
             notespds[pos] = 0.0;
             noteposs[pos] = offset*2.0*pi;
             noteamps[pos] = 0.0;
         }
         --notes;
     }
     void BasicGen::reset() {
         for (sbSample& amp : noteamps)
             amp = 0.0;
         for (double& marcher : noteposs)
             marcher = (offset*2.0*pi);
     }
     void BasicGen::setPolymorphism(size_t poly) {
         reset();
         size_t olde = notespds.size();
         noteposs.resize(poly);
         noteamps.resize(poly);
         notespds.resize(poly);
         for (size_t i = olde; i < poly; ++i) {
             noteamps[i] = sbSampleZero;
             noteposs[i] = 0;
             notespds[i] = 0;
         }
     }
     void BasicGen::tick(float *sample, size_t chans) {
         sample[0] = 0.0f;
         for (size_t i = 0; i < notespds.size(); ++i) {
             if (notespds[i] != 0.0) {
                noteposs[i] = std::fmod(noteposs[i],2.0*pi);
                sample[0] += wav->getRaw(noteposs[i])*noteamps[i]*gen_amp;
                noteposs[i] += 2.0*pi*(notespds[i]/sample_rate);
             }
         }
         if (notes > 0)
            sample[0] /= notes;

         for (size_t i = 0; i < chans; ++i) {
            sample[i] = sample[0];
         }
     }
}
