#include "LedPlaf.h"

extern ledPlaf* pLedPlaf;

boolean ledPlaf::serLoop() {
  byte x = 1; {
    boolean X; if (STRIP_END == STRIP_CNT - 1) X = false; else X = true;
    if (Serial3.available())
      if (serFunc(false))
        if (X) x = 0;
        else return true;
      else return false;
    if (X && Serial2.available())
      if (serFunc(true)) return true;
      else return false;
    else if (!x) return true;
  } if (serNow[0] || serNow[1]) return true;
  if (clrUp) {
    clrSerSendF(); return true;
  } objS* p; x = cntStrip = (cntStrip + 1) % STRIP_AANT; do {
    p = &obj[x];
    if (p->clrSUp)
      if (p->clrSUp == 1) {
        if (!p->offSUp) {
          if (p->offSerSendF()) Serial3.write('$'); break;
        }
      } else {
        clrUp = 1 + x * 3; break;
      } if (!p->clrUpBl) continue; p->clrUpF(); break;
  } while ((x = (x + 1) % STRIP_AANT) != cntStrip);
  return true;
}

void ledPlaf::objS::clrUpF() {
  byte c = clrUp; clrUpBl = false;
  byte tS = 0; char s[STRIP_CNT * 3 + 2];
  for (byte t = 0, x; t < parChld; t++)
    if ((x = parChldC(t)) == no) continue;
    else if (x >= STRIP_STRT && x <= STRIP_END) {
      pLedPlaf->obj[x - STRIP_STRT].clrUpReg(kleurAr[c], c);
    } else {
      volatile byte* y; {
        y = kleurAr[c];
        s[tS++] = basprc::cvintcharchar(no);
        s[tS++] = 'C';
        s[tS++] = basprc::cvintcharchar(c); s[tS++] = '_';
      } for (byte t = 0; t < STRIP_CNT; t++) s[tS++] = basprc::cvintcharchar(y[t]), s[tS++] = '_'; s[tS - 1] = '\n', s[tS] = '\0'; Serial2.print(s); break;
    }
}

boolean ledPlaf::serFunc(boolean ser) {
  char thisChar = thisCharF(ser); if (thisChar == '|') {
    serReset(ser); return false;
  } else if (thisChar == '$') {
    Serial3.write('$');
    return true;
  } else if (!serLees[ser]) serLees[ser] = true, servChld[ser] = STRIP_CNT, serST[!ser] = serNul(ser);
  if (!serRead(thisChar, ser)) return true;
  unsigned int deNo = serNo[ser];
  switch (serNow[ser]) {
    case 'S': serSetSer(ser); break;
    case 'O': serOffUp(ser); break;
    case 'N': serStClr(ser); break;
    case 'C': serStNClr(ser); break;
    case 'V': serStVarF(ser); break;
    case 'D': serDetach(ser); break;
    case 'A': serAttach(ser); break;
    case 'a': serReadAtMess(ser); break;
  } serTel[ser] = 0;
  if (!(serNo[ser] = deNo)) serNow[ser] = '\0';
  return true;
}



boolean ledPlaf::serRead(char thisChar, boolean ser) {
  if (!serNo[ser]) serVal[ser] = 59;
  if ((thisChar == '\n' && !(serNo[ser] = serLees[ser] = false)) || (thisChar == '_' && ++serNo[ser])) {
    serBuf[ser][serTel[ser]] = '\0'; return true;
  } else if (serBron[ser] == 255) serBron[ser] = basprc::cvintcharint(thisChar);
  else if (!serNow[ser]) serNow[ser] = thisChar; else serBuf[ser][serTel[ser]++] = thisChar; return false;
}

void ledPlaf::serVoeg4(boolean ser, char* x, byte l) {
  if (!serOpn[ser]) serOpnF(ser), serOpn[ser] = true;
  serVoeg2(x, l, !ser);
  char c; if (!serNo[ser]) c = '\n'; else c = '_'; serVoeg(c, !ser);
}



