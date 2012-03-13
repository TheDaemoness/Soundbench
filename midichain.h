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

        /*Objects of this type MUST be dynamically allocated by PlayerStartNode.*/
        class MIDIEventIterator {
            std::thread* tread;
            friend class PlayerStartNode;
            PlayerStartNode* star;
            explicit MIDIEventIterator(PlayerStartNode*);
        };

        class PlayerStartNode : public MIDIEventNode {
            MIDIEventIterator* it;
        public:
            explicit PlayerStartNode(Synth* syn) {
                MIDIEventNode::synref = syn;
                it = nullptr;
            }
            ~PlayerStartNode() {
                if (it != nullptr)
                    delete it;
            }

            void doevent() {};
            void run() {
                if (it == nullptr)
                    it = new MIDIEventIterator(this);
                else {
                    delete it;
                    it = new MIDIEventIterator(this);
                }
            }
            void stop() {
                if (it != nullptr)
                    delete it;
            }
        };

        /*class NoteOnEventNode : public MIDIEventNode {

        };

        class NoteOffEventNode : public MIDIEventNode {

        };*/
    }
}

#endif // MIDICHAIN_H
