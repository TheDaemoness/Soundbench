#ifndef MIDICHAIN_H
#define MIDICHAIN_H

#include "synth.h"

#include <thread>

namespace sb {
    namespace midi {

        class PlayerStartNode;

        class MIDIEventNode {
        protected:
            friend class PlayerStartNode;

            MIDIEventNode* next;
            uint16_t delay;
            static Synth* synref;
        public:
            explicit MIDIEventNode(uint16_t delae = 0) {
                delay = delae;
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

        class NoteOnEventNode : public MIDIEventNode {
            int halfsteps;
            sbSample amp;
        public:
            explicit NoteOnEventNode(int notedist, sbSample ampe = 1.0, uint16_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
                next = nexte;
                halfsteps = notedist;
                amp = ampe;
            }
            void doevent() {
                synref->noteOn(halfsteps,amp);
            }
        };

        class NoteOffEventNode : public MIDIEventNode {
            int halfsteps;
        public:
            explicit NoteOffEventNode(int notedist, uint16_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
                next = nexte;
                halfsteps = notedist;
            }
            void doevent() {
                synref->noteOff(halfsteps);
            }
        };
    }
}

#endif // MIDICHAIN_H
