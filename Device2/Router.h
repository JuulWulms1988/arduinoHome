#ifndef Router_h
#define Router_h
#include <SPI.h>
#include <Ethernet.h>
#include "Arduino.h"

extern const byte routPin;

class routerCls {
  public:
    void func();
    volatile boolean flagBl = false;
    void flagF();
  private:
   // EthernetClient ff;
    boolean proc() {
      /*for (byte t = 0; t < 3; t++) if (ff.connect("www.google.com", 80)) {
          ff.println("GET /search?q=kaas HTTP/1.1");
          ff.println("Host: www.google.com");
          ff.println("Connection: close");
          ff.println();
          return true;
        }*/ return true;
    }; boolean conBool = false;
    unsigned int tel = 0;
    byte stand = 0;
    char tel2 = -1;
    byte tel1 = 0;
    void lowFunc() {
      digitalWrite(routPin, HIGH), stand++;
    }
};

#endif
