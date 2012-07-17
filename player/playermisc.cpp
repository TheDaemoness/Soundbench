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
    std::vector<std::string> Player::getPorts() {
        if(midin != nullptr)
            return midin->getPorts();
        return std::vector<std::string>();
    }
    size_t Player::getCurrentPort() {
        if (midin != nullptr)
            return midin->getCurrentPort();
        return 0;
    }
    bool Player::setVirtualPort(bool vport) {
        if (midin == nullptr)
            return false;
        if (!midin->supportsVirtualPorts())
            return false;
        midin->setVirtualPort(vport);
        return true;
    }
    void Player::startPlay() {
        if (playing)
            return;
        playing = true;
        playthread = new std::thread(playChain,this);
    }
    void Player::stopPlay() {
        if (!playing)
            return;
        playing = false;
        playthread->join();
        delete playthread;
    }
    void Player::startRt() {
        if(midin != nullptr)
            midin->start();
    }
    void Player::stopRt() {
        if(midin != nullptr)
            midin->stop();
    }

    void Player::startRec() {
        if(midin != nullptr)
            return;
        if(!midin->isRecording())
            return;
        midin->record(true);

    }

    void Player::stopRec() {
        if(midin == nullptr)
            return;
        if(!midin->isRecording())
            return;

        midin->record(false);
        if(first != nullptr)
            delete first;
        first = midin->detachChain();

        uint32_t len = 0;
        for(auto i = first->returnNext(); i != nullptr; i = i->returnNext())
            ++len;
        --len; //Ignore the ending node.
        if (len == 0) {
            std::cerr << "No events were recorded.\n";
            first->chainDestroy();
        }
        else if (len == 1)
            std::cerr << "Recorded 1 event.\n";
        else
            std::cerr << "Recorded " << len << " events.\n";
    }

    void Player::playChain(Player* plai) {
        uint64_t micros = 0;
        auto iter = plai->first->returnNext();
        while (plai->playing) {
            while (micros >= iter->getDelay()) {
                iter->doEvent();
                micros -= iter->getDelay();
                iter = iter->returnNext();
                if (iter == nullptr) {
                    emit plai->donePlaying();
                    return;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            micros += 1000;
        }
    }
}
