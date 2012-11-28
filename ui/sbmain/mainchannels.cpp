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

#include "soundbenchmain.h"


void SoundbenchMain::setGenType(int which) {
    switch (which) {
    case 0:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener1TypeBox->currentIndex());
        ui->gen1SettButton->setEnabled(blu->gener[which] != sb::NoGener);
        break;
    case 1:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener2TypeBox->currentIndex());
        ui->gen2SettButton->setEnabled(blu->gener[which] != sb::NoGener);
        break;
    case 2:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener3TypeBox->currentIndex());
        ui->gen3SettButton->setEnabled(blu->gener[which] != sb::NoGener);
        break;
    case 3:
        blu->gener[which] = static_cast<sb::GenerType>(ui->gener4TypeBox->currentIndex());
        ui->gen4SettButton->setEnabled(blu->gener[which] != sb::NoGener);
        break;
    default:
        std::cerr << "A generator type-changing function was called by some random widget.\n";
        return;
    }
    stopAndReset();
    arch->buildSynth(syn,blu);
    em->start();
}

void SoundbenchMain::setGenSett(int which) {
    stopAndReset();
    met->pauseMeter();
    genSetts(which);
    arch->buildSynth(syn,blu);
    syn->reset();
    met->startMeter();
    em->start();
}

void SoundbenchMain::updateChannelEffectInfo(int chan) {
    switch (chan) {
    case 0:
        ui->fx1SettButton->setEnabled(blu->eff[chan][ui->fxNumberBox1->value()-1] != sb::NoFx);
        ui->fx1Feedback->setValue(blu->eff_data[chan][ui->fxNumberBox1->value()-1][sb::EffectFeedback].pod.sample);
        ui->fx1TypeBox->setCurrentIndex(blu->eff[chan][ui->fxNumberBox1->value()-1]);
        break;
    case 1:
        ui->fx2SettButton->setEnabled(blu->eff[chan][ui->fxNumberBox2->value()-1] != sb::NoFx);
        ui->fx2Feedback->setValue(blu->eff_data[chan][ui->fxNumberBox2->value()-1][sb::EffectFeedback].pod.sample);
        ui->fx2TypeBox->setCurrentIndex(blu->eff[chan][ui->fxNumberBox2->value()-1]);
        break;
    case 2:
        ui->fx3SettButton->setEnabled(blu->eff[chan][ui->fxNumberBox3->value()-1] != sb::NoFx);
        ui->fx3Feedback->setValue(blu->eff_data[chan][ui->fxNumberBox3->value()-1][sb::EffectFeedback].pod.sample);
        ui->fx3TypeBox->setCurrentIndex(blu->eff[chan][ui->fxNumberBox3->value()-1]);
        break;
    case 3:
        ui->fx4SettButton->setEnabled(blu->eff[chan][ui->fxNumberBox4->value()-1] != sb::NoFx);
        ui->fx4Feedback->setValue(blu->eff_data[chan][ui->fxNumberBox4->value()-1][sb::EffectFeedback].pod.sample);
        ui->fx4TypeBox->setCurrentIndex(blu->eff[chan][ui->fxNumberBox4->value()-1]);
        break;
    default:
        std::cerr << "Some widget is confused about how many internal channels there are.\n";
        return;
    }
}
void SoundbenchMain::setEffectFeedback(int chan) {
    switch (chan) {
    case 0:
        blu->eff_data[chan][ui->fxNumberBox1->value()-1][sb::EffectFeedback] = sb::makeParamfromSample(ui->fx1Feedback->value());
        break;
    case 1:
        blu->eff_data[chan][ui->fxNumberBox2->value()-1][sb::EffectFeedback] = sb::makeParamfromSample(ui->fx2Feedback->value());
        break;
    case 2:
        blu->eff_data[chan][ui->fxNumberBox3->value()-1][sb::EffectFeedback] = sb::makeParamfromSample(ui->fx3Feedback->value());
        break;
    case 3:
        blu->eff_data[chan][ui->fxNumberBox4->value()-1][sb::EffectFeedback] = sb::makeParamfromSample(ui->fx4Feedback->value());
        break;
    default:
        std::cerr << "Feedback cannot be set for an effect that has no possibility of existing.\n";
        return;
    }
}

void SoundbenchMain::channelEffectSettings(int chan) {
    stopAndReset();
    met->pauseMeter();
    if(chan == 0)
        fxSetts(chan, ui->fxNumberBox1->value()-1);
    else if(chan == 1)
        fxSetts(chan, ui->fxNumberBox2->value()-1);
    else if(chan == 2)
        fxSetts(chan, ui->fxNumberBox3->value()-1);
    else if(chan == 3)
        fxSetts(chan, ui->fxNumberBox4->value()-1);
    else
        std::cerr << "Something tried to change the settings for an effect in a channel that doesn't exist.\n";
    arch->buildSynth(syn,blu);
    syn->reset();
    met->startMeter();
    em->start();
}


void SoundbenchMain::changeChannelEffect(int chan) {
    switch (chan) {
    case 0:
        blu->eff[chan][ui->fxNumberBox1->value()-1] = static_cast<sb::FxType>(ui->fx1TypeBox->currentIndex());
        break;
    case 1:
        blu->eff[chan][ui->fxNumberBox2->value()-1] = static_cast<sb::FxType>(ui->fx2TypeBox->currentIndex());
        break;
    case 2:
        blu->eff[chan][ui->fxNumberBox3->value()-1] = static_cast<sb::FxType>(ui->fx3TypeBox->currentIndex());
        break;
    case 3:
        blu->eff[chan][ui->fxNumberBox4->value()-1] = static_cast<sb::FxType>(ui->fx4TypeBox->currentIndex());
        break;
    default:
        std::cerr << "An effect type-changing function was called by some random widget.\n";
        return;
    }
    updateChannelEffectInfo(chan);
}

void SoundbenchMain::genSetts(size_t i) {
    if (blu->gener[i] == sb::GenerBasic) {
        gsd.basic = new BasicGenerSettings(i,blu);
        gsd.basic->exec();
        delete gsd.basic;
    }
    else
        std::cerr << "A request for an unimplemented generator's settings was made. Ignoring...\n";
}

void SoundbenchMain::fxSetts(size_t chan, size_t nel) {
    if(blu->eff[chan][nel] == sb::FxBiquad)
        std::cerr << "Biquad settings interface not yet implemented.\n";
    else
        std::cerr << "A request for an unimplemented effect's settings was made. Ignoring...\n";
}
