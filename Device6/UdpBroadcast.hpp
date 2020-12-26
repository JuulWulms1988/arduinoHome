#ifndef UdpBroadcast_hpp
#define UdpBroadcast_hpp

#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

namespace udpBroadcast {
  
enum {MAX_SIZE = 11};

inline int readPack(char* buf, EthernetUDP& Udp) {
  static int pSize = 0;// (Serial.begin(9600), 0);
  if (pSize = Udp.parsePacket())
    if (pSize > MAX_SIZE)
      do Udp.read(buf, MAX_SIZE);
      while ((pSize -= pSize > MAX_SIZE ? MAX_SIZE : pSize)/* || (Serial.println("te lang"), 0)*/);
    else {
      Udp.read(buf, MAX_SIZE) /*, Serial.print("gelezen("), Serial.print(pSize), Serial.print("): "), Serial.println(buf)*/;
      if (!buf[pSize - 1] && pSize > 1) return pSize - 1; 
    }
    return 0;
}

}

#endif
