#include "piScreen.h"
#include <SPI.h>
#include <Ethernet.h>
extern EthernetClient client;
extern char cvintcharchar(byte aant);
extern void dod2F (void (*x)(void*, int), void* y);
extern int dodTelMes;

piScreenCls::piScreenCls(int i, byte n) {
  pinMode(pin = i, OUTPUT), digitalWrite(pin, LOW), no = n;
}

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

void piScreenCls::func() {
  switch (mode) {
    case 0:
      if (set && !counting) // mode = ;
        digitalWrite(pin, HIGH), mode = 1, endF(500), counting = true;
      break;
    case 1:
      if (!counting) {
        char snd[] = { '@', 'P', 'S', cvintcharchar(no / 60), cvintcharchar(no % 60), 'A', 'N', '+', '\0' };
        client.print(snd); dodTelMes = 24000;
        endF(500), counting = true;
      }
      break;
    case 2:
      if (!set) mode = 5, endF(60000), counting = true;
      break;
    case 3:
      if (!counting) {
        char snd[] = { '@', 'P', 'S', cvintcharchar(no / 60), cvintcharchar(no % 60), 'U', 'T', '+', '\0' };
        client.print(snd); dodTelMes = 24000;
        endF(500), counting = true;
      }
      break;
    case 4:
      if (!counting) digitalWrite(pin, LOW), mode = 0, endF(3000), counting = true;
      break;
    case 5:
      if (set) counting = false, cnt = 0, mode = 2;
      else if (!counting) mode = 3, func();
      break;
  }
}

void piScreenCls::confirm(boolean IO) {
  if (IO && mode == 1)
    counting = false, cnt = 0, mode = 2;

  else if (!IO && mode == 3) counting = false, cnt = 0, mode = 4, endF(24000), counting = true;
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
