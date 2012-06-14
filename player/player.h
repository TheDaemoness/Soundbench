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

#ifndef PLAYER_H
#define PLAYER_H

#include "midichain.h"
#include "midifio/midifio.h"
#include "cpumeter.h"
#include "sfwriter.h"

#include "frontend/portmidi.h"

#include <deque>
#include <cstdio>

#include <QListWidget>

namespace sb {

    class Player : public QObject {
        Q_OBJECT
    private:
        bool isready;
        std::deque<midi::MIDIEventNode> nodes;
        time_t midiclocklen;
        std::string fi;

        midi::MidiFIO* reed;
        midi::PlayerStartNode* first;
        SoundFileWriter* wri;
        MidiFrontend* midin;

        QListWidget* affectedlist;
        CpuMeter* affectedmet;

    public:
        explicit Player(Synth* syn, QListWidget* tracklist, CpuMeter* themet) {
            affectedlist = tracklist;
            affectedmet = themet;
            isready = false;

            reed = new midi::MidiFIO;
            first = new midi::PlayerStartNode(syn);
            wri = new SoundFileWriter(syn);

            connect(this,SIGNAL(progressed(int)),affectedmet,SLOT(setProgress(int)));
        }
        ~Player() {
            disconnect(SIGNAL(progressed(int)),affectedmet,SLOT(setProgress(int)));
            delete first;
            delete reed;
            delete wri;
        }

        bool loadTrack(uint16_t track);

        inline bool setTempo(int tmp) {
            return reed->setTempo(tmp);
        }
        inline int getTempo() {
            return reed->getTempo();
        }

        void play() {}
        void stop() {}
        void initfrontend();
        bool ready() {
            return isready;
        }
        void setFile(std::string thefile) {
            fi = thefile;
        }

    signals:
        void progressed(int gression);

    public slots:
        void writeFile();
        bool loadFile();
    };
}

#endif // PLAYER_H
