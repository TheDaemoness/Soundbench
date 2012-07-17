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

#include <QListWidget>

namespace sb {

    class Player : public QObject {
        Q_OBJECT
    private:
        std::deque<midi::nodes::MIDIEventNode> nodes;
        std::map<FrontendType,bool> supported_apis;
        time_t midiclocklen;
        std::string fi;

        midi::MidiFIO* reed;
        midi::nodes::PlayerStartNode* first;
        SoundFileWriter* wri;
        MidiFrontend* midin;

        QListWidget* affectedlist;
        CpuMeter* affectedmet;

    public:
        explicit Player(Synth* syn, QListWidget* tracklist, CpuMeter* themet) {
            affectedlist = tracklist;
            affectedmet = themet;

            reed = new midi::MidiFIO;
            first = new midi::nodes::PlayerStartNode(syn);
            wri = new SoundFileWriter(syn);

            initfrontend(syn);

            connect(this,SIGNAL(progressed(int)),affectedmet,SLOT(setProgress(int)));
        }
        ~Player() {
            disconnect(SIGNAL(progressed(int)),affectedmet,SLOT(setProgress(int)));
            if (first != nullptr)
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
        std::vector<std::string> getPorts() {
            if(midin != nullptr)
                return midin->getPorts();
            return std::vector<std::string>();
        }
        size_t getCurrentPort() {
            if (midin != nullptr)
                return midin->getCurrentPort();
            return 0;
        }
        bool setVirtualPort(bool vport) {
            if (midin == nullptr)
                return false;
            if (!midin->supportsVirtualPorts())
                return false;
            midin->setVirtualPort(vport);
            return true;
        }

        inline void close() {
            reed->close();
            first->chainDestroy();
        }
        inline bool isOpen() {
            return reed->isOpen();
        }

        void startPlay() {}
        void stopPlay() {}
        void startRt() {
            if(midin != nullptr)
                midin->start();
        }
        void stopRt() {
            if(midin != nullptr)
                midin->stop();
        }
        void startRec() {
            if(midin != nullptr)
                midin->record(true);
        }
        void stopRec() {
            if(!midin->isRecording())
                return;

            if(midin != nullptr)
                midin->record(false);
            if(first != nullptr)
                delete first;
            first = midin->detachChain();

            uint32_t len = 0;
            for(auto i = first->returnNext(); i != nullptr; i = i->returnNext())
                ++len;
            --len;
            if (len == 0) {
                std::cerr << "No events were recorded.\n";
                first->chainDestroy();
            }
            else if (len == 1)
                std::cerr << "Recorded 1 event.\n";
            else
                std::cerr << "Recorded " << len << " events.\n";
        }
        void setPort(size_t porter) {
            if(midin != nullptr)
                midin->setPort(porter);
        }

        void initfrontend(Synth* syn);
        bool empty() {
            return first->returnNext() == nullptr;
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
