#include "serKnop.h"
#include <SPI.h>
#include <Ethernet.h>

extern EthernetClient client;
extern byte hetadres;
extern char lees[31];
extern char cv16char0(byte waarde);
extern char cv16char1(byte waarde);
extern void dod2F(void (*x)(void*, int), void* y);
extern int dodTelMes;

serKnCls serKn;

void serKnCls::setF() {
  Serial.begin(74880);
  for (byte t = 0; t < KN_CNT; t++) knop[t] = knopst[t] = knoptel2[t] = knopuitgestuurd2[t] = knopdim2[t] = knopuitDimgestuurd[t] = knopnawacht[t] = 0;
  char inChar = '~';
  Serial.println("S");
  boolean esc = false;
  while (!esc)
    while (Serial.available())
      if ((inChar = (char)Serial.read()) != '\n') {
        if (inChar != '\r') knopMem = inChar;
      }
      else if (knopMem == 'r') {
        Serial.println("s");
        esc = true;
        break;
      }
}

void serKnCls::netwSet() {
  for (byte tel = 0; tel < KN_CNT; tel++) knopdim2[tel] = knoptel2[tel] = knopnawacht[tel] = 0;
}

void serKnCls::verwerk(byte depin) {
  if (lees[4] == 'U' && lees[5] == 'T') lees[4] = lees[5] = '0'; else if (lees[4] == 'A' && lees[5] == 'N') lees[4] = lees[5] = 'f';
  else if (lees[4] == 'a' && lees[5] == 'n') lees[4] = lees[5] = 'X';
  else if (lees[4] == 'u' && lees[5] == 't') lees[4] = lees[5] = 'x';
  Serial.println(lees[4]), Serial.println(lees[5]);
  Serial.write((lees[8] - '0') * 10 + (lees[9] - '0') + 'A'), Serial.println();
}

void serKnCls::updateF() {
  char inChar = '~';
  do
    if ((inChar = (char)Serial.read()) != '\n') {
      if (inChar != '\r') knopMem = inChar;
    }
    else if (knopMem >= 'a' and knopMem <= 'i') knop[knopMem - 'a'] = 0;
    else if (knopMem >= 'A' and knopMem <= 'I') knop[knopMem - 'A'] = 1;
    else if (knopMem == 'r') {
      Serial.println("s");
      char snd[] {'#', 'S', 'V', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), '+', '\0'};
      client.print(snd); dodTelMes = 24000;
    } while (Serial.available());
}

void serKnCls::procKnop() {
  int tDiff, knopTelN;
   dod2F([](void* x, int y) {
      *((int*)x) = y;
    }, &tDiff);
  for (byte no = 0; no < KN_CNT; no++) {
    knopTelN = tDiff + knoptel2[no];
    dod2F([](void* x, int y) {
      *((int*)x) += y;
    }, &(knopTelN = knoptel2[no] + tDiff));
    if (knopst[no]) {
      if (knopTelN >= 100 && !knopuitgestuurd2[no]) {
        char snd[32] = {'@', 'S', 'I', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'U', 'T', 'K', 'N' };
        knopuitgestuurd2[no] = 1; sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000;
      }
      if ((!knop[no] && knopTelN > 3) || knopTelN >= 255) {
        char snd[32] = {'@', 'S', 'I', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'U', 'T', 'K', 'N' };
        if (!knopuitDimgestuurd[no]) {
          knopuitDimgestuurd[no] = true;
          snd[5] = cv16char0(knopTelN), snd[2] = 'u';
          if (!knopdim2[no]) snd[6] = cv16char1(knopTelN); else snd[6] = knopTelN % 16 + 'A';
          sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000;
        }
        if (knop[no] || knopTelN <= 3) return;
        knopst[no] = 0;
        if (knopTelN < 100) snd[2] = snd[5] = 'I', snd[6] = 'D', sluitknpr(no, snd + 9), client.print(snd), dodTelMes = 24000;
        knopdim2[no] = !knopdim2[no]; knoptel2[no] = -tDiff; knopuitDimgestuurd[no] = knopuitgestuurd2[no] = 0; knopnawacht[no] = 1;
      }
    }
    else {
      if (knopnawacht[no] && knopTelN >= 30) {
        knopnawacht[no] = 0; knoptel2[no] = 0;
      }
      else {
        if (knop[no]) {
          char snd[32] = {'@', 'S', 'u', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'A', 'N', 'K', 'N' };
          if (!knopdim2[no]) snd[5] = 'U', snd[6] = 'T';
          sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000; knoptel2[no] = -tDiff; knopst[no] = 1; knopuitgestuurd2[no] = 0;
        }
      }
    }
  }
}

void serKnCls::timF() {
  for (byte tel = 0; tel < KN_CNT; tel++) if ((knopst[tel] || knopnawacht[tel]) && knoptel2[tel] < 255) knoptel2[tel]++;
}
