#ifndef serKnop_h
#define serKnop_h
#include "Arduino.h"
#include "knop.h"

class serKnCls {
  public:
    void setF();
    void netwSet();
    void verwerk(byte depin);
    void tijdF(byte x) {
      knopC.timSetter(x % 4 + 't');
    }
    void timF();
    void procKnop();
    boolean knop[KN_CNT];
  private:
    boolean knopst[KN_CNT];
    int knoptel2[KN_CNT];
    boolean knopuitgestuurd2[KN_CNT];
    boolean knopuitDimgestuurd[KN_CNT];
    boolean knopdim2[KN_CNT];
    boolean knopnawacht[KN_CNT];
    void sluitknpr(byte no, char* ls) {
      *(ls++) = no / 10 + '0'; *(ls++) = no % 10 + '0'; *(ls++) = '+'; *ls = '\0';
    }
};

extern serKnCls serKn;
#endif
