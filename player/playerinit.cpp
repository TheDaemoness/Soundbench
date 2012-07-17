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

#include "player.h"

namespace sb {
    Player::Player(Synth* syn, QListWidget* tracklist, CpuMeter* themet) {
        affectedlist = tracklist;
        affectedmet = themet;

        reed = new midi::MidiFIO;
        first = new midi::nodes::PlayerStartNode(syn);
        wri = new SoundFileWriter(syn);

        initfrontend(syn);

        connect(this,SIGNAL(progressed(int)),affectedmet,SLOT(setProgress(int)));
    }
    Player::~Player() {
        disconnect(SIGNAL(progressed(int)),affectedmet,SLOT(setProgress(int)));
        if (first != nullptr)
            delete first;
        delete reed;
        delete wri;
    }
}
