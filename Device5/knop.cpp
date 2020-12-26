#include <TimerOne.h>
#include "knop.h"
extern void dod2F (void (*x)(void*, int), void* y);


knopCls knopC;

knopCls::knopCls() {
  for (byte tel = 0; tel < KN_CNT; tel++) knipper[tel] = true, knopst[tel] = ledstNu[tel] = dimRicht[tel] = dimTelBl[tel] = dimTel[tel] = knopwacht[tel] = ledst[tel] = ledUp[tel] = 0, pinMode (knop[tel], INPUT_PULLUP), pinMode (led[tel], INPUT_PULLUP);
}

boolean knopCls::deLoop() {
  for (byte tol = 0; tol < KN_CNT; tol++) knopproc(tol);
  if (!dimCheck0) return false;
  dimCheck0 = false; boolean ja = false; for (byte t = 0; t < KN_CNT; t++) if (dimTel[t] == 5000) ja = true, dimTel[t] = 0, dimTelBl[t] = false;
  if (ja) return true;
  return false;
}

void knopCls::knopproc(byte no) {
  if (!knopwacht[no] && digitalRead (knop[no]) == knopst[no]) dod2F([](void* X, int y) {
    *((int*)X) = 3 + y;
  }, knopwacht + no), knopPoint[no] = (knopst[no] = !knopst[no]);
  /*
    if (knopwacht[no]) {
    if (knopst[no]) {
      if (digitalRead (knop[no])) {
        knopPoint[no] = false; knopst[no] = 0;
      }
    }
    else {
      knopwacht[no] = 0;
    }
    }
    else {
    if (!digitalRead (knop[no])) {
      knopPoint[no] = true; knopwacht[no] = 1; knopst[no] = 1;
    }
    }
  */
}

// kleine letter = uit groot aan
void knopCls::serEvFunc(char ch_0, char ch_1, char ch_2) {
  for (byte ttt = 0; ttt < 3; ttt++) {
    char knopMem; switch (ttt) {
      case 0: knopMem = ch_0; break;
      case 1: knopMem = ch_1; break;
      case 2: knopMem = ch_2; break;
    }
    if (knopMem >= 'A' and knopMem <= 'I') knopMem -= 'A', ledstNu[knopMem] = ledst[knopMem] = gelezen, ledUp[knopMem] = true, gelezen = 0;
    else if (knopMem >= 'A' and knopMem <= 'I') {
      knopMem -= 'A';
      if (gelezen) {
        dimTelBl[knopMem] = true, dimRicht[knopMem] = gelezen / 100 - 3,
        dod2F([](void* X, int y) {
          *((int*)X) = 0 - y;
        }, dimTel + knopMem); if (ledstNu[knopMem] != dimRicht[knopMem] * 255) ledUp[knopMem] = true, ledst[knopMem] = ledstNu[knopMem] + dimRicht[knopMem] * 1 - !dimRicht[knopMem] * 1,
          knipperproc([](int UUU) {
          dod2F([](void* X, int y) {
            *((int*)X) += y;
          }, &UUU); return UUU;
        }(timertel));
      }
      else dimTel[knopMem] = 0, dimTelBl[knopMem] = false, ledstNu[knopMem] = ledst[knopMem] = gelezen % 1000, ledUp[knopMem] = true;
    }
      else if (knopMem == 'X') if (!(gelezen % 1000)) gelezen += 400; else continue;
      else if (knopMem == 'x') if (!(gelezen % 1000)) gelezen += 300; else continue;
    else if ((knopMem >= 'a' and knopMem <= 'f') || (knopMem >= '0' && knopMem <= '9'))
        if (!gelezen) gelezen = knopMem; else gelezen = cv16waarde(gelezen, knopMem);
    else if (knopMem >= 'a' and knopMem <= 'i') {
      knopMem -= 'a', ledst[knopMem] = ledstNu[knopMem] = 0;
      digitalWrite (led[knopMem], HIGH);
    }
  }
}

void knopCls::timSetter(char knopMem) {
  timertel = (knopMem -= 't') % 2 * 100, knip3T = !(boolean)(knopMem / 2);
  if (!timerloopbool) timerloopbool = true;
}

void knopCls::timerloop(int tDiff) {
  timertel++; if (timertel >= 200) timertel = 0, knip3T = !knip3T;
  bool ja = false;
  auto FF = [&] {
    if (!ja && (ja = true))
      dod2F([](void* X, int y) {
      *((int*)X) += y;
    }, &tDiff);
  };
  for (byte t = 0; t < KN_CNT; t++) {
    if (knopwacht[t]) {
      FF();
      if (tDiff >= knopwacht[t]) knopwacht[t] = 0;
      else knopwacht[t]--;
    }
    if (!dimTelBl[t]) continue;
    FF();
    if (dimTel[t] < 512 && ++dimTel[t] + tDiff >= 512) dimTel[t] = 5000, dimCheck0 = true;
  }
}

void knopCls::knipperproc(int tDiff) {
  boolean knip = [&] (boolean knitp3T, int timetrtel) {
    if ((!knitp3T && (timetrtel < 67 || timetrtel >= 133)) || (knitp3T && timetrtel >= 67 && timetrtel < 133 )) return true;
    else return false;
  }(((tDiff / 200) % 2 && !knip3T) || (knip3T && !((tDiff / 200) % 2)), tDiff % 200);
  for (byte tel = 0, rek; tel < KN_CNT; tel++)
    if (!(rek = ledst[tel]))
      if (ledUp[tel] || !knipper[tel]) digitalWrite(led[tel], knipper[tel] = HIGH);
      else continue;
    else if (rek == 255)
      if (ledUp[tel] || knipper[tel] != knip) digitalWrite(led[tel], knipper[tel] = knip);
      else continue;
    else if ((rek = 100 - [&] {
    if (!(rek = rek * (unsigned int)(100) / 255)) return 1; else return (int)rek;
    }()) && knipper[tel] != (boolean)(rek = [&] {
    if (tDiff >= 200 - rek || tDiff < rek) return HIGH; else return LOW;
  }())) digitalWrite(led[tel], knipper[tel] = rek);
  for (byte t = 0; t < KN_CNT; t++) ledUp[t] = false;
}

void knopCls::knipDoorF(int tDiff) {
  dod2F([](void* x, int y) {
    *((int*)x) += y;
  }, &(tDiff));
  for (byte tel = 0, u; tel < KN_CNT; tel++)
    if (dimTelBl[tel] && dimTel[tel + tDiff > 1])
      if (dimRicht[tel]) {
        if (ledst[tel] < 255)
          if ((u = (dimTel[tel] + tDiff + ledstNu[tel]) % 255) >= 255) ledst[tel] = 255, ledUp[tel] = true;
          else if (u != ledst[tel]) ledst[tel] = u, ledUp[tel] = true;
      }
      else if (ledst[tel])
        if (tDiff + dimTel[tel] >= ledstNu[tel]) ledst[tel] = 0, ledUp[tel] = true;
        else if ((u = ledstNu[tel] - (tDiff + dimTel[tel])) != ledst[tel]) ledst[tel] = u, ledUp[tel] = true;
  tDiff += timertel, knipperproc(tDiff);
}
