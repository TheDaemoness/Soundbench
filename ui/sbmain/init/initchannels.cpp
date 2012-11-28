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

#include "ui/sbmain/soundbenchmain.h"

void SoundbenchMain::initChannelsPage() {
    connect(ui->gener1TypeBox,SIGNAL(currentIndexChanged(int)),gen_type_sigmap,SLOT(map()));
    connect(ui->gener2TypeBox,SIGNAL(currentIndexChanged(int)),gen_type_sigmap,SLOT(map()));
    connect(ui->gener3TypeBox,SIGNAL(currentIndexChanged(int)),gen_type_sigmap,SLOT(map()));
    connect(ui->gener4TypeBox,SIGNAL(currentIndexChanged(int)),gen_type_sigmap,SLOT(map()));

    connect(ui->gen1SettButton,SIGNAL(clicked()),gen_sett_sigmap,SLOT(map()));
    connect(ui->gen2SettButton,SIGNAL(clicked()),gen_sett_sigmap,SLOT(map()));
    connect(ui->gen3SettButton,SIGNAL(clicked()),gen_sett_sigmap,SLOT(map()));
    connect(ui->gen4SettButton,SIGNAL(clicked()),gen_sett_sigmap,SLOT(map()));

    connect(ui->fxNumberBox1,SIGNAL(valueChanged(int)),fx_num_sigmap,SLOT(map()));
    connect(ui->fxNumberBox2,SIGNAL(valueChanged(int)),fx_num_sigmap,SLOT(map()));
    connect(ui->fxNumberBox3,SIGNAL(valueChanged(int)),fx_num_sigmap,SLOT(map()));
    connect(ui->fxNumberBox4,SIGNAL(valueChanged(int)),fx_num_sigmap,SLOT(map()));

    connect(ui->fx1SettButton,SIGNAL(clicked()),fx_sett_sigmap,SLOT(map()));
    connect(ui->fx2SettButton,SIGNAL(clicked()),fx_sett_sigmap,SLOT(map()));
    connect(ui->fx3SettButton,SIGNAL(clicked()),fx_sett_sigmap,SLOT(map()));
    connect(ui->fx4SettButton,SIGNAL(clicked()),fx_sett_sigmap,SLOT(map()));

    connect(ui->fx1TypeBox,SIGNAL(currentIndexChanged(int)),fx_type_sigmap,SLOT(map()));
    connect(ui->fx2TypeBox,SIGNAL(currentIndexChanged(int)),fx_type_sigmap,SLOT(map()));
    connect(ui->fx3TypeBox,SIGNAL(currentIndexChanged(int)),fx_type_sigmap,SLOT(map()));
    connect(ui->fx4TypeBox,SIGNAL(currentIndexChanged(int)),fx_type_sigmap,SLOT(map()));

    connect(ui->fx1Feedback,SIGNAL(valueChanged(double)),fx_fb_sigmap,SLOT(map()));
    connect(ui->fx2Feedback,SIGNAL(valueChanged(double)),fx_fb_sigmap,SLOT(map()));
    connect(ui->fx3Feedback,SIGNAL(valueChanged(double)),fx_fb_sigmap,SLOT(map()));
    connect(ui->fx4Feedback,SIGNAL(valueChanged(double)),fx_fb_sigmap,SLOT(map()));

    //Connect the sigmaps.
    connect(gen_sett_sigmap,SIGNAL(mapped(int)),SLOT(setGenSett(int)));
    connect(gen_type_sigmap,SIGNAL(mapped(int)),SLOT(setGenType(int)));
    connect(rate_sigmap,SIGNAL(mapped(int)),SLOT(setSampleRate(int)));
    connect(emit_sigmap,SIGNAL(mapped(int)),SLOT(changeAudioAPI(int)));
    connect(fx_num_sigmap,SIGNAL(mapped(int)),SLOT(updateChannelEffectInfo(int)));
    connect(fx_sett_sigmap,SIGNAL(mapped(int)),SLOT(channelEffectSettings(int)));
    connect(fx_type_sigmap,SIGNAL(mapped(int)),SLOT(changeChannelEffect(int)));
    connect(fx_fb_sigmap,SIGNAL(mapped(int)),SLOT(setEffectFeedback(int)));
}
