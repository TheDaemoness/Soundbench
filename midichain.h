#ifndef MIDICHAIN_H
#define MIDICHAIN_H

#include "synth.h"

#include <thread>

namespace sb {
    namespace midi {

        class PlayerStartNode;

        class MIDIEventNode {
            friend class PlayerStartNode;

            MIDIEventNode* next;
            uint16_t delay;
            static Synth* synref;
        public:
            MIDIEventNode() {
                next = nullptr;
            }
            MIDIEventNode* nextDestroy() {
                return next->returnNext();
                delete next;
            }
           void chainDestroy() {
                if (next != nullptr) {
                    next->chainDestroy();
                    delete next;
                }
            }
            virtual ~MIDIEventNode() {
                chainDestroy();
            }


            uint16_t getDelay() {
                return delay;
            }
            MIDIEventNode* returnNext() {
                return next;
            }
            virtual void doevent() = 0;
        };



        class MIDIEventIterator {
            //WARNING: Objects of this type delete themselves.
            std::thread* tread;
            friend class PlayerStartNode;
            PlayerStartNode* star;
            explicit MIDIEventIterator(PlayerStartNode*);
        };

        class PlayerStartNode : public MIDIEventNode {
            MIDIEventIterator* it;
            bool running;
        public:
            explicit PlayerStartNode(Synth* syn) {
                MIDIEventNode::synref = syn;
                it = nullptr;
                running = false;
            }
            ~PlayerStartNode() {
                if (it != nullptr)
                    delete it;
            }
            bool isRunning() {
                return running;
            }
            void doevent() {};
            void run() {
                if (it == nullptr)
                    it = new MIDIEventIterator(this);
                else {
                    delete it;
                    it = new MIDIEventIterator(this);
                }
                running = true;
            }
            void stop() {
                if (it != nullptr) {
                    running = false;
                    it = nullptr;
                }
            }
        };

        /*class NoteOnEventNode : public MIDIEventNode {

        };

        class NoteOffEventNode : public MIDIEventNode {

        };*/
    }
}

#endif // MIDICHAIN_H
