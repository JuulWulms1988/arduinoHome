#include "LedPlaf.h"

byte ledPlaf::partOfCl(byte cl, volatile boolean*& cL) {
  for (byte t = 0; t < STRIP_AANT; t++)
    if (obj[t].parChldAr[cl]) {
      cL = obj[t].parChldAr; return obj[t].parChld;
    } return 0;
}

byte ledPlaf::objS::parChldPos() {
  byte t = 0; for (byte tt = 0; tt < parChld; tt++)
    if (!parChldAr[tt]) continue;
    else if (tt == no) break;
    else t++; return t;
}

byte ledPlaf::objS::rndExPr(byte *e, byte eC, byte t) {
  while (true) {
    boolean flag = false;
    for (byte tt = 0; tt < eC; tt++)
      if (e[tt] == t) {
        flag = true; break;
      } if (flag) t++; else break;
  }
  return t;
}

byte ledPlaf::objS::rndClr(byte *e, byte eC) {
  byte t = rndExPr(e, eC, 1);
  for (byte t2 = 0, uit = random(18 - eC); t2 < uit; t2++) {
    t = rndExPr(e, eC, t + 1);
  } return t;
}


unsigned int ledPlaf::objS::telW(unsigned long tel) {
  bool tOBl; if (tOffTel == (unsigned long)(0 - 1)) tOffTel = 0, tOBl = true; else tOBl = false;
  unsigned int x = 0; if (tOffTel) {
    unsigned int a = ((unsigned long)(0 - 1) % per); a = (a + 1) % per; for (unsigned long t = 0; t < tOffTel; t++) x = (x + a) % per;
  } if (tOBl) tOffTel = (unsigned long)(0 - 1); return ((unsigned long)((telD(tel)) % per ) + x ) % per;
}

void ledPlaf::objS::setFullClrF() {
  clrNlF(); byte t = 0, m = STRIP_CNT - 1;
  {
    volatile byte* y = kleurAr[0];
    byte a[1]; y[0] = rndClr(a, 0);
    while (t < m) *a = t++, y[t] = rndClr(a, 1);
  } t = 0, m += 3; switch (randVar) {
    case 1: while (t < m) clrF1(t++, t); break;
    case 2: while (t < m) clrF2(t++, t); break;
  }
}

void ledPlaf::objS::clrF1(byte X, byte Y) {
  volatile byte* x = kleurAr[X]; volatile byte* y = kleurAr[Y]; for (byte t = 0, ex[2] = { x[t] }, a = 1; t < STRIP_CNT; t++, ex[0] = x[t], a = 1 ) {
    if (t) a++, ex[1] = y[t - 1]; y[t] = rndClr(ex, a);
  }
}

boolean ledPlaf::objS::clrF(byte X, byte Y) {
  if (parChldPos()) return false; switch (randVar) {
    case 1: clrF1(X, Y); break;
    case 2: clrF2 (X, Y); break;
    default: return false;
  } return true;
}

boolean ledPlaf::objS::telResetF(unsigned long tel) {
  boolean flag = false, fl2; if (tOffTel == (unsigned long)(0 - 1)) fl2 = true, tOffTel = 0; else fl2 = false;
  if (offSUp) {
    unsigned int pF = perF();
    if (!tOffTel && telD(tel) < pF) tOffset = tel - pF;
  } if (tOffTel < bufTRes[0] || telD(tel) < bufTRes[1]) {
    if (fl2) tOffTel = (unsigned long)(0 - 1);
    return false;
  }
  tOffTel = 0; tOffset = tel - perF();
  return true;
}

byte ledPlaf::kleur (byte x, byte y) {
  if (!x) return 254;
  else if (x < 4)
    if (y + 1 == x) return 254;
    else return 0;
  else if (x < 7)
    if (y + 4 == x) return 0;
    else return 254;
  else if (x < 10)
    if (y + 7 == x) return 254;
    else return 127;
  else if (x < 13)
    if (y + 10 == x) return 127;
    else return 254;
  boolean even = ((x -= 13) % 2);
  if (y == x / 2) return 127;
  boolean welk = 0;
      for (byte t = 0; t < 3; t++) if (t == x / 2) continue; else if (t == y) break; else welk++;
    if (!welk) if (even) return 254; else return 0;
  else if (even) return 0; return 254;
}

void ledPlaf::objS::setOffF(unsigned long tel, unsigned int& tW) {
  boolean isPar = false; if (!parChldPos()) isPar = true;
  tOffset = getOff, gamIntWait = getGam, clrPos = getPos, tOffTel = getOffT, bufTRes[0] = getRes[0], bufTRes[1] = getRes[1];
  {
    boolean ps = parChldPos(); unsigned int tPass; tW = telW(getTel);
    if (getTel > tel) tPass = tel + ((unsigned long)(0 - 1) - getTel) + 1; else tPass = tel - getTel;
    if (helPer) helCur = ((unsigned long)(getHelCur) + tPass) % helPer; else helCur = 0;
    tPass++;
    for (byte t = 1; t <= tPass; t++) {
      //     if (t) {
      tW = (tW + 1) % per;
      if (per && gamInterval && !tW && ++gamIntWait == gamInterval) gamIntWait = 0;
      if (!telResetF(getTel + t) && (getTel + t == tOffset)) tOffTel++;
      //  }
      if (!tW) {
        clrPos = clrPsF(1);
        if (!ps) setClrFunc();
      }
    }
  } settedColor = true;
}


