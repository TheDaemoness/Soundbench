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
        unsigned char tempo;
        time_t midiclocklen;
    public:
        explicit Player(Synth* syn) {
            first = new midi::PlayerStartNode(syn);
            tempo = 108;
            /*
              microseconds/beat = 1 / (beats/minute * minutes/second * seconds/microsecond)
              */
            midiclocklen = static_cast<time_t>(1.0 / (static_cast<double>(tempo)/(0.000000017)));
            first->accessMIDIClockLen(&midiclocklen);
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
