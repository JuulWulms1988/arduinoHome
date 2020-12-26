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
      unsigned long uitkomst = 1; for (byte tel = 0; tel < x; tel++) uitkomst = uitkomst * macht; return uitkomst;
    } static unsigned long cvintcharStr(char* x) {
      byte len = charPLen(x); unsigned long y = 0; for (byte tel = 0; tel < len; tel++) y += cvintcharint(*(x + tel)) * (macht(60, len - 1 - tel)); return y;
    } static unsigned int cvCharLngth(byte stelsel, unsigned long getal, char* stuur) {
      unsigned int dec = 0; {
        unsigned long x = getal;
        do {
          dec++;
        } while (x /= stelsel);
      } for (unsigned int t = dec; t--; * (stuur + t) = cvintcharchar(getal % 60), getal /= 60); return dec;
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
      if (val == nVal) return; auto A = [&](byte RR) {
        unsigned int a = (unsigned int)(prt) * RR / 255;
        if (!a) return 1;
        else return (int)a;
      }; if (nVal < val) val -= A(val - nVal); else val += A(nVal - val);
    } static void cvRaise2(byte& val, byte nVal, byte prt) {
      if (val == nVal) return; if (nVal < val) val -= (unsigned int)(prt) * (val - nVal) / 255; else val += (unsigned int)(prt) * (nVal - val) / 255;
    }
};

#endif

