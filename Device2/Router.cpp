#include "Router.h"

void routerCls::func() {
  if (++tel2 < 100) return;
 tel2 = 0;
 if (conBool) {
    if (++tel1 > 9) tel1 = 0, flagBl = true;
    return;
  }
 if ((stand && stand != 18 && (stand < 3 || stand == 19 || !(stand % 2))) && ++tel1 > 9) {
  tel1 = 0;
  flagBl = true;
  return;
 }
  switch (stand) {
    case 0: if (++tel1 > 19) stand = 19, digitalWrite(routPin, LOW), tel1 = 0;
      return;
    case 1: if (tel > 2) stand = 0, digitalWrite(routPin, HIGH); return;
    case 2: if (tel > 5) tel = 0, stand++, digitalWrite(routPin, HIGH); return;
    case 18: if (++tel1 > 19) stand = 2, digitalWrite(routPin, LOW), tel1 = 0;
      return;
    case 19:
      if (tel > 5) tel = 0, stand = 18, digitalWrite(routPin, HIGH);
      return;
  }
  if (stand % 2) {
    if (++tel1 > 19) {
      tel1 = 0, digitalWrite(routPin, LOW);
      if (stand < 17) stand++; else stand--;
    }
    return;
  }
  if (stand < 10) {
    if (tel < 9) return;
  }
  else if (stand < 16) {
    if (tel < 15) return;
  }
  else if (tel < 45) return;
  tel = 0, stand++, digitalWrite(routPin, HIGH);
}

void routerCls::flagF() {
  if (conBool) {
    conBool = false;
   // if (ff.available()) 
   stand = 1, tel = 0;
   // else tel++;
    //ff.stop();
    flagBl = false;
    return;
  }
  if (proc()) conBool = true;
  else tel++; flagBl = false;
}
