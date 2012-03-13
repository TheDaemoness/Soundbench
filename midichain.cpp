#include "midichain.h"
namespace sb {
    namespace midi {
        Synth* MIDIEventNode::synref;

        MIDIEventIterator::MIDIEventIterator(PlayerStartNode* master) {
            this->star = master;
            auto lamb = [](MIDIEventNode* first) {

            };
            tread = new std::thread(lamb,star->returnNext());
            tread->join();
            delete tread;
            star->stop();
            delete this;
        }
    }
}
