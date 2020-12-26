#include "LedPlaf.h"

void ledPlaf::objS::waardeFunc(delaytWS* wS) {
  byte cPs = clrPos; unsigned x = 0, tW = wS->tW; if (per) {
    byte aDl, tel = 0; if (negDlay) aDl = STRIP_CNT - no - 1; else aDl = no;
    tel = wS->zet(tW, aDl, dlay, per); if (cPs < tel) cPs = STRIP_CNT + 3 - (tel - cPs); else cPs -= tel; x = per - (unsigned long)(rgbPauze) * per / 255;
  } ledPlaf::kleurF(kleurAr[cPs][no], wS->waarde); if (tW < x) return;
  if (!randVar) return; byte newW[3]; ledPlaf::kleurF(kleurAr[(cPs + 1) % (STRIP_CNT + 3)][no], newW);
  for (byte t = 0; t < 3; t++) basprc::cvRaise(wS->waarde[t], newW[t], basprc::partValR(tW - x, per - x));
}

boolean ledPlaf::objS::gamFunc(delaytWS* wS, byte& out) {
  byte gVal = gamVal; boolean uit = false; if (per && gamInterval && gamPer)
    while (true) {
      unsigned int tW = wS->tW; byte gamW = gamIntWait, aDl; if (negGamDlay) aDl = STRIP_CNT - no - 1; else aDl = no; gamTelFunc(gamDlay * aDl, tW, gamW); unsigned int tel = 0; {
        byte gamW2 = 0; unsigned int x, tW2 = x = per - (unsigned long)(gamPauze) * per / 255, gamPer2 = gamPer - 1 % per; if (gamPer - 1 > per) gamW2 = gamPer - 1 / per;
        if (per - tW2 < gamPer2) gamW2++, tW2 = gamPer2 - (per - tW2); if ((!gamW && tW < x) || gamW > gamW2 || (gamW == gamW2 && tW > tW2)) break; if (!gamW) tel = tW - x; else tel = gamW * per + tW;
      } uit = true, out = basprc::partValR2(tel, gamPer), basprc::cvRaise(gVal, gamIVal, out); break;
    } byte gamC[3]; ledPlaf::kleurF(gamClr, gamC); for (byte t = 0; t < 3; t++) basprc::cvRaise2(wS->waarde[t], gamC[t], gVal); return uit;
}

void ledPlaf::objS::heldFunc(delaytWS* wS, boolean helKB, byte& helK) {
  byte hW = helder; if (helKB) basprc::cvRaise(hW, gamIHel, helK);
  while (helAmp && helPer) {
    byte amp = helAmp, cur = helCur; if (helder < 4 || hW < 4) break; if (amp > helder - 2) amp = helder - 2; if (amp > hW - 2) amp = hW - 2;
    byte aDl; if (negHelCur) aDl = STRIP_CNT - no - 1; else aDl = no; unsigned int tt = (unsigned long)(helDlay) * aDl % helPer;
    if (tt > cur) cur = helPer - (tt - cur); else cur -= tt; hW -= amp * ((1 + cos((2 * cur / float (helPer) + 1) * PI)) / 2); break;
  } byte high = 0; byte* pW = wS->waarde; for (byte t = 1; t < 3; t++) if (pW[t] > pW[high]) high = t; if (hW % 2) hW--; if (pW[high] == hW) return; else if (pW[high] < 2) {
    for (byte t = 0; t < 3; t++) pW[t] = 0; return;
  } float fac = hW / float (pW[high]); pW[high] = hW; for (byte t = 0; (t == high && !++t) || t < 3; t++) if (!pW[t]) continue; else if (!(pW[t] *= fac)) pW[t] = 1;
}

