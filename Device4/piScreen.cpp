#include "piScreen.h"
#include <SPI.h>
#include <Ethernet.h>


extern EthernetClient client;
extern char cvintcharchar(byte aant);
extern void sluisTv(boolean IO);
extern boolean tv2Var;
extern boolean tv3Var;
extern void dod2F (void (*x)(void*, int), void* y);
extern int dodTelMes;

struct dodPiScS {
  unsigned long& x;
  unsigned long y;
  void func (unsigned int z) {
    x = y + z;
  }
};

void piScreenCls::endF(unsigned long x) {
  dodPiScS y{ cntEnd, x };
  dod2F([](void* X, int y) {
    ((dodPiScS*)X)->func(y);
  }, &y);
}

piScreenCls::piScreenCls(int i, byte n) {
  pinMode(pin = i, OUTPUT), digitalWrite(pin, LOW), no = n;
}


void piScreenCls::func() {
  switch (mode) {
    case 0:
      if (set && !counting) {
        tv3Var = true;
        if (!tv2Var) sluisTv(true);
        mode = 2,	digitalWrite(pin, HIGH), endF(12000), counting = true;
      }
      break;
    case 1:
      if (!counting) {
        char snd[] = { '@', 'P', 'S', cvintcharchar(no / 60), cvintcharchar(no % 60), 'A', 'N', '+', '\0' };
        client.print(snd); dodTelMes = 24000;
        endF(500), counting = true;
      }
      break;
    case 2: // Heej TESTTESTTESTTEST
      if (!(!set && !counting)) break;
      tv3Var = false;
      if (!tv2Var) sluisTv(false);
      mode = 3, func();
      break;
    case 3:
      if (!counting) {
        char snd[] = { '@', 'P', 'S', cvintcharchar(no / 60), cvintcharchar(no % 60), 'U', 'T', '+', '\0' };
        client.print(snd); dodTelMes = 24000;
        endF(500), counting = true;
      }
      break;
    case 4:
      if (!counting) digitalWrite(pin, LOW), mode = 0, endF(1000), counting = true;
      break;
  }
}

void piScreenCls::confirm(boolean IO) {
  if (IO && mode == 1) counting = false, cnt = 0, mode = 2;
  else if (!IO && mode == 3) counting = false, cnt = 0, mode = 4, endF(3000), counting = true;
}

void piScreenCls::cntF(int tDiff) {
  if (!counting) return;
  int Y;
  dod2F([](void* X, int y) {
      *((int*)X) = y;
    }, &Y);
  if (Y + cnt + tDiff >= cntEnd) cnt = 0, counting = false;
  else cnt++;
}
