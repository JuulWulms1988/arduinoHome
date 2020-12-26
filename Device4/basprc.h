#ifndef basprc_h
#define basprc_h
#include "Arduino.h"

class basprc {
  public:
    static byte cvintcharint (char x) {
      if (x <= '9') return x - '0'; else if (x <= 'Z') return x - 55; else return x - 61;
    } static char cvintcharchar(byte aant) {
      if (aant < 10) return aant + '0'; else if (aant < 36) return aant + 55; else return aant + 61;
    } static unsigned long macht(byte macht, byte x) {
      if (x == 0) return 1; unsigned long uitkomst = macht; for (byte tel = 0; tel < x - 1; tel++) uitkomst = uitkomst * macht; return uitkomst;
    } static unsigned long cvintcharStr(char* x) {
      byte len = charPLen(x); unsigned long y = 0; for (byte tel = 0; tel < len; tel++) y += cvintcharint(*(x + tel)) * (macht(60, len - 1 - tel)); return y;
    } static unsigned int cvCharLngth(byte stelsel, unsigned int getal, char* stuur) {
      unsigned int dec[2] = { 0, 0 }; do dec[0]++; while (getal / macht(stelsel, dec[0])); for (unsigned int tel = dec[0] - 1; tel < dec[0]; tel--) *(stuur + dec[1]++) = cvintcharchar(getal / macht(stelsel, tel) % stelsel); return dec[0];
    } static unsigned int charPLen(char* x) {
      byte y = 0; while (*(x + y) != '\0') y++; return y;
    } static void longASom (unsigned long* x, unsigned long y) {
      if (x[2] > (unsigned long)(0 - 1) - y) x[0]++; x[1] += y;
    } static byte partVal(unsigned int x, unsigned int y) {
      return x * (unsigned long)(255) / y;
    } static byte cosRaise(byte x) {
      return (1 + cos(PI * (1 + x / (float)(255)))) / 2 * 255;
    } static byte cosRaise2(byte x) {
      return (1 + cos(PI * (1 + x / (float)(255) * 2))) / 2 * 255;
    } static byte partValR(unsigned int x, unsigned int y) {
      return cosRaise(partVal(x, y));
    } static byte partValR2(unsigned int x, unsigned int y) {
      return cosRaise2(partVal(x, y));
    } static void cvRaise(byte& val, byte nVal, byte prt) {
      if (val == nVal) return; if (nVal < val) val -= 1 + (unsigned int)(prt) * (val - 1 - nVal) / 255; else val += 1 + (unsigned int)(prt) * (nVal - 1 - val) / 255;
    } static void cvRaise2(byte& val, byte nVal, byte prt) {
      if (val == nVal) return; if (nVal < val) val -= (unsigned int)(prt) * (val - nVal) / 255; else val += (unsigned int)(prt) * (nVal - val) / 255;
    }
};

#endif

