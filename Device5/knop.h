#ifndef knop_h
#define knop_h
#define KN_CNT 1
#include "Arduino.h"

extern byte cv16waarde(char char0, char char1);

class knopCls {
  public:
    knopCls();
    void timSetter(char knopMem);
    void netSet() {
      for (byte tel = 0; tel < KN_CNT; tel++) digitalWrite(led[tel], HIGH), ledst[tel] = false, ledUp[tel] = true;
    }
    boolean deLoop();
    void timerloop(int tDiff);
    boolean* knopPoint;
    void serEvFunc(char ch_0, char ch_1, char ch_2);
    void knipDoorF(int tDiff);
  private:
    byte timertel = 0;
    boolean knip3T = false;
    boolean teltimer = 0;
    boolean knipper[KN_CNT];
    int knop[KN_CNT] { 2 };
    int led[KN_CNT] { 8 };
    volatile byte ledst[KN_CNT];
    volatile boolean timerloopbool = 0;
    int knopwacht[KN_CNT];
    boolean knopst[KN_CNT];
    boolean ledstNu[KN_CNT];
    volatile boolean ledUp[KN_CNT];
    int dimTel[KN_CNT];
    boolean dimTelBl[KN_CNT];
    boolean dimCheck0 = false; boolean dimRicht[KN_CNT];
    char knopMem;
    unsigned int gelezen = 0;
    void knopproc(byte no);
    void knipperproc(int tDiff);
};
extern knopCls knopC;
#endif
