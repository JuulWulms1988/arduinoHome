#include "LedPlaf.h"

void ledPlaf::serStVarF(boolean ser) {
  boolean sN = false; byte sNo = serNo[ser];
  if (sNo == 1) sN = true, serVal[ser] = cvIntStr(ser);
  boolean doorstuur = false;
  volatile boolean* cL; byte c = partOfCl(serBron[ser], cL);
  if (!c) doorstuur = true; byte ckerPar = 255;
  unsigned long wVal = cvIntStr(ser);
  boolean welOff = false;
  for (byte t, tt = t = 0; tt < c; t++) {
    if (!cL[t] || !(++tt)) continue;
    else if ((ckerPar == 255 && !((ckerPar = t) + 1)) || (t >= STRIP_STRT && t <= STRIP_END)) {
      if (sN) continue;
      if (serVal[ser] > 19) obj[t - STRIP_STRT].settedColor = false;
      obj[t - STRIP_STRT].serVarSetOb(serVal[ser], wVal);
      switch (serVal[ser]) {
        case 5: //gamPer -> per
          if (obj[t - STRIP_STRT].per >= wVal) break;
          {
            unsigned long jaja; if (wVal && wVal < 100) jaja = 100; else jaja = wVal;
            obj[t - STRIP_STRT].settedColor = false, obj[t - STRIP_STRT].serVarSetOb(23, jaja), welOff = true;
          }
          break;
        case 23: //per -> gamper
          if (obj[t - STRIP_STRT].gamPer) {
            if (obj[t - STRIP_STRT].gamPer <= wVal) break;
            obj[t - STRIP_STRT].serVarSetOb(5, wVal);
          } break;
      }
    }
    else {
      doorstuur = true;
      if (t > STRIP_END) break;
    }
  }
  if ((welOff || serVal[ser] > 19) && ckerPar >= STRIP_STRT && ckerPar <= STRIP_END) {
    objS& p = obj[ckerPar - STRIP_STRT];
    p.telResNl();
    if (c > 1 && serVal[ser] > 39 && p.randVar) p.clrSUp = 2, p.offSUp = true; else p.clrSUp = 1, p.offSUp = true;
    p.settedColor = true;
  }
  if (doorstuur && !serNo[ser]) {
    serVoeg(basprc::cvintcharchar(serBron[ser]), !ser), serVoeg('V', !ser);
    if (serVal[ser] >= 60) serVoeg(basprc::cvintcharchar(serVal[ser] / 60), !ser); serVoeg(basprc::cvintcharchar(serVal[ser] % 60), !ser); serVoeg('_', !ser);
    serVoeg2(serBuf[ser], basprc::charPLen(serBuf[ser]), !ser); serVoeg('\n', !ser);
  }
}
