#include "LedPlaf.h"
ledPlaf* pLedPlaf = NULL;
boolean settedOB = false;
volatile boolean settedOB2 = false;
unsigned int intrUpTl = 384; volatile byte intrUpTlU = 0; volatile boolean intrUpTURec = false;
volatile boolean intrFlagBl = false;
volatile boolean borg_bl = false, borg_bl2 = false;
byte borg_cnt = 0, borg_cnt2 = 0;


ledPlaf::ledPlaf() {
  constr();
  pLedPlaf = this;
  while (serLoop()) interruptF();
}

ledPlaf::ledPlaf(boolean x) {
  if (!x) settedOB2 = false, intrUpTl = 384, intrUpTlU = 0, intrUpTURec = false;
  constr();
  if (x) pLedPlaf = this;
}

ledPlaf::~ledPlaf() {
  if (pLedPlaf != this) *pLedPlaf = *this; else settedOB2 = false, intrUpTl = 384, intrUpTlU = 0, intrUpTURec = false;
}

void ledPlaf::constr2() {
  for (byte t = 0, x = STRIP_AANT * 3; t < x; t++) pinMode(pinF(t), OUTPUT);
  Serial3.begin(74880); boolean X = false; if (STRIP_END != STRIP_CNT - 1) X = true, Serial2.begin(74880);
  randomSeed(analogRead(A0)); Serial3.write('|'); if (X) Serial2.write('|'); pinMode(22, OUTPUT), pinMode(23, OUTPUT); pinMode(20, INPUT); pinMode(21, INPUT); digitalWrite(22, digitalRead(20)), digitalWrite(23, digitalRead(21));

  attachInterrupt(digitalPinToInterrupt(21), []() {
    digitalWrite(23, digitalRead(21));
    do {
      if (!settedOB2) return;
      if (borg_cnt) {
        if (borg_cnt2) {
          byte j;
          if (borg_bl)
            if (borg_cnt2 / 10) borg_cnt2 = 0;
            else j = 1;
          else j = 0, borg_cnt2 %= 10;
          if (borg_cnt2)
            if (!(borg_cnt2 = (borg_cnt2 + 1) / 4)) {
              borg_cnt = 0, borg_bl2 = false; break;
            }
            else borg_cnt2 += j * 10;
        }
        else if (!borg_bl) borg_cnt2 = 1;
        if (!(borg_cnt = (borg_cnt + 1) / 11)) intrFlagBl = true, borg_bl2 = false, borg_cnt2 = 0;
      }
      else if (borg_bl2) borg_cnt = 1;
    } while (false);
    if ((intrFlagBl && !(intrFlagBl = false) && !intrUpTURec && ((!intrUpTlU && (intrUpTl >= 384 || !intrUpTl ) && (intrUpTURec = true)) || (intrUpTlU == 1 && intrUpTl && intrUpTl < 128 && (intrUpTURec = true) && !(intrUpTl = 512)))) || !intrUpTl) {
      intrUpTl = 1;
      pLedPlaf->int_up = true;
      if ((intrUpTlU = (intrUpTlU + 1) % 8) == 4) intrUpTURec = false;
    }
    else intrUpTl = (intrUpTl + 1) % 512;
    pLedPlaf->int_flag = true;
  }, CHANGE);
  attachInterrupt(digitalPinToInterrupt(20), []() {
    boolean U;
    digitalWrite(22, U = digitalRead(20));
    if (U) borg_bl2 = borg_bl = true;
    else borg_bl = false;
  }, CHANGE);
}

void ledPlaf::constr() {
  if (!pLedPlaf) constr2(); for (byte t = 0; t < STRIP_AANT; t++) obj[t].no = t + STRIP_STRT; for (byte t = 0, x = STRIP_AANT * 3; t < x; t++) analogWrite(pinF(t), 0); if (STRIP_END == STRIP_CNT - 1) {
    serBootF0(false), Serial3.write('$');
  } else serBootF1();
}

boolean ledPlafLoop() {
  if (pLedPlaf)
    if (pLedPlaf->serLoop()) {
      pLedPlaf->interruptF();
      return true;
    }
    else {
      ledPlaf(false); return false;
    } ledPlaf(true), pLedPlaf = &(*((ledPlaf*)malloc(sizeof(*pLedPlaf))) = *pLedPlaf); return true;
}

void ledPlaf::serBootF1() {
  byte stW = 0; do
    switch (stW++) {
      case 0:
        do {
          if (Serial3.available()) Serial3.read(); if (Serial2.available() && Serial2.read() == '&') break;
        } while (true); break;
      case 1: if (serBootF0(true)) stW = 0; else Serial2.write('&'); break;
      case 2:
        do {
          if (Serial3.available() && Serial3.read() == '|') {
            Serial2.write('|'); break;
          } if (!Serial2.available()) continue; char tC = Serial2.read(); if (tC == '|') {
            Serial3.write('|'); break;
          } if (tC != '$') continue; Serial3.write('$'); return;
        } while (true); stW = 0; break;
    } while (true);
}

void ledPlaf::serReset(boolean ser) {
  if (ser) Serial3.write('|'); else if (STRIP_END < STRIP_CNT - 1) Serial2.write('|');
  settedOB = false;
}

