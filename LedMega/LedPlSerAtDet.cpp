#include "LedPlaf.h"

boolean ledPlaf::serDetFunc(byte strNo) {
	boolean doorstuur = false;
	volatile boolean* cL; byte c = partOfCl(strNo, cL);
	boolean clAr[STRIP_CNT]; for (byte tTt = 0; tTt < STRIP_CNT; tTt++) clAr[tTt] = cL[tTt];
	if (!c) doorstuur = true;
	for (byte t, tt = t = 0; tt < c; t++)
		if (!clAr[t] || (t < STRIP_STRT && ++tt) || !++tt) continue;
		else if (t == strNo && t >= STRIP_STRT && t <= STRIP_END) {
			volatile boolean* a = obj[t - STRIP_STRT].parChldAr; obj[t - STRIP_STRT].parChld = 1;
			for (byte TT, T = TT = 0; TT < c; T++)
				if ((T == t && ++TT) || !a[T] || !++TT) continue;
				else a[T] = false;
		}
		else if (t >= STRIP_STRT && t <= STRIP_END) 
		obj[t - STRIP_STRT].changeParChld(false, strNo);
		else {
			doorstuur = true; break;
		}
		return doorstuur;
}

void ledPlaf::serDetach(boolean ser) {
  if (serDetFunc(serBron[ser])) serVoeg(basprc::cvintcharchar(serBron[ser]), !ser), serVoeg('D', !ser), serVoeg('\n', !ser);
}

void ledPlaf::serAttach(boolean ser) {
  boolean doorstuur = false;
  volatile boolean* cL; byte c = partOfCl(serBron[ser], cL); 
  byte ontCl = basprc::cvintcharint(*serBuf[ser]); boolean newMess = false, door2 = [&]{ if (serBron[ser] < STRIP_STRT) return false; else return serDetFunc(ontCl); }();
  boolean clAr[STRIP_CNT]; for (byte tTt = 0; tTt < STRIP_CNT; tTt++) clAr[tTt] = cL[tTt];
  if (!c) doorstuur = true;
  for (byte t, tt = t = 0; tt < c; t++)
    if (!clAr[t] || (t < STRIP_STRT && ++tt) || !++tt) continue;
    else if (t == serBron[ser] && t >= STRIP_STRT && t <= STRIP_END) {
      obj[t - STRIP_STRT].changeParChld(true, ontCl);
      if (ontCl >= STRIP_STRT && ontCl <= STRIP_END) {
        (obj[ontCl - STRIP_STRT] = objS(obj[t - STRIP_STRT], obj[ontCl - STRIP_STRT])).settedColor = false, obj[t - STRIP_STRT].offSUp = true, obj[t - STRIP_STRT].clrSUp = 1;
        if (ontCl < t) obj[t - STRIP_STRT].clrUp = 0;
      }
      else newMess = true;
    }
    else if (t >= STRIP_STRT && t <= STRIP_END) {
      obj[t - STRIP_STRT].changeParChld(true, ontCl);
    }
    else {
      doorstuur = true; break;
    }
  
  if (newMess) serSendAtMess(ontCl, &obj[serBron[ser] - STRIP_STRT], [&] { if (ontCl < STRIP_STRT) return false; return true; }());
  if (!doorstuur && door2 && (!newMess || ontCl < STRIP_STRT)) serVoeg(basprc::cvintcharchar(ontCl), !ser), serVoeg('D', !ser), serVoeg('\n', !ser);
  else if (doorstuur) serVoeg(basprc::cvintcharchar(serBron[ser]), !ser), serVoeg('A', !ser), serVoeg(basprc::cvintcharchar(ontCl), !ser), serVoeg('\n', !ser);
  objS& pP = obj[serBron[ser] - STRIP_STRT];
  if (pP.randVar) pP.clrSUp = 2; else pP.clrSUp = 1; pP.offSUp = true; if (ontCl < pP.no) pP.clrUp = 0;
}

void ledPlaf::serSendAtMess(byte ontCl, void* varObj, boolean ser) {
  boolean eerst = true; auto entF = [&] { if (eerst) eerst = false; else serVoeg('_', ser); };
	{
		serVoeg(basprc::cvintcharchar(ontCl), ser), serVoeg('a', ser);
		volatile boolean* p = ((objS*)(varObj))->parChldAr;
		for (byte t, tt = t = 0, m = ((objS*)(varObj))->parChld; tt < m; t++)
			if (!p[t] || (t == ontCl && ++tt) || !++tt) continue;
			else entF(), serVoeg(basprc::cvintcharchar(t), ser);
	}
	unsigned long w;
	for (byte t = 0, m = 40 + LEDPL_VARAANT2; (t == LEDPL_VARAANT && (t = 20)) || (t == LEDPL_VARAANT1 + 20 && (t = 40)) || t < m; t++)
		if (!(w = ((objS*)(varObj))->getVarF(t))) continue;
		else entF(), serVoeg(basprc::cvintcharchar(t), ser), serVoeg2(serBuf[!ser], basprc::cvCharLngth(60, w, serBuf[!ser]), ser);
		serVoeg('\n', ser);
}

void ledPlaf::serReadAtMess(boolean ser) {
	boolean doorstuur;
	if (!ser && serVal[ser] == 59) serVal[ser] = 58, doorstuur = serDetFunc(serBron[ser]);
 	else doorstuur = false;
	if (serBron[ser] < STRIP_STRT || serBron[ser] > STRIP_END) {
		serVoeg3(ser);
		return;
	}
   else {
    if (serVal[ser] == 58 || serVal[ser] == 59) serVal[ser] = 57, obj[serBron[ser] - STRIP_STRT] = objS(serBron[ser], obj[serBron[ser] - STRIP_STRT].ledNu);
    if (serTel[ser] == 1) obj[serBron[ser] - STRIP_STRT].changeParChld(true, basprc::cvintcharint(*serBuf[ser]));
    else obj[serBron[ser] - STRIP_STRT].serVarSetOb(basprc::cvintcharint(*serBuf[ser]), basprc::cvintcharStr(serBuf[ser] + 1));
  }
	if (doorstuur) serVoeg(basprc::cvintcharchar(serBron[ser]), !ser), serVoeg('D', !ser), serVoeg('\n', !ser);
}
