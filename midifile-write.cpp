#include "midifile.h"

namespace sb {
    namespace midi {

        void MidiFIO::write(MidiFileItem writeitem) {
            if (!writing || !river.is_open())
                return;
            //TODO: The following two lines are a hack. In a future version, Soundbench will correctly store the full range of values of a uint32_t.
            if (writeitem.delay & 0xF0000000) { // The most significant 4 bits of a uint32_t.
                std::cerr << "Internal warning: one or more of the last significant bits of a MidiFileItem::delay were set.\n";
                river.write("\255\255\255\127",4);
            }
            else {
                uint8_t len = 0;
                //TODO: Messy code. Clean this up.
                if (writeitem.delay < 0x80)
                    len = 1;
                else if (writeitem.delay < 0x8000)
                    len = 2;
                else if (writeitem.delay < 0x800000)
                    len = 3;
                else
                    len = 4;
                tracklen += len;
                writeitem.delay <<= len + (4-len)*8;
                for(uint8_t i = 0; i < len; ++i) {
                    writeitem.delay >>= 1;
                    uint8_t val = *(reinterpret_cast<uint8_t*>(&writeitem.delay)+i);
                    if (i == len-1)
                        val &= ~Bit1; //Set the next-byte-exists bit to 0.
                    else
                        val |= Bit1; //Set the next-byte-exists bit to 1.
                    river.put(val);
                }
            }
            uint8_t dat = writeitem.evtype;
            dat <<= 4;
            dat += writeitem.chan;
            river.put(dat);
            if(writeitem.evtype != midi::SystemEvent) {
                river.put(writeitem.params.first);
                river.put(writeitem.params.second);
            }
            else {
                river.put(writeitem.meta);
                if (writeitem.meta == midi::EndOfTrack) {
                    river.put(0);
                }
            }
        }

    }
}
