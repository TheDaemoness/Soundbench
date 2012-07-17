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

#include "frontend/rtmidi.h"

#include <deque>
#include <cstdio>
#include <thread>

#include <QListWidget>

namespace sb {

    class Player : public QObject {
        Q_OBJECT

    public:
        explicit Player(Synth* syn, QListWidget* tracklist, CpuMeter* themet);
        ~Player();

        bool loadTrack(uint16_t track);
        std::vector<std::string> getPorts();
        size_t getCurrentPort();
        bool setVirtualPort(bool vport);

        inline void close() {
            reed->close();
            first->chainDestroy();
        }
        inline bool isOpen() {
            return reed->isOpen();
        }
        inline bool empty() {
            return first->returnNext() == nullptr;
        }
        inline void setFile(std::string thefile) {
            fi = thefile;
        }
        inline bool setTempo(int tmp) {
            return reed->setTempo(tmp);
        }
        inline int getTempo() {
            return reed->getTempo();
        }
        inline void setPort(size_t porter) {
            if(midin != nullptr)
                midin->setPort(porter);
        }
        inline bool isRtAvailable() {
            return midin != nullptr;
        }

        void startPlay();
        void stopPlay();
        void startRt();
        void stopRt();
        void startRec();
        void stopRec();

        void initfrontend(Synth* syn);

    signals:
        void progressed(int gression);
        void donePlaying();

    public slots:
        void writeFile();
        bool loadFile();

    private slots:
        void cleanThread() {
            playthread->join();
            delete playthread;
        }

    private:
        std::deque<midi::nodes::MIDIEventNode> nodes;
        std::map<FrontendType,bool> supported_apis;
        time_t midiclocklen;
        std::string fi;
        bool playing;

        midi::MidiFIO* reed;
        midi::nodes::PlayerStartNode* first;
        SoundFileWriter* wri;
        MidiFrontend* midin;
        std::thread* playthread;

        QListWidget* affectedlist;
        CpuMeter* affectedmet;

        static void playChain(Player*);
    };
}

#endif // PLAYER_H
