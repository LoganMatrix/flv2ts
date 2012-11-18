#include <iostream>
#include <ts/parser.hh>
#include <ts/packet.hh>

using namespace flv2ts;

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "Usage: ts-extract-audio TS_FILE" << std::endl;
    return 1;
  }

  const char* filepath = argv[1];
  ts::Parser parser(filepath);
  if(! parser) {
    std::cerr << "Can't open file: " << filepath << std::endl;
    return 1;
  }

  for(int i=0; ! parser.eos(); i++) {
    ts::Packet packet;
    if(! parser.parse(packet)) {
      std::cerr << "parse packet failed " << std::endl;
      return 1;
    }

    if(! parser.is_audio_packet(packet)) {
      continue;
    }

    switch(parser.get_payload_type(packet)) {
    case ts::Packet::PAYLOAD_TYPE_PES: {
      const ts::PES& pes = *reinterpret_cast<ts::PES*>(packet.payload); 
      std::cout.write(reinterpret_cast<const char*>(pes.data), pes.data_size);
      break;
    }
    case ts::Packet::PAYLOAD_TYPE_DATA: {
      const ts::Data& data = *reinterpret_cast<ts::Data*>(packet.payload);
      std::cout.write(reinterpret_cast<const char*>(data.data), data.data_size);      
      break;
    }
    default:
      std::cerr << "unexpected packet type: " << parser.get_payload_type(packet) << std::endl;
      return 1;
    }
  }
  return 0;
}
