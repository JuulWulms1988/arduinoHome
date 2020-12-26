#ifndef piScreen_h
#define piScreen_h
#include "Arduino.h"

class piScreenCls {
  public:
    piScreenCls(int i, byte n);
    byte no;
    boolean set = false;
    void cntF(int tDiff);
    void func();
    void confirm(boolean IO);
  private:
    void endF(unsigned long x);
    int pin;
    volatile boolean counting = false;
    unsigned long cnt = 0;
    unsigned long cntEnd;
    byte mode = 0;
};
#endif
