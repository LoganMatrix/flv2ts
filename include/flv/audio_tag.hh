#ifndef FLV2TS_FLV_AUDIO_TAG_HH
#define FLV2TS_FLV_AUDIO_TAG_HH

#include "tag.hh"
#include <cstddef>

namespace flv2ts {
  namespace flv {
    struct AudioTag : public TagData {
      uint8_t  sound_format:4;
      uint8_t  sound_rate:2;
      uint8_t  sound_size:1;
      uint8_t  sound_type:1;
      uint8_t  aac_packet_type; // present if sound_format == 10
      
      uint8_t* payload;

      size_t headerSize() const {
        if(sound_format == 10) {
          return 2;
        } else {
          return 1;
        }
      }

      size_t payloadSize(const Tag& tag) const {
        return tag.data_size - headerSize();
      }
    };
  }
}

#endif
