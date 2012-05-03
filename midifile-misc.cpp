#include "midifile.h"

#include <sstream>

namespace sb {
    namespace midi {

        bool MidiFIO::close() {
            if (river.is_open()) {
                if (writing) {
                    river.seekp(18);
                    river.write(reinterpret_cast<char*>(&tracklen),4);
                }
                river.close();
            }
            tracks.clear();
            return true;
        }

        void MidiFIO::readFrom(uint16_t traque) {
            river.clear();
            if(!river.is_open() || writing || traque >= tracks.size() || filetype == 0)
                return;

            river.seekg(tracks[traque]+4); //Soundbench can ignore the 4 bytes with the MTrk.
            tracklen = 0;
            for (uint8_t i = 0; i < 4; ++i) {
                tracklen += river.get();
                if (i < 3)
                    tracklen <<= 8;
            }
        }

        std::string MidiFIO::getTrackName(uint16_t traque) {
            if (traque >= tracks.size() || !river.is_open() || writing)
                return "";

            uint64_t currpos = river.tellg();
            if (filetype == 0)
                river.seekg(22);
            else if (filetype == 1 && traque != 0) //TODO: Do a search for an instrument name meta event.
                return std::string("Instrument Track ") + lexical_cast<std::string>(traque);
            else
                river.seekg(tracks[traque]+8); //Skip over the MTrk and the track length.

            //Ensure that we're reading a track name event. That'd be nasty if we weren't.
            std::string name;
            const uint8_t tracknamebytes[3] = {0x0,MetaEvent,MetaTrackName};
            for(uint8_t i = 0; i < 3; ++i) {
                if(river.get() != tracknamebytes[i])
                    return (filetype==1?"Metadata Track":"");
            }

            uint32_t evlen;
            for(uint8_t i = 0; i < 4; ++i) { //Parse the variable length data field
                evlen <<= 7;
                uint8_t byte = river.get();
                evlen |= byte & ~Bit1;
                if (!(byte & Bit1))
                    break;
            }
            for(uint32_t i = 0; i < evlen; ++i)
                name.push_back(river.get());

            river.seekg(currpos);
            return name;
        }
    }
}
