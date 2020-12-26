#include "LedPlaf.h"

extern boolean settedOB;
extern ledPlaf* pLedPlaf;

void ledPlaf::interruptF() {
  if (!settedOB)
    if (!int_up) return;
    else settedOB = true, int_up = false;
  if (!int_now)
    if (!int_flag) return;
    else {
      int_flag = false;
      if (int_up) {
        int_up = false;
        if (((int_wacht = telDifF(int_nieuwTel += 512, tel)) < 0 && int_wacht++) || !int_wacht) return;
        else {
          tel += int_wacht--, int_tW[0] = 0;
          int_now = (boolean)(int_wacht) * STRIP_AANT;
        }
      }
      else if (int_wacht < 0) {
        int_wacht++; return;
      } else tel++;
    }
  if (int_wacht && int_now == STRIP_AANT) {
    obj[int_tW[0]].raiseTimeF(int_wacht, tel);
    if (++int_tW[0] == STRIP_AANT) int_now = 1;
  }
  else if (int_now < STRIP_AANT) {
    if (int_wacht) int_wacht = int_now = 0;
    if (!int_now) for (byte t = 0; t < STRIP_AANT; t++) int_tW[t] = obj[t].offUp;
    objS& P = obj[int_now];
    int_tW[int_now] = obj[int_now].func(int_tW[int_now], tel);
    int_now++;
  }
  else if (int_flag && int_now != STRIP_AANT) int_now = 0, interruptF();
  else obj[int_ledValNext].funcW(objS::delaytWS{ int_tW[int_ledValNext] }), int_ledValNext = (int_ledValNext + 1) % STRIP_AANT, int_now = (int_now + 1) % (STRIP_AANT * 2);
}

int ledPlaf::telDifF(unsigned long x, unsigned long y) {
  unsigned long z;
  int uit;
  if (x < y)
    if ((z = (unsigned long)(y - x)) < 4096) uit = (int)z * (-1);
    else uit = (int)((unsigned long)((unsigned long)(0 - 1) - y)) + 1 + x;
  else if (y < x)
    if ((z = (unsigned long)(x - y)) < 4096) uit =  z;
    else uit = ((int)((unsigned long)((unsigned long)(0 - 1) - x)) + 1 + y) * (-1);
  else uit = 0;
  // Serial.println(uit);
  return uit;
}

void ledPlaf::objS::raiseTimeF(int tPass, unsigned long tel) {
  boolean ps = parChldPos(); unsigned long gtel = tel - tPass - 1;
  unsigned int tW = telW(gtel);
  if (helPer) helCur = ((unsigned long)(helCur) + tPass) % helPer; else helCur = 0;
  for (byte t = 1; t <= tPass; t++) {
    tW = (tW + 1) % per;
    if (per && gamInterval && !tW && ++gamIntWait == gamInterval) gamIntWait = 0;
    if (!telResetF(gtel + t) && (gtel + t == tOffset)) tOffTel++;
    if (!tW) {
      clrPos = clrPsF(1);
      if (!ps) setClrFunc();
    }
  }
}
