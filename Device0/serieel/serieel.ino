#include <TimerOne.h>
byte timertel = 0;
boolean knipper[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
const int knop[9] = {2, 4, 6, 8, 10, 12, A0, A2, A4};
const int led[9] = {3, 5, 7, 9, 11, 13, A1, A3, A5};
volatile byte ledst[81];
byte ledstNu[81];
boolean timerloopbool = 0;
int knopwacht[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
boolean knopst[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int dimTel[81]; volatile boolean dimCheck = false; boolean dimRicht[81];
boolean dimTelBl[81];
volatile boolean ledUp[81];

boolean reset = 1;
volatile boolean catveranderd = 0;
volatile boolean knip3T;
char knopMem;
byte decat = 0;
unsigned int gelezen = 0;
boolean delees = 0;
volatile boolean dodBool, dodBool3, dodBool2, dodBool1 = dodBool = dodBool3 = !(dodBool2 = true);
unsigned int dodTel = 0, dodTel2, dodTel3 = 0;
boolean tijdUpKnip; byte tijdUpTim;

void setup() {
  Serial.begin(74880);
  for (byte tel = 0; tel < 81; tel++) {
    if (tel < 9) {
      pinMode (knop[tel], INPUT_PULLUP);
      pinMode (led[tel], INPUT_PULLUP);
    }
    ledst[tel] = ledstNu[tel] = dimTelBl[tel] = ledUp[tel] = dimTel[tel] = 0;
  }
  Timer1.initialize(10000), Timer1.stop(), Timer1.restart();
}

void loop() {
  if (reset) {
    Serial.println("r");
    delay(5);
  }
  for (byte tol = 0; tol < 9; tol++) knopproc(tol);
  if (dodBool) dodF();
  else serialEventF();
  if (!dimCheck) return;
  dimCheck = false; boolean ja = false; for (byte t = 0; t < 81; t++) if (dimTel[t] == 5000) ja = true, dimTel[t]++;
  if (ja) Serial.println("r");
}

void dodF() {
  auto x = [] {
    if (dodBool3) dodBool3 = false, dodBool1 = true;
  };
  dodBool = dodBool2 = false;
  if (dodBool1) dodBool1 = false, dodTel++;
  bool routtBB = true;
  unsigned int tel; if (dodTel > 10) {
    tel = 10, dodTel -= 11;
    dodBool2 = dodBool = true;
    x();
  } else tel = dodTel, dodTel = 0, dodBool2 = true, x();
  {
    for (unsigned int dTel = 0; dTel <= tel; dTel++) {
      timerloop([&] { if (dodTel2 && dodTel2 - 1 == dTel) return true; return false; }(), tel - dTel);
      if (routtBB && dTel == dodTel3) routtBB = false, dod2F([](void* X, int y) {
        if (y) dodTel3 = y - 1;
        else dodTel3 = 0;
      }, NULL), knipDoorF(tel - dTel);
      serialEventF();
    }
  }
  if (dodTel2 > 10) dodTel2 -= 11;
  else if (dodTel2) dodTel2 = 0;
  if (routtBB && dodTel3 > 10) dodTel3 -= 11;
}

void knopproc(byte no) {
  if (!knopwacht[no] && digitalRead (knop[no]) == knopst[no]) dod2F([](void* X, int y) {
    *((int*)X) = 3 + y;
  }, knopwacht + no), Serial.write(no + 'A' + (!(knopst[no] = !knopst[no]) * 32)), Serial.write('\n');
}

void dod2F (void (*x)(void*, int), void* y) {
  dodBool2 = false;
  if (dodBool1 && (dodBool1 = false))
    if (!dodBool) dodBool = true;
    else if (dodTel > 2000) dodTel = 0;
    else dodTel++;
  x(y, dodBool * 1 + dodTel), dodBool2 = true;
  if (dodBool3) dodBool3 = false, dodBool1 = true;
}
// kleine letter = uit groot aan
void serialEventF() {
  char inChar;
  while (Serial.available()) {
    if ((inChar = Serial.read()) != '\n') {
      if (inChar != '\r') knopMem = inChar; continue;
    }
    else if (knopMem >= 'A' and knopMem <= 'I') {
      knopMem = (gelezen / 1000) * 9 + (knopMem - 'A');
      if (gelezen % 1000 > 299) {
        dimTelBl[knopMem] = true, dimRicht[knopMem] = gelezen % 1000 / 100 - 3,
        dod2F([](void* X, int y) {
          *((int*)X) = 0 - y;
        }, dimTel + knopMem); if (ledstNu[knopMem] != dimRicht[knopMem] * 255) ledUp[knopMem] = true, ledst[knopMem] = ledstNu[knopMem] + dimRicht[knopMem] * 1 - !dimRicht[knopMem] * 1,
          knipperproc([](int UUU) {
          dod2F([](void* X, int y) {
            *((int*)X) += y;
          }, &UUU); return UUU;
        }(timertel));
      }
      else dimTel[knopMem] = dimTelBl[knopMem] = false, ledst[knopMem] = ledstNu[knopMem] = gelezen % 1000, ledUp[knopMem] = true;
    }
      else if ((knopMem >= 'a' and knopMem <= 'f') || (knopMem >= '0' && knopMem <= '9')) if (!gelezen % 1000) gelezen += knopMem; else gelezen = gelezen / 1000 * 1000 + cv16waarde(gelezen % 1000, knopMem);
      else if (knopMem == 'X') if (!(gelezen % 1000)) gelezen += 400; else continue;
      else if (knopMem == 'x') if (!(gelezen % 1000)) gelezen += 300; else continue;
    else if (knopMem >= 'J' && knopMem <= 'R') {
      decat = knopMem - 'J';
      catveranderd = 1;
    }
    else if (knopMem >= 'j' && knopMem <= 'r') {
      gelezen = 1000 * (knopMem - 'j');
    }
    else if (knopMem >= 't' && knopMem <= 'w') {
      if (!timerloopbool) timerloopbool = true, timertel = (knopMem -= 't') % 2 * 100, knip3T = !(boolean)(knopMem / 2), Timer1.start(), Timer1.attachInterrupt([] {
        if (!dodBool2 && (dodBool3 = true)) return;
        if (dodBool1 && (dodBool1 = false))
          if (!dodBool) dodBool = true;
          else if (dodTel > 2000) dodTel = 0;
          else dodTel++;
        if (dodBool)
          if (dodTel > 2000) dodTel = 0;
          else dodTel++;
        dodBool = true;
      });
      else tijdUpTim = (knopMem -= 't') % 2 * 100, tijdUpKnip = !(boolean)(knopMem / 2), dod2F([](void* X, int y) {
        dodTel2 = y + 1;
      }, NULL);
    }
    else {
      switch (knopMem) {
        case 'S':
          if (!reset) for (byte tel = (decat = reset++); tel < 81; tel++) ledst[tel] = ledstNu[tel] = 0;
          catveranderd = true;
          break;
        case 's':
          reset = 0;
          break;
      }
    }
    knopMem = '\n';
  }
}

void timerloop(boolean tijdUp, int tDiff) {
  if (tijdUp) tijdUp = false, knip3T = tijdUpKnip, timertel = tijdUpTim;
  timertel++; if (timertel >= 200) timertel = 0, knip3T = !knip3T;
  bool ja = false;
  auto FF = [&] {
    if (!ja && (ja = true))
      dod2F([](void* X, int y) {
      *((int*)X) += y;
    }, &tDiff);
  };
  for (byte t = 0; t < 81; t++) {
    if (t < 9 && knopwacht[t]) {
      FF();
      if (tDiff >= knopwacht[t]) knopwacht[t] = 0;
      else knopwacht[t]--;
    }
    if (!dimTelBl[t]) continue;
    FF();
    if (dimTel[t] < 512 && ++dimTel[t] + tDiff >= 512) dimTel[t] = 5000, dimCheck = true;
  }
}

void knipDoorF(int tDiff) {
  dod2F([](void* x, int y) {
    *((int*)x) += y;
  }, &(tDiff));
  for (byte tel = 0, u; tel < 81; tel++)
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

//;

void knipperproc(int tDiff) {
  boolean knip = [&] (boolean knitp3T, int timetrtel) {
    if ((!knitp3T && (timetrtel < 67 || timetrtel >= 133)) || (knitp3T && timetrtel >= 67 && timetrtel < 133 )) return true;
    else return false;
  }(((tDiff / 200) % 2 && !knip3T) || (knip3T && !((tDiff / 200) % 2)), tDiff % 200);
  for (byte tel = 0, rek, up; tel < 9 && ((up = ledUp[tel + decat * 9]) || !up); tel++)
    if (!(rek = ledst[tel + decat * 9]))
      if (up || catveranderd || !knipper[tel]) digitalWrite(led[tel], knipper[tel] = HIGH);
      else continue;
    else if (rek == 255)
      if (up || catveranderd || knipper[tel] != knip) digitalWrite(led[tel], knipper[tel] = knip);
      else continue;
    else if ((rek = 100 - [&] {
    if (!(rek = rek * (unsigned int)(100) / 255)) return 1; else return (int)rek;
    }()) && knipper[tel] != (boolean)(rek = [&] {
    if (tDiff >= 200 - rek || tDiff < rek) return HIGH; else return LOW;
  }())) digitalWrite(led[tel], knipper[tel] = rek);
  if (catveranderd) catveranderd = false;
  for (byte t = 0; t < 81; t++) ledUp[t] = false;
}

byte cv16waarde(char char0, char char1) {
  byte uitkomst;
  if (char1 >= 'a' && char1 <= 'f') {
    uitkomst = 10 + (char1 - 'a');
  }
  else {
    uitkomst = char1 - '0';
  }
  if (char0 >= 'a' && char0 <= 'f') {
    uitkomst = uitkomst + ((10 + (char0 - 'a')) * 16);
  }
  else {
    uitkomst = uitkomst + ((char0 - '0') * 16);
  }
  return uitkomst;
}
