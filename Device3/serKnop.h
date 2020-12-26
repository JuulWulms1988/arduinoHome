#ifndef serKnop_h
#define serKnop_h
#include "Arduino.h"

#define KN_CNT 5

class serKnCls {
  public:
    void setF();
    void netwSet();
    void verwerk(byte depin);
    void updateF();
    void tijdF(byte x) {
      Serial.write(x % 4 + 't'), Serial.println();
    }
    void timF();
    void procKnop();
  private:
    char knopMem;
    boolean knop[KN_CNT];
    boolean knopst[KN_CNT];
    int knoptel2[KN_CNT];
    boolean knopuitgestuurd2[KN_CNT];
    boolean knopuitDimgestuurd[KN_CNT];
    boolean knopdim2[KN_CNT];
    volatile boolean knopnawacht[KN_CNT];
    void sluitknpr(byte no, char* ls) {
      *(ls++) = no / 10 + '0'; *(ls++) = no % 10 + '0'; *(ls++) = '+'; *ls = '\0';
    }
};

extern serKnCls serKn;
#endif
