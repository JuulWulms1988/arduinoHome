#ifndef LedPlaf_h
#define LedPlaf_h
#include "basprc.h"
#define STRIP_CNT 11
#define STRIP_STRT 0
#define STRIP_END 3
#define STRIP_AANT 4

#define LEDPL_VARAANT 14
#define LEDPL_VARAANT1 4
#define LEDPL_VARAANT2 1

extern const byte pinTim; extern boolean ledPlafLoop(); class ledPlaf {
  public: ledPlaf(); ~ledPlaf();// byte ffTelSer = 0, telFFF2 = 59;
    volatile boolean int_up = false;
    int int_wacht = 0;
    unsigned int int_tW[STRIP_AANT];
    byte int_now = 0;
    byte int_ledValNext = 0;
    byte int_cnt = 0;
    volatile boolean int_flag = false;
    void interruptF();
    unsigned long int_nieuwTel = 0;
    static int telDifF(unsigned long x, unsigned long y);
    static byte pinF(byte x) {
      if (x < 3) return 44 + x; if (x < 5) return x - 1; return x;
    } ledPlaf(boolean x); boolean serLoop();  private: friend class objS; byte cntStrip = STRIP_AANT - 1; byte clrUp = 0; static byte* kleurF(byte x, byte* y) {
      for (byte t = 0; t < 3; t++) y[t] = kleur(x, t); return y;
    } boolean serBootF0(boolean x); void serBootF1(); byte vergStrip(byte x) {
      if (x >= STRIP_STRT && x <= STRIP_END) return 0; else if (x < STRIP_STRT) return 1; return 2;
    } void serVoeg(char y, bool ser) {
      if (ser) Serial2.write(y); else Serial3.write(y);
    } void serVoeg2(char* y, byte l, bool ser) {
      if (ser) Serial2.write(y, l); else Serial3.write(y, l);
    } void serVoegMes(boolean ser, char aanv, byte bron, char* mes, unsigned int l) {
      serVoeg(basprc::cvintcharchar(bron), ser); if (ser) Serial2.write(aanv); else Serial3.write(aanv); serVoeg2(mes, l, ser), serVoeg('\n', ser);
    } boolean serSetSerDoor(byte bron, boolean ser, byte sP, byte* sPA); byte serNul (boolean ser) {
      serBron[ser] = 255, serNow[ser] = '\0'; return serTel[ser] = serOpn[ser] = serNo[ser] = 0;
    } void serOpnF(boolean ser) {
      serVoeg(basprc::cvintcharchar(serBron[ser]), !ser), serVoeg(serNow[ser], !ser);
    } unsigned long cvIntStr(boolean ser) {
      return basprc::cvintcharStr(serBuf[ser]);
	} void serStClr(boolean ser); void serStNClr(boolean ser); boolean serDetFunc(byte strNo); void serDetach(boolean ser); void serSendAtMess(byte ontCl, void* varObj, boolean ser); void serReadAtMess(boolean ser); void serAttach(boolean ser); void serSetSer(boolean ser); void serSetSerPrep(boolean ser, byte sNo, byte sP); void clrSerSendF();
    void serStVarF(boolean ser); void serVoeg4(boolean ser, char* x, byte l); void serVoeg3(boolean ser) {
      serVoeg4(ser, serBuf[ser], serTel[ser]);
    } byte servChldAr[2][STRIP_CNT - 1]; byte servChld[2]; boolean serFunc(boolean ser); void serReset(boolean ser);
    void serOffUp(boolean ser); boolean serRead(char thisChar, boolean ser); char thisCharF(boolean ser) {
      if (ser) return (char)Serial2.read(); return (char)Serial3.read();
    } byte serTel[2]; boolean serLees[2] {false, false}; char serBuf[2][63]; char serSBuf[2][63]; unsigned int serST[2]; byte serVal[2]; char serNow[2] {'\0', '\0'};
    unsigned int serNo[2] {0, 0}; boolean serOpn[2] {0, 0}; byte serBron[2]; unsigned long tel = 1; byte partOfCl(byte cl, volatile boolean*& cL); class objS {
      private: friend class ledPlaf; static byte rndExPr(byte *e, byte eC, byte t); void clrUpF(); byte clrUp; volatile boolean clrUpBl = false; byte clrSUp = 0; byte clrRUp = 0; void raiseTimeF(int tPass, unsigned long tel); struct delaytWS {
          unsigned int tW; byte zet(unsigned int& sTW, byte aDl, unsigned int dlay, unsigned int per) {
            byte tel = 0;
            for (byte t = 0; t < aDl; t++) {
              unsigned int del = dlay; if (del > per) tel += del / per, del %= per; if (del > sTW) tel++, sTW = per - (del - sTW); else sTW -= del;
            } return tel;
          } byte waarde[3];
        }; objS(); objS(byte x, byte* y); void changeParChld(boolean what, byte strNo) {
          if (what) { if (!parChldAr[strNo]) parChldAr[strNo] = true, parChld++; } else if (parChldAr[strNo]) parChld--, parChldAr[strNo] = false; } objS(objS& varOb, objS& parOb); boolean offUp = false; boolean offSUp = false; boolean offSerSendF();
        boolean clrF(byte X, byte Y); void clrF1(byte X, byte Y); void clrF2(byte X, byte Y) {
          volatile byte* x = kleurAr[X]; volatile byte* y = kleurAr[Y]; for (byte t = 1; t < STRIP_CNT; t++) y[t] = x[t - 1]; byte p[1] {y[1]}, z = 1; y[0] = rndClr(p, z);
        } unsigned int func(boolean offV, unsigned long tel); void funcW(delaytWS wS); void waardeFunc(delaytWS* wS); boolean gamFunc(delaytWS* wS, byte& out); void heldFunc(delaytWS* wS, boolean helKB, byte& helK); void gamTelWFunc(byte tt, byte& gamW) {
          if ((tt %= gamInterval) > gamW) gamW = gamInterval - (tt - gamW); else gamW -= tt;
        } void gamTelFunc(unsigned long tt, unsigned int& tW, byte& gamW) {
          if (tt > per) gamTelWFunc(tt / per, gamW), tt %= per; if (tt > tW) gamTelWFunc(1, gamW), tW = per - (tt - tW); else tW -= tt;
        } boolean telResetF(unsigned long tel); void telResNl(); void setOffF(unsigned long tel, unsigned int& tW); void setClrFunc(); unsigned long bufTRes[2] {0, 0}; void setSerChld(byte sP, byte* sPA) {
          for (byte t = 0; t < sP; t++) parChldAr[sPA[t]] = true;
        } byte no; byte ledNu [3] {0, 0, 0}; volatile byte helder = 0; volatile unsigned int dlay = 0; volatile boolean negDlay = false;
        volatile unsigned int gamDlay = 0; volatile boolean negGamDlay = false; volatile unsigned int gamPer = 0; volatile byte gamPauze = 0; volatile byte gamClr = 0; volatile byte gamVal = 0; volatile byte gamIVal = 0; volatile byte gamIHel = 0; volatile byte helAmp = 0; volatile unsigned int helPer = 0; unsigned int helCur = 0; volatile unsigned int helDlay = 0; volatile boolean negHelCur = false;
        byte getGam; byte getPos; unsigned int getHelCur; unsigned long getTel; unsigned long getRes[2]; unsigned long getOff; unsigned long getOffT; volatile byte gamInterval = 0; byte gamIntWait = 0;
        volatile byte rgbPauze = 0; byte clrPos = 0; byte clrPsF(byte x) {
			return (clrPos + x) % (STRIP_CNT + 3); 
        } unsigned long getVarF(byte vNo); volatile boolean settedColor = false; unsigned long tOffset = 0; unsigned long tOffTel = 0; volatile unsigned int per = 0; volatile byte parChld = 1;
        volatile boolean parChldAr[STRIP_CNT]; volatile byte kleurAr[STRIP_CNT + 3][STRIP_CNT]; byte parChldPos(); void stClrNll () {
          clrNlF(); for (byte t = 0; t < STRIP_CNT; t++)
            if (t != no) parChldAr[t] = false;
            else parChldAr[t] = true;
        } void clrNlF() {
          for (byte t = 0; t < STRIP_CNT + 3; t++) for (byte T = 0; T < STRIP_CNT; T++) kleurAr[t][T] = 0;
        } volatile byte randVar = 0; unsigned int perF() {
          return dlay * (STRIP_CNT - 1) + per;
        } byte parChldC(byte t) {
          for (byte a = 0, b = 0; b < parChld; a++) if (parChldAr[a] && b++ == t) return a;
        } unsigned int telW(unsigned long tel); unsigned long telD (unsigned long tel) {
          if (tOffset > tel) return tel + 1 + ((unsigned long)(0 - 1) - tOffset); return tel - tOffset;
        } static byte rndClr(byte* e, byte eC); void clrUpReg(volatile byte* ar, byte no) {
          volatile byte* a = kleurAr[no]; for (byte t = 0; t < STRIP_CNT; t++) a[t] = ar[t];
        } void offCl(unsigned long off, byte gam, unsigned long offT, unsigned long* bufR) {
          tOffset = off, gamIntWait = gam; tOffTel = offT; settedColor = true; bufTRes[0] = bufR[0]; bufTRes[1] = bufR[1];
        } void nullTim(unsigned long tel, unsigned int& tW); void serVarSetOb(byte gVar, unsigned long gVal); void setFullClrF();
    } obj[STRIP_AANT]; void cpClrAll(volatile byte** bron, volatile byte** des); boolean clrSerSendPrepF(byte obNo, objS& p); objS& resetCls(byte bron) {
      objS& x = obj[bron - STRIP_STRT]; return x = objS(bron, x.ledNu);
    } void serSetSerSIntern(byte bron, byte serBron, byte sP, byte* sPA) {
      objS& x = resetCls(bron); x.parChld = 1 + sP, x.parChldAr[serBron] = true; if (sP) x.setSerChld(sP, sPA);
    }  static byte kleur (byte x, byte y); void constr(); void constr2();
};

#endif
