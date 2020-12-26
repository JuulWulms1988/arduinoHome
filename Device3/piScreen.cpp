#include "piScreen.h"
#include <SPI.h>
#include <Ethernet.h>
#include <IRremote.h>

extern EthernetClient client;
extern char cvintcharchar(byte aant);
extern void dod2F (void (*x)(void*, int), void* y);
extern int dodTelMes;

enum remoteMode { REM_MODE_NEC, REM_MODE_RC_ON, REM_MODE_RC_OFF };

IRsend IRSend;

static void irSendF(remoteMode sw) {
  switch (sw) {
    case REM_MODE_NEC: IRSend.sendNEC(0x7da05f, 32); break;
    case REM_MODE_RC_ON: IRSend.sendRC6(0x1000C, 20); break;
    case REM_MODE_RC_OFF: IRSend.sendRC6(0xC, 20); break;
  }
}

piScreenCls::piScreenCls(int i, psNo n) {
  pinMode(pin = i, OUTPUT), digitalWrite(pin, pin == A2 ? HIGH : LOW), no = n;
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

void piScreenCls::sendUtF() {
  char snd[] = { '@', 'P', 'S', cvintcharchar(no / 60), cvintcharchar(no % 60), 'U', 'T', '+', '\0' };
  client.print(snd); dodTelMes = 24000;
}

void piScreenCls::func() {
  static byte irFlag = 0;
  switch (mode) {
    case 0:
      if (set && !counting) mode = 5,	digitalWrite(pin, pin == A2 ? LOW : HIGH), endF(no == PSNO_0 ? 500 : 2500), counting = true;
      break;
    case 1:
      if (!counting) {
        char snd[] = { '@', 'P', 'S', cvintcharchar(no / 60), cvintcharchar(no % 60), 'A', 'N', '+', '\0' };
        client.print(snd); dodTelMes = 24000;
        endF(500), counting = true;
      }
      break;
    case 2: // Heej TESTTESTTESTTEST
      if (!set && !counting)
        if ((irFlag = irFlag ? no == PSNO_1 ? 2 : 0 : 1) == 1)
          sendUtF(), mode = 7, endF(50), irSendF(no == PSNO_1 ? REM_MODE_RC_OFF : REM_MODE_NEC), counting = true;
        else
          mode = 3, func();
      break;
    case 3:
      if (!counting) sendUtF(), endF(500), counting = true;
      break;
    case 4:
      if (!counting) digitalWrite(pin, pin == A2 ? HIGH : LOW), mode = 0, endF(1000), counting = true;
      break;
    case 5:
      if (!counting) mode = (irFlag = irFlag ? 0 : 1) ? (endF(50), irSendF(no == PSNO_1 ? REM_MODE_RC_ON : REM_MODE_NEC), 6) : (endF(no == PSNO_0 ? 11500 : 9500), 2), counting = true;
      break;
    case 6:
      if (counting) break;
      if (!irFlag) irSendF(no == PSNO_0 ? REM_MODE_RC_ON : REM_MODE_NEC);
      else if (irFlag == 2) irFlag = 0, irSendF(REM_MODE_RC_OFF);
      else irFlag = 0;
      endF(no == PSNO_0 ? 11450 : 9450);
      mode = 2;
      counting = true;
      break;
    default:
      if (counting) break;
      if (!irFlag) irSendF(no == PSNO_0 ? REM_MODE_RC_ON : REM_MODE_NEC);
      else if (irFlag == 2) irFlag = 0, irSendF(REM_MODE_RC_OFF);
      else irFlag = 0;
      if (mode == 7) mode = 3, endF(450), counting = true;
      else mode = 4, endF(3000), counting = true;
      break;
  }
}

void piScreenCls::confirm(boolean IO) {
  if (IO && mode == 1) counting = false, cnt = 0, mode = 2;
  else if (!IO)
    if (mode == 3) counting = false, cnt = 0, mode = 4, endF(3000), counting = true;
    else if (mode == 7) mode = 8;
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
