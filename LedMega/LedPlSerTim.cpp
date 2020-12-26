#include "LedPlaf.h"

void ledPlaf::serOffUp(boolean ser) {
  boolean doorstuur = false;
  volatile boolean* cL; byte c = partOfCl(serBron[ser], cL);
  if (!c) doorstuur = true;
  for (byte t, tt = t = 0; tt < c; t++)
    if (!cL[t] || !(++tt)) continue;
    else if (t < STRIP_STRT) {
      if (ser) {
        doorstuur = true;
        break;
      } continue;
    } else if (t >= STRIP_STRT && t <= STRIP_END ) {
      objS* o = &obj[t - STRIP_STRT]; unsigned long uit = cvIntStr(ser); switch (serNo[ser]) {
        case 0: o->getRes[1] = uit; o->offUp = true; break; case 1: o->getOff = uit; break;
        case 2: o->getOffT = uit; break; case 3: o->getTel = uit; break;
        case 4: o->getGam = uit; break; case 5: o->getHelCur = uit; break; case 6: o->getPos = uit; break; case 7: o->getRes[0] = uit; break;
      } if (t < o->no) break;
    } else {
      if (!ser) doorstuur = true; break;
    } if (doorstuur) serVoeg3(ser); else if (!serNo[ser]) Serial3.write('$');
}

void ledPlaf::serStClr(boolean ser) {
  boolean doorstuur = false; volatile boolean* cL; byte c = partOfCl(serBron[ser], cL);
  if (!c) doorstuur = true; objS* pp = NULL; for (byte t, tt = t = 0; tt < c; t++) {
    if (!cL[t]) continue; else tt++;
    if (t < STRIP_STRT) {
      if (ser) {
        doorstuur = true;
        break;
      } continue;
    } else if (t >= STRIP_STRT && t <= STRIP_END) {
      if (!pp) pp = &obj[t - STRIP_STRT];
      objS* o = &obj[t - STRIP_STRT]; /*if (o->settedColor) continue;*/ unsigned int i = serNo[ser];
      if (!i) o->kleurAr[pp->clrRUp][STRIP_CNT - 1] = cvIntStr(ser);
      else o->kleurAr[pp->clrRUp][i - 1] = cvIntStr(ser);

      if (t < serBron[ser]) break;
    } else {
      if (!ser) doorstuur = true;
      break;
    }
  } if (!serNo[ser] && pp && ++pp->clrRUp == STRIP_CNT + 3) pp->clrRUp = 0;
  if (doorstuur) serVoeg3(ser);
}

void ledPlaf::serStNClr(boolean ser) {
  boolean sN = false; byte sNo = serNo[ser];
  if (sNo == 1) sN = true, serVal[ser] = cvIntStr(ser);
  boolean doorstuur = false;
  volatile boolean* cL; byte c = partOfCl(serBron[ser], cL);
  if (!c) doorstuur = true;
  for (byte t, tt = t = 0; tt < c; t++) {
    if (!cL[t]) continue; else tt++;
    if (t < STRIP_STRT) {
      if (ser) doorstuur = true; continue;
    } else if (t >= STRIP_STRT && t <= STRIP_END) {
      if (sN) continue;
      if (sNo) obj[t - STRIP_STRT].kleurAr[serVal[ser]][serNo[ser] - 2] = cvIntStr(ser);
      else obj[t - STRIP_STRT].kleurAr[serVal[ser]][STRIP_CNT - 1] = cvIntStr(ser);
    } else {
      doorstuur = true;
      break;
    }
  } if (doorstuur) serVoeg3(ser);
}

boolean ledPlaf::objS::offSerSendF() {
  clrSUp = 0; byte ser = 3; for (byte t = 0, tt = 0; tt < parChld; t++)
    if (!parChldAr[t] || !++tt || t == no) continue;
    else if (t < no) {
      if (t < STRIP_STRT) ser = 0; break;
    } else if (t <= STRIP_END) continue;
    else {
      ser = 1; break;
    } if (ser == 3) return true; char x[63]; *x = basprc::cvintcharchar(no), x[1] = 'O'; byte t = 0; unsigned int l = 2;
  do {
    unsigned long uit; if (t) x[l++] = '_'; switch (t++) {
      case 0: uit = getOff; break; case 1: uit = getOffT; break;
      case 2: uit = getTel; break; case 3: uit = getGam; break;
      case 4: uit = getHelCur; break; case 5: uit = getPos; break; case 6: uit = getRes[0]; break; case 7: uit = getRes[1]; break;
    } l += basprc::cvCharLngth(60, uit, x + l);
  } while (t < 8); x[l++] = '\n'; x[l] = '\0';
  if (ser) Serial2.print(x); else Serial3.print(x); return false;
}

void ledPlaf::clrSerSendF() {
  byte obNo = (clrUp - 1) / 3, ac; objS& p = obj[obNo]; ac = p.clrSUp - 2; if (!ac && clrSerSendPrepF(obNo, p)) return; {
    char s[STRIP_CNT * 3]; byte t = 0; volatile byte* pp = p.kleurAr[ac++]; for (byte tt = 0; tt < STRIP_CNT; tt++) {
      if (tt) s[t++] = '_'; t += basprc::cvCharLngth(60, pp[tt], s + t);
    }
    serVoegMes((clrUp - 2) % 3, 'N', p.no, s, t);
  } if (ac == STRIP_CNT + 3) {
    p.clrSUp = 1, clrUp = 0; if (!obNo) cntStrip = STRIP_AANT - 1; else cntStrip = obNo - 1;
  } else p.clrSUp++;
}

boolean ledPlaf::clrSerSendPrepF(byte obNo, objS &p) {

  volatile boolean* a = p.parChldAr;
  for (byte t = 0; t < STRIP_CNT; t++)
    if (!a[t] || t == p.no) continue;
    else if (t < p.no) {
      if (t < STRIP_STRT) {
        clrUp = obNo * 3 + 2; break;
      } else cpClrAll((volatile byte**) p.kleurAr, (volatile byte**) obj[t - STRIP_STRT].kleurAr); break;
    } else if (t <= STRIP_END) cpClrAll((volatile byte **)p.kleurAr, (volatile byte**)obj[t - STRIP_STRT].kleurAr);
    else {
      clrUp = obNo * 3 + 3; break;
    }
  if ((clrUp - 1) % 3) return false;
  p.clrSUp = 1, clrUp = 0; if (!obNo) cntStrip = STRIP_AANT - 1; else cntStrip = obNo - 1; return true;
}

void ledPlaf::cpClrAll(volatile byte** bron, volatile byte** des) {
  byte t, tt = t = 0; do {
    des[tt] = bron[tt++]; if (tt != STRIP_CNT) continue; if (t < STRIP_CNT + 2) tt = 0, t++, bron++, des++; else return;
  } while (true);
}

