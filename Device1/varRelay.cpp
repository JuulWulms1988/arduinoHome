#include "varRelay.h"
extern void dod2F (void (*x)(void*, int), void* y);

varRelayCls::varRelayCls(byte pinVar, unsigned int onVar, unsigned int offVar) {
  pinMode(pinVar, OUTPUT), digitalWrite(relayPin = pinVar, LOW), maxOn = onVar, maxOff = offVar;
}

void varRelayCls::func(boolean nowVar) {
  if (!staat)
    if (wacht) {
      if (!timTelt) wacht = false;
    }
    else {
      if (nowVar) digitalWrite(relayPin, staat = true);
    }
  else {
    if (wacht) {
      if (nowVar) timTelt = (wacht = false);
      else if (!timTelt) digitalWrite(relayPin, staat = false), dod2F([](void* X, int y) {
        *((int*)X) = y;
      }, &timTel), timTel2 = 100 - (timTel % 100), timTel = maxF() - maxOff - ((boolean)(timTel) * (1 + timTel / 100)), timTelt = true;
    }
    else if (!nowVar) dod2F([](void* X, int y) {
      *((int*)X) = y;
    }, &timTel), timTel2 = 100 - (timTel % 100), timTel = maxF() - maxOn - ((boolean)(timTel) * (1 + timTel / 100)), timTelt = (wacht = true);
  }
}

void varRelayCls::timFunc(int tDiff) {
  if (!timTelt) return;
  if (!(timTel2 = (timTel2 + 1) % 100)) ++timTel;
  timTelt = (tDiff % 100 + timTel2 >= 100) + tDiff / 100 + timTel < maxF();
}