boolean ledPlaf::serBootF0(boolean x) {
  byte teol = 4;
  do {
    bool flag; do {
      flag = false;
      if (x && Serial2.available() && (Serial2.read() == '|' || !(flag = true))) {
        Serial3.write('|'); return true;
      }
      if (Serial3.available())
        if (Serial3.read() == '&') {
          borg_cnt = 0, borg_cnt2 = 0, borg_bl = false, borg_bl2 = false, settedOB2 = intrFlagBl = true;
          return false;
        }
        else flag = true;
    } while (flag); if (!(teol = (teol + 1) % 5)) Serial3.write('&'); delay(1);
  } while (true);
}

ledPlaf::ledPlaf::objS::objS() {
  stClrNll();
}

ledPlaf::ledPlaf::objS::objS(byte x, byte* y) {
  no = x, stClrNll(); for (byte t = 0; t < 3; t++) ledNu[t] = y[t];
}

ledPlaf::ledPlaf::objS::objS(objS& varOb, objS& parOb) {
  *this = varOb, no = parOb.no; byte* y = parOb.ledNu; for (byte t = 0; t < 3; t++) ledNu[t] = y[t];
}

void ledPlaf::objS::telResNl() {
  unsigned int pF = perF();
  unsigned long uit[2] {0, 0};
  {
    unsigned int x = pF % per;
    {
      unsigned long out = x;
      unsigned long t = 1;
      while (out %= per) out += x, t++;
      for (unsigned int tel = 0; tel < t; tel++) basprc::longASom (uit, pF);
    }
  }
  bufTRes[1] = 0, bufTRes[1] = pF; basprc::longASom(bufTRes, uit[1]);
  for (unsigned long t = 0; t < uit[0]; t++) basprc::longASom(bufTRes, (unsigned long)(0 - 1)), basprc::longASom(bufTRes, 1);
}

unsigned int ledPlaf::objS::func(boolean offV, unsigned long tel) {
  if (offV)
    if (telDifF(tel, getTel) >= 0) offUp = true, offV = false;
    else offUp = false, offV = true;
  else offV = offUp, offUp = false;
  unsigned int tW = telW(tel);
  if (helPer) helCur = (helCur + 1) % helPer;
  if (per && gamInterval && !tW && ++gamIntWait == gamInterval) gamIntWait = 0;
  if (!telResetF(tel) && tel == tOffset) tOffTel++; if (offSUp) tW = telW(tel); nullTim(tel, tW);
  offUp = offV;
  return tW;
}

void ledPlaf::objS::funcW(delaytWS wS) {
  if (!settedColor) return;
  waardeFunc(&wS);
  byte helK = 0; heldFunc(&wS, gamFunc(&wS, helK), helK);
  helK = 0; byte* c = wS.waarde; byte b = (no - STRIP_STRT) * 3; byte* a = ledNu; while (!helK++ || (helK < 4 && c++ && a++))
    if (*a != *c) analogWrite(pinF(b + helK - 1), *a = *c);
}

void ledPlaf::objS::setClrFunc() {
  byte xx = clrPsF(3); for (byte t = 0, x; t < parChld; t++)
    if ((x = parChldC(t)) == no) continue;
    else if (x >= STRIP_STRT && x <= STRIP_END) {
      pLedPlaf->obj[x - STRIP_STRT].clrUpReg(kleurAr[xx], xx);
    } else if (x > STRIP_END) {
      if (!clrUpBl) clrUp = xx, clrUpBl = true;
      break;
    }
}

void ledPlaf::objS::nullTim(unsigned long tel, unsigned int& tW) {
  {
    boolean up = offUp + offSUp;
    if ((tW && !up ) || (!tW && ((randVar && ((clrPos = clrPsF(1)) || !clrPos)) || !randVar) && parChldPos() && !up)) return;
    if (settedColor && !tW && !parChldPos() && clrF(clrPsF(2), clrPsF(3)) && parChld > 1) {
      setClrFunc();
      if (!up) return;
    }
  } if (offUp) {
    if (telDifF(tel, getTel) >= 0) setOffF(tel, tW), offUp = false;
    if (!offSUp) return;
  }
  for (byte t = 0, x; t < parChld; t++)
    if ((x = parChldC(t)) == no) continue;
    else if (x >= STRIP_STRT && x <= STRIP_END) {
      objS& p = pLedPlaf->obj[x - STRIP_STRT];

      byte pCps = clrPos; boolean terug;
      if (x > no) terug = true; else terug = false;
      if (terug && !tW)
        if (!pCps) pCps = STRIP_CNT + 2;
        else pCps--;
      unsigned long tOffPP = tOffTel;
      if (terug && !tW)
        if (tOffPP) tOffPP--;
        else tOffPP = (unsigned long)(0 - 1);
      p.offCl(tOffset, [&] { if (per && gamInterval && terug && !tW) if (!gamIntWait) return (int)gamInterval - 1; else return (int)gamIntWait - 1; else return (int)gamIntWait; }(), tOffPP , bufTRes);
      if (helPer)
        if (terug && !helCur) p.helCur = helPer - 1;
        else p.helCur = helCur - (terug * 1);
      p.clrPos = pCps;
      if (!terug) break;
    } else {
      getOff = tOffset, getGam = gamIntWait, getRes[0] = bufTRes[0], getRes[1] = bufTRes[1], getPos = clrPos, getOffT = tOffTel, getTel = tel, getHelCur = helCur; break;
    } offSUp = false;
}
