#ifndef PLAYER_H
#define PLAYER_H

#include "midi.h"
#include "midichain.h"
#include <deque>

namespace sb {

    class Player
    {
    private:
        midi::PlayerStartNode* first;
        std::deque<midi::MIDIEventNode> nodes;
    public:
        explicit Player(Synth* syn) {
            first = new midi::PlayerStartNode(syn);
        }
        ~Player() {
            delete first;
        }
        inline void play() {
            first->run();
        }
        inline void stop() {
            first->stop();
        }
    };
}

#endif // PLAYER_H
