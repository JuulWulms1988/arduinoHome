#ifndef piScreen_h
#define piScreen_h
#include "Arduino.h"

enum psNo{PSNO_0 = 2, PSNO_1 = 4};

class piScreenCls {
  public:
    piScreenCls(int i, psNo n);
    psNo no;
    boolean set = false;
    void cntF(int tDiff);
    void func();
    void confirm(boolean IO);
  private:
    void endF(unsigned long x);
    void sendUtF();
    int pin;
    volatile boolean counting = false;
    volatile unsigned long cnt = 0;
    unsigned long cntEnd;
    byte mode = 0;
};
#endif
