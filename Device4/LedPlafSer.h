#ifndef LedPlafSer_h
#define LedPlafSer_h
#include "Arduino.h"
#include "basprc.h"
#include <Ethernet.h>
#define PIN_PLAFRELAY 6
extern const int hetadres;
extern EthernetClient client;
extern volatile boolean plafBool;
extern volatile boolean serBeginBool1;
extern int dodTelMes;

class ledPlafSerCls {
  public:
    byte upM = 0;
    byte upD = 0;
    byte upU = 0;
    byte upMi = 0;
    byte upS = 0;
    unsigned long upN = 0;
    ledPlafSerCls();
    void serLs (char thisChar);
    void func();
    byte bufTel = 5; byte strtW = 0;
    void openF(){
      strtW = 3;
      char ss[32]; for (byte t = 0; t < 5; t++) ss[t] = buf[t]; byte ssl = 5; ss[ssl++] = '&';
      for (byte t = 0; t < 6; t++) { unsigned long in; switch (t) {
        case 0: in = upM; break;
        case 1: in = upD; break;
        case 2: in = upU; break;
        case 3: in = upMi; break;
        case 4: in = upS; break;
        case 5: in = upN; break;
        }
      ss[(ssl += basprc::cvCharLngth(60, in, ss + ssl))++] = '\\';
      } ss[ssl - 1] = '+', ss[ssl] = '\0';
      client.print(ss); dodTelMes = 24000;
      }
  private:
    char buf[32] {'@', 'S', 'P', char (hetadres / 10 + '0'), char (hetadres % 10 + '0') };
    void sendBuf() {
      buf[bufTel++] = '+', buf[bufTel] = '\0', client.print(buf), dodTelMes = 24000, bufTel = 5;
    } void bufLs(char thisChar);
};

#endif
