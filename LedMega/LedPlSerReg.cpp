#include "LedPlaf.h"

void ledPlaf::objS::serVarSetOb(byte gVar, unsigned long gVal) {
  switch (gVar) {
    case 0: helder = gVal; return; case 1: rgbPauze = gVal; return; case 2: negDlay = gVal; return; case 3: gamDlay = gVal; return; case 4: negGamDlay = gVal; return;
    case 5: gamPer = gVal; return; case 6: gamClr = gVal; return; case 7: gamVal = gVal; return; case 8: gamIVal = gVal; return; case 9: gamIHel = gVal; return;
    case 10: helDlay = gVal; return; case 11: negHelCur = gVal; return; case 12: gamPauze = gVal; return; case 13: helAmp = gVal; return;

    case 20: dlay = gVal; return; case 21: gamInterval = gVal; gamIntWait = 0; return; case 22: helPer = gVal; return; case 23: per = gVal; return;

    case 40: randVar = gVal; if (randVar && !parChldPos()) setFullClrF(); return;
  }
}

unsigned long ledPlaf::objS::getVarF(byte vNo) {
	switch (vNo) {
	case 0: return helder; case 1: return rgbPauze;	case 2: return negDlay;	case 3: return gamDlay;	case 4: return negGamDlay; case 5: return gamPer;
	case 6: return gamClr; case 7: return gamVal; case 8: return gamIVal; case 9: return gamIHel; case 10: return helDlay; case 11: return negHelCur;
	case 12: return gamPauze; case 13: return helAmp; case 20: return dlay; case 21: return gamInterval; case 22: return helPer;	case 23: return per;
	case 40: return randVar;
	}
}

void ledPlaf::serSetSer(boolean ser) {
  byte sNo = serNo[ser]; {
    byte sP = servChld[ser];
    if (sP == STRIP_CNT || (sP && servChldAr[ser][sP - 1] == STRIP_CNT)) {
      serSetSerPrep(ser, sNo, sP); return;
    }
  } volatile boolean* cL; byte b = serBron[ser], c = partOfCl(b, cL);
  objS* flag = NULL;
  for (byte t, v, tt = t = 0; tt < c && t < STRIP_CNT; t++)
    if (!cL[t] || !++tt) continue;
    else if (!(v = vergStrip(t))) {
      obj[t - STRIP_STRT].serVarSetOb(basprc::cvintcharint(*serBuf[ser]), basprc::cvintcharStr(serBuf[ser] + 1));
      if (!sNo && t == b) flag = &obj[t - STRIP_STRT];
    } else if (v == 2) break;
  if (serOpn[ser]) serVoeg3(ser);
  if (!flag) return;
  flag->telResNl();
  if (c > 1 && flag->randVar) flag->clrSUp = 2, flag->offSUp = true; else flag->clrSUp = 1, flag->offSUp = true;
  flag->settedColor = true;
}

void ledPlaf::serSetSerPrep(boolean ser, byte sNo, byte sP) {
  byte* sPA = servChldAr[ser];
  if (sP == STRIP_CNT) {
    sP = servChld[ser] = cvIntStr(ser);
    if (sP) {
      sPA[sP - 1] = STRIP_CNT;
      return;
    }
  } else if ((sNo && sNo - 2 < sP) || (!sNo && sP && sPA[sP - 1] == STRIP_CNT)) {
    if (!sNo) sPA[sP - 1] = cvIntStr(ser);
    else {
      sPA[sNo - 2] = cvIntStr(ser);
      if (sNo - 1 != sP) return;
    }
  } byte y = serBron[ser]; boolean f, f2; f = f2 = false;
  switch (vergStrip(y)) {
    case 0: f2 = serSetSerDoor(y, ser, sP, sPA); serSetSerSIntern(y, y, sP, sPA); break;
    case 1: f2 = serSetSerDoor(y, ser, sP, sPA); break;
    case 2: f = true; break;
  } if (!f)
    for (byte t = 0, x; t < sP; t++) {
      switch (vergStrip(x = sPA[t])) {
        case 0: serSetSerSIntern(x, y, sP, sPA); break;
        case 2: f = true;
      } if (f) break;
    } if (f || f2) {
    char c[STRIP_CNT * 2] { basprc::cvintcharchar(serBron[ser]), 'S' , basprc::cvintcharchar(sP) }; byte l = 3;
    for (byte t = 0; t < sP; t++) c[l++] = '_', c[l++] = basprc::cvintcharchar(sPA[t]);
    unsigned int buff;
    if (!f && f2) buff = serNo[ser], serNo[ser] = 0;
    else f2 = false;
    serVoeg2(c, l, !ser), serVoeg([&]{ if (!serNo[ser]) return '\n'; return '_'; }(), !ser), serOpn[ser] = true;
    if (f2) serOpn[ser] = false, serNo[ser] = buff;
  }
}

boolean ledPlaf::serSetSerDoor(byte bron, boolean ser, byte sP, byte* sPA) {
  byte t = 0; 
  boolean out = false; while (true) {
    volatile boolean* cL; byte c = partOfCl(bron, cL);
    if (c) {
      for (byte tt = 0, ttt = 0, x; ttt < c && tt < STRIP_CNT; tt++) {
        if (!cL[tt]) continue;
        if (!(x = vergStrip(tt))) {
          if (bron != tt) obj[tt - STRIP_STRT].changeParChld(false, bron);
        } else if (x == 2 && (out = true)) break; ttt++;
      }
    } 
	if (t < sP) bron = sPA[t++];
    else break;
  } return out;
}

