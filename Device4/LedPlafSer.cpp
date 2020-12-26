#include "LedPlafSer.h"

extern boolean thisMes0; extern boolean thisMes1; extern volatile boolean plafLoopt;

ledPlafSerCls::ledPlafSerCls() {
  plafBool = false;
}

void ledPlafSerCls::func() {
  char thisChar;
  if (!serBeginBool1) return;
  if (Serial.available()) thisChar = Serial.read(); else return;
  
  switch (strtW) {
    case 0: return;
    case 4: serLs(thisChar); return;
    case 3: if (thisChar == '|') plafBool = false, plafLoopt = false, strtW = 1; return;
    case 1: if (thisChar != '&') return; Serial.write('&'); break;
    case 2: if (thisChar != '$') {
        if (thisChar == '|') strtW = 1; return;
      } bufTel = 5, plafBool = true; openF(); break;
  } strtW++;
}

void ledPlafSerCls::serLs (char thisChar) {
  bool flag = false;
  if (thisChar == '|') {
    plafBool = false; strtW = 1; return;
  } bufLs(thisChar); if (thisChar == '$' && !(thisMes0 = !(flag = true)) && bufTel != 5) sendBuf();
  if (flag && thisMes1) thisMes1 = false, openF();
}

void ledPlafSerCls::bufLs(char thisChar) {
  if (thisChar != '+') {
    buf[bufTel++] = thisChar; if (bufTel != 30) return;
  } else if (bufTel == 5) return; sendBuf();
}
