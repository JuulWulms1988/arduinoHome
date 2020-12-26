#include "LedPlafSer.h"
#include "piScreen.h"
#include "UdpBroadcast.hpp"
#include <SPI.h>
#include <IRremote.h>

#include <TM1637Display.h>
#define PIN_TINTERRUPT A5 // verander

EthernetUDP Udp;
char udpLees[11];
int udpLeesL = 0;

volatile boolean plafLoopt = false; boolean plafNuPinW = false;
unsigned int plafLooptTel; boolean plafDodBl = false;

const int hetadres = 4; // zoek naar verander
byte mac[] = {
  0xDE, 0xBD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 179, 206);

const int CLK = 7; const int DIO = A0;
TM1637Display display(CLK, DIO);
ledPlafSerCls ldPlSr;
int dodTelMes = 0;
volatile boolean serBeginBool0; volatile boolean serBeginBool1 = serBeginBool0 = false;
boolean thisMes; boolean thisMes1; boolean thisMes0 = thisMes = thisMes1 = false; byte thisMesB; byte thisMesBl = thisMesB = 0; char thisMesBuf[6];
boolean dispnu = 0;
boolean speakerstand[5] = {0, 0, 0, 0, 0};
byte converter[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; byte converterpnt[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
byte ouduur = 88;
byte oudminuut = 88;
boolean tijdloopbool = 0;
byte pteloud = 88;
byte leesaanv;
byte dag;
byte maand;
byte digitdat[21] = {0x00, 0x00, 0x00, 0x00, 0x5E, 0x77, 0x6F, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte timerup[2];
byte kwartup;
unsigned int jaar;
byte weekdag;
unsigned int distel = 0;
byte seconde;
byte minuut;
const int pinlamp[1] {5};
byte uur;
byte tijdtel;
byte digit[4] = {0x00, 0x00, 0x00, 0x00};
byte secondeup;
byte uurup;
byte temp = 40;
byte oudtemp = 0;
byte pauzetel = 0;
boolean datwacht[2];
byte tijdtemp = 0;
struct strDelSw {
  byte pin;
  boolean wacht = false;
  boolean stand = false;
  boolean stand2 = false;
  boolean destand = false;
  int tel = 0;
  volatile boolean tels = false;
  volatile boolean telt = false;
  void telF(int tDiff);
  void func();
} appSw[2];

boolean tv2Var = false;
boolean tv3Var = false;
boolean versterkerstand = 0;
const int pinversterker[2] = {A1, 9};
const int pinspeaker = 8;
const int piezopin = 6;
boolean versterkerdestand = 0;
volatile boolean versterkertels = 0;
boolean versterkerwacht = 0;
byte versterkerwachtst = 0;
signed long versterkertel = 0;
boolean versterkerhoofdbool = 0;
volatile boolean bel [2] = {0, 0};
char lees[31];
int leestel;
int leesGetelt;
byte lezen = 0;
volatile boolean plafBool;

volatile boolean delayWacht = false, delayReady = false;
int delayTel = 0;
boolean dim = false, dim2 = false;
int dimtel = 0;
byte dimstand[1] = {0};
volatile boolean dimCheck = false;
boolean belStStr;

byte dimNu = 0;
volatile boolean dodBool, dodBool3, dodBool2, dodBool1 = dodBool = dodBool3 = !(dodBool2 = true);
volatile unsigned int dodTel = 0, dodTel2, dodTel3 = 0;


int beltel = 0;
// Enter the IP address of the server you're connecting to:
IPAddress server(192, 168, 179, 19);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 23):
EthernetClient client;
piScreenCls piScreenC(A2, 3);

void setup() {
  for (byte t = 0, p[2] {4, A4}; t < 2; t++) pinMode (appSw[t].pin = p[t], OUTPUT), digitalWrite(p[t], LOW);
  //  pinMode (piezopin, OUTPUT);
  pinMode(A3, OUTPUT); //2e Timer LedPlf
  pinMode (pinlamp[0], OUTPUT);
  pinMode (2, INPUT_PULLUP);
  pinMode (pinspeaker, OUTPUT), pinMode(PIN_TINTERRUPT, OUTPUT), digitalWrite(PIN_TINTERRUPT, LOW); pinMode(PIN_PLAFRELAY, OUTPUT), digitalWrite(PIN_PLAFRELAY, LOW);

  for (byte tel = 0; tel < 2; tel++) {
    pinMode (pinversterker[tel], OUTPUT);
  } display.setBrightness(0x0d);
  //noTone(piezopin);


  Ethernet.begin(mac, ip);
  Udp.begin(1235);



  // Open serial communications and wait for port to open:
  // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  // }
  //  Serial.print("kaas");

  // give the Ethernet shield a second to initialize:
  delay(5000);

  attachInterrupt(digitalPinToInterrupt(2), [] {
    if (plafBool) {
      digitalWrite(A3, plafDodBl = !plafDodBl);
      if (!plafLoopt) plafLooptTel = 0, digitalWrite(PIN_TINTERRUPT, (plafLoopt = plafNuPinW = true));
      else if ((plafLooptTel = (plafLooptTel + 1) % 4096) >= 1024 && plafLooptTel < 4086 && plafNuPinW) digitalWrite(PIN_TINTERRUPT, plafNuPinW = false);
      else if (plafLooptTel >= 4086 && !plafNuPinW) digitalWrite(PIN_TINTERRUPT, plafNuPinW = true);
    }
    else if (((plafDodBl && ([&] { digitalWrite(A3, plafDodBl = false); return true; })()) || !plafDodBl) && (plafLoopt && (plafLoopt = false)) || plafNuPinW) digitalWrite(PIN_TINTERRUPT, plafNuPinW = false);
    if (!dodBool2 && (dodBool3 = true)) return;
    if (dodBool1 && (dodBool1 = false))
      if (!dodBool) dodBool = true;
      else if (dodTel > 2000) dodTel = 0;
      else dodTel++;
    if (dodBool)
      if (dodTel > 2000) dodTel = 0;
      else dodTel++;
    dodBool = true;
  }, CHANGE);
  if (client.connect(server, 10002)) {

    char snd[] {'|', '-', hetadres / 10 + '0', hetadres % 10 + '0', '+', '@', 'V', 'R', 'U', 'T', '+', '#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', 'R', '+', '\0'};
    client.print(snd);
    dodTelMes = 24000;
    //client.print("|-"); client.write(hetadres / 10 + '0'); client.write(hetadres % 10 + '0'); client.print("+|");
    //client.print ("#SV"); client.write(hetadres / 10 + '0'); client.write(hetadres % 10 + '0'); client.write('R'); client.write('+');
  }
}


void netwerksetup () {
  dodTel2 = 0;
  dim = false, dim2 = false;
  dimtel = 0;
  dimCheck = false;
  bel[0] = 0;// noTone (piezopin);
  beltel = 0, tijdloopbool = 0,

  dispnu = distel = 0,
  ouduur = oudminuut = 88, leestel = lezen = 0;
  // Timer1.start(); Timer1.attachInterrupt(dodeman);


  if (client.connect(server, 10002)) {
    ldPlSr.bufTel = 5;
    {
      char snd[] {'|', '-', hetadres / 10 + '0', hetadres % 10 + '0', '+', '|', '\0'};

      client.print(snd); dodTelMes = 24000;
    }
  }

  //client.print("|-"); client.write(hetadres / 10 + '0'); client.write(hetadres % 10 + '0'); client.print("+|");
  statofadresdoorgeefproc();

  client.print([] { if (versterkerdestand) return "@VRAN+"; return "@VRUT+"; }());
  char snd[] {'#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', 'R', '+', '\0'};

  client.print(snd); dodTelMes = 24000;

  //client.print ("#SV"); client.write(hetadres / 10 + '0'); client.write(hetadres % 10 + '0'); client.write('R'); client.write('+');

}
void sluisTv (boolean IO) {
  appSw->stand = IO;
}

boolean leesstandaardproc(char thisChar) {
  boolean uitkomst = 0; if (!lezen) {
    if (thisChar >= '@' and thisChar <= '@' + 2 and !lezen) leesaanv = thisChar - '@', lezen = 1, leestel = 0, lees[0] = '\0';
    else if (thisChar == '&' || (thisChar == '$' && (thisMes0 = true))) {
      if (ldPlSr.strtW == 4) thisMes = true; else if (thisChar == '$') thisMes0 = false, ldPlSr.serLs('$');
      lezen = 2, leestel = 0;
    }
    else if (thisChar == '#' && (lezen = 2) && !(leestel = 0) && (thisMes0 = true))
      switch (ldPlSr.strtW) {
        case 4: thisMes = true; break;
        case 3: ldPlSr.strtW = 4; thisMes = true; break;
        default: thisMes0 = false, ldPlSr.serLs('$'); break;
      }
  }
  else if (thisChar == '+') {
    if (lezen == 1) uitkomst = 1;
    else {
      if (leestel <= 4) thisMes0 = false;
      thisMes = false, thisMesB = thisMesBl = 0;
    }
    leesGetelt = leestel, leestel = lezen = 0;
  } else if (lezen == 2) {
    if (thisMes) {
      if (thisMesB == 6) {
        leestel++;
        Serial.write(thisChar);
        return false;
      }
      if (thisChar != '\\') {
        thisMesBuf[thisMesBl++] = thisChar;
        return false;
      }
      thisMesBuf[thisMesBl] = '\0', thisMesBl = 0; unsigned long uit = basprc::cvintcharStr(thisMesBuf);
      switch (thisMesB++) {
        case 0: ldPlSr.upM = uit; break;
        case 1: ldPlSr.upD = uit; break;
        case 2: ldPlSr.upU = uit; break;
        case 3: ldPlSr.upMi = uit; break;
        case 4: ldPlSr.upS = uit; break;
        case 5: ldPlSr.upN = uit; break;
      }
    }
  }
  else lees[leestel] = thisChar, lees[++leestel] = '\0'; return uitkomst;
}

boolean PSgelezenproc() {
  if (lees[0] == 'P' && lees[1] == 'S') return true; return false;
}

boolean regulieruitvoergelezen () {
  if ((lees[0] == 'U' and (lees[1] == 'V' or lees[1] == 'U' or lees[1] == 'D')) or (lees[0] == 'V' and lees[1] == 'U')) return true; return false;
}

boolean tijdgelezenproc(char* lees, int leesGetelt) {
  boolean uitkomst = 0; if (leesGetelt > 3 && (lees[0] == 'T' || lees[0] == 't') and (lees[1] == 'T' or lees[1] == 'U' or lees[1] == 't' || lees[1] == 'u')) uitkomst = 1; return uitkomst;
}

boolean tempgelezenproc() {
  if (lees[0] == 'T' and lees[1] == 'E') return 1; return 0;
}

boolean SWgelezenproc() {
  boolean uitkomst = 0; if (lees[0] == 'S' and lees[1] == 'W') uitkomst = 1; return uitkomst;
}

int leesnaaractie(char thisChar) {
  int uitkomst = 0;
  if (leesstandaardproc(thisChar))
    if (regulieruitvoergelezen ()) uitkomst = 1;
    else if (SWgelezenproc()) uitkomst = 3;
    else if (tijdgelezenproc(lees, leesGetelt)) uitkomst = 4;
    else if (tempgelezenproc()) uitkomst = 5;
    else if (PSgelezenproc()) uitkomst = 6;
  return uitkomst;
}

int leesnaaractieUdp() {
  if (tijdgelezenproc(udpLees + 1, udpLeesL)) return 4;
  return 0;
}

boolean aanuitstuitv() {
  boolean uitkomst = 0;
  if (lees[4] == 'A' and lees[5] == 'N') {
    uitkomst = 1;
  }
  return uitkomst;
}

boolean pinAppSw(byte depin) {
  strDelSw* p; for (byte t = 0, w; t < 2 && (p = appSw + t); t++)
    if (depin == p->pin) {
      w = aanuitstuitv(); if (t == 1 && !w && (p->stand2 || p->wacht)) ldPlSr = ledPlafSerCls(), Serial.write('|'), ldPlSr.strtW = 0, thisMes1 = thisMes0 = false;
      p->stand = w; return true;
    } return false;
}

void verwerkcode(int switchvar, char* lees, byte leesaanv) {
  unsigned int reken; byte depin, jaarlen; switch (switchvar) {
    case 1: depin = uitvoerpin();
      if (depin == 4) {
        if (((tv2Var = aanuitstuitv()) || !tv2Var) && !tv3Var) sluisTv(tv2Var);
        break;
      }
      else if (pinAppSw(depin)) break;
      if (depin == A2) piScreenC.set = aanuitstuitv();
      else if (depin == piezopin) {
        bel[0] = aanuitstuitv(); if (bel[0] and !bel[1]) {
          bel[1] = 1, dod2F([](void* X, int y) {
            beltel = 0 - ((int)X + y);
          }, NULL), zetbel(belStStr = false);
        }
      }
      else if (depin == pinlamp[0])
        if ((lees[4] == 'u' && lees[5] == 't' && !(dim2 = false)) || (lees[4] == 'a' && lees[5] == 'n' && (dim2 = true))) {
          dim = true, dod2F([](void* X, int y) {
            *((int*)X) = 0 - y;
          }, &dimtel); if (dimstand[0] != dim2 * 255) analogWrite(depin, dimNu = dimstand[0] + dim2 * 1 - !dim2 * 1);
        }
        else dim = false, analogWrite(depin, dimNu = dimstand[0] = cv16waarde(lees[4], lees[5])), dimtel = 0;
      else {
        digitalWrite(depin, aanuitstuitv()); if (depin == pinspeaker) speakerstand[0] = aanuitstuitv();
      } break;
    case 3: depin = uitvoerpin();
      if ((lees[4] == '0' and lees[5] == '0') or (lees[4] == 'U' and lees[5] == 'T'))
        if (depin == 200) reken = ((lees[8] - '0') * 10 + (lees[9] - '0')) + 1, speakerstand[reken] = 0;
        else {
          //  knipper[0] = 0; digitalWrite(depin, HIGH);
        }
      else {
        if (depin == 200)
          reken = ((lees[8] - '0') * 10 + (lees[9] - '0')) + 1, speakerstand[reken] = 1;
        else {
          //  digitalWrite(depin, knipperstand); knipper[0] = 1;
        }
      }
      break;
    case 4:
      kwartup = 0;
      if (lees[0] == 'T') kwartup += 50;
      if (lees[1] == 'U' || lees[1] == 'T') kwartup += 25;
      if (tijdloopbool) dod2F([](void* X, int y) {
        dodTel2 = y + 1;
      }, NULL), uurup = cvintcharint(lees[2]), secondeup = cvintcharint(lees[3]), timerup[0] = leesaanv, timerup[1] = cvintcharint(lees[4]);
      else if (lees[1] == 'T' || lees[1] == 't') {
        uur = cvintcharint(lees[3]) % 24, minuut = cvintcharint(lees[4]), seconde = cvintcharint(lees[5]), weekdag = (cvintcharint(lees[3]) / 24) * 6 + (cvintcharint(lees[6]) / 31) * 3 + (cvintcharint(lees[7]) / 12) + 1, dag = cvintcharint(lees[6]) % 31 + 1; maand = cvintcharint(lees[7]) % 12 + 1;
        jaar = 2000, jaarlen = 0; for (unsigned int tel = 8; lees[tel] != '\0'; tel++) jaarlen++; for (byte tel = 0; tel < jaarlen; tel++) jaar += cvintcharint(lees[tel + 8]) * (macht(60, jaarlen - 1 - tel));
        tijdtel = 0, tijdtemp = leesaanv, pauzetel = cvintcharint(lees[2]), distel = kwartup, zetdatdigit(), tijdloopbool = 1;
      } break;
    case 5:
      temp = cvintcharint(lees[3]) - 20;
      break;
    case 6:
      if (lees[4] == 'C' && lees[5] == 'U') piScreenC.confirm(false);
      break;
  }
}
byte uitvoerpin () {
  int uitkomst = 0;
  if (lees[6] == 'B' and lees[7] == 'B' and lees[8] == '0' and lees[9] == '0') uitkomst = piezopin;
  else if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinlamp[0];
  }
  else if (lees[6] == 'S' and lees[7] == 'P' and lees[8] == '0' and lees[9] == '0') uitkomst = pinspeaker;
  else if (lees[6] == 'D' and lees[7] == 'P' and lees[8] == '0' and lees[9] - '0' < 4) uitkomst = 200;
  else if (lees[6] == 'T' and lees[7] == 'V' and lees[8] == '0' and lees[9] - '0' < 4) uitkomst = appSw[0].pin;
  else if (lees[6] == 'P' and lees[7] == 'I' and lees[8] == '0' and lees[9] - '0' < 4) uitkomst = A2;
  else if (lees[6] == 'P' and lees[7] == 'L' and lees[8] == '0' and lees[9] - '0' < 4) uitkomst = appSw[1].pin;
  return uitkomst;
}

void dimproc(int tDiff) {
  if (!dim) return;
  int dimT = dimtel + tDiff;
  dod2F([](void* X, int y) {
    *((int*)X) += y;
  }, &dimT);
  if (dimT < 2) return;
  else if (dim2) {
    if (dimNu < 255 && [&] { if (dimT >= 255 - dimstand[0]) return !(dimNu == 255 || !(dimNu = 255)); byte DD = dimstand[0] + dimT; return dimNu != DD && ((dimNu = DD) || true); }()) analogWrite(pinlamp[0], dimNu);
  }
  else if (dimNu > 0 && [&] { if (dimT >= dimstand[0]) return !(!dimNu || (dimNu = 0)); byte DD = dimstand[0] - dimT; return dimNu != DD && ((dimNu = DD) || true); }()) analogWrite(pinlamp[0], [&] { if (dimT >= dimstand[0]) return 0; return dimstand[0] - dimT; }());
}


void loop() {
  telnetloop(), versterkerloop();
  if (!delayWacht && dimCheck) {
    dimCheck = false;
    char snd[] { '#', 'S', 'V', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'R', '+', '\0'};
    client.print(snd); dodTelMes = 24000;
  }
  for (byte t = 0; t < 2; t++) appSw[t].func();
  if (dodBool) dodF();
  else ldPlSr.func();
}

void dodF() {
  auto x = [] {
    if (dodBool3) dodBool3 = false, dodBool1 = true;
  };
  dodBool = dodBool2 = false;
  if (dodBool1) dodBool1 = false, dodTel++;
  bool routtBB = true;
  unsigned int tel; if (dodTel > 10) {
    tel = 10, dodTel -= 11;
    dodBool2 = dodBool = true;
    x();
  } else tel = dodTel, dodTel = 0, dodBool2 = true, x();
  {
    for (unsigned int dTel = 0; dTel <= tel; dTel++) {
      dodeman(tel - dTel, [&] { if (dodTel2 && dodTel2 - 1 == dTel) return true; return false; }());
      if (routtBB && dTel == dodTel3) routtBB = false, dod2F([](void* X, int y) {
        if (y) dodTel3 = y - 1;
        else dodTel3 = 0;
      }, NULL), dimproc(tel - dTel);
      ldPlSr.func();
    }
  }
  if (dodTel2 > 10) dodTel2 -= 11;
  else if (dodTel2) dodTel2 = 0;
  if (routtBB && dodTel3 > 10) dodTel3 -= 11;
}


boolean extraAp = false;
boolean extraAp2 = false;

IRsend IRSend;

void strDelSw::func() {
  if (!stand2)
    if (!wacht) {
      if (stand) {
        digitalWrite(pin, HIGH);
        if (pin == A4) ldPlSr.strtW = 1, thisMes1 = thisMes0 = false, serBeginBool0 = true;
        dod2F([](void* X, int y) {
          *((int*)X) = 0 - y;
        }, &tel);
        if (pin == 4) extraAp = true, tel -= 500;
        tels = wacht = true;
      }
    } else {
      if (pin == 4 && extraAp) {
        if (!telt) return;
        dod2F([](void* X, int y) {
          *((int*)X) = 0 - (y + 2000);
        }, &tel);
        telt = extraAp = false, IRSend.sendRC5(12, 12), tels = true;
      }
      else {
        if (pin == A4 && !ldPlSr.strtW && stand) ldPlSr.strtW = 1;
        if (telt) telt = wacht = false, stand2 = true;
      }
    }
  else if (!wacht) {
    if (pin == 4) {
      if (!extraAp) {
        if (!stand) {
          dod2F([](void* X, int y) {
            *((int*)X) = 0 - (2000 + y);
          }, &tel), extraAp = true, IRSend.sendRC5(12, 12), tels = true;
        }
      } else { //wel extraAp
        if (telt)
          if (stand) wacht = !(stand2 = false), func();
          else extraAp = telt = false, digitalWrite(4, LOW), dod2F([](void* X, int y) {
            *((int*)X) = 0 - y;
          }, &tel), tels = wacht = true;
      }
    }
    else {
      if (!stand) {
        dod2F([](void* X, int y) {
          *((int*)X) = 0 - y;
        }, &tel);
        if (pin == A4) extraAp2 = true, tel += 500, ldPlSr.strtW = 0, thisMes1 = thisMes0 = serBeginBool1 = false, Serial.end();
        else digitalWrite(pin, LOW); tels = wacht = true;
      }
      else if (pin == A4 && !ldPlSr.strtW) ldPlSr.strtW = 1;
    }
  }
  else if (pin == A4 && extraAp2) {
    if (telt) digitalWrite(A4, telt = extraAp2 = false), dod2F([](void* X, int y) {
      *((int*)X) = 0 - y;
    }, &tel), tels = true;
  }
  else if (telt) telt = wacht = stand2 = false;
}

void versterkerloop() {
  int tDiff;
  dod2F([](void* X, int y) {
    *((int*)X) = y;
  }, &tDiff); signed long vT = versterkertel + tDiff;
  if (!versterkerstand) {
    if (!versterkerwacht) {
      if (speakerstand[0] or speakerstand[1] or speakerstand[2] or speakerstand[3] or speakerstand[4]) {
        digitalWrite(pinversterker[0], HIGH); versterkerdestand = 1;
        client.print("@VRAN+"); dodTelMes = 24000;
        versterkerwacht = 1; versterkerwachtst = 0; versterkertel = 0 - tDiff; versterkertels = 1;
      }
    }
    else {
      switch (versterkerwachtst) {
        case 0:
          if (vT >= 300) {
            digitalWrite(pinversterker[1], HIGH); versterkertel = 0 - tDiff; versterkerwachtst = 1;
          }
          break;
        case 1:
          if (vT >= 1000) {
            versterkerwacht = 0; versterkertels = 0; versterkerstand = 1;
          }
          break;
      }
    }
  }
  else {
    if (!versterkerwacht) {
      if (!speakerstand[0] and !speakerstand[1] and !speakerstand[2] and !speakerstand[3] and !speakerstand[4]) {
        versterkerwacht = 1; versterkerwachtst = 0; versterkertel = 0 - tDiff; versterkertels = 1;
      }
    }
    else {
      switch (versterkerwachtst) {
        case 0:
          if ((speakerstand[0] or speakerstand[1] or speakerstand[2] or speakerstand[3] or speakerstand[4]) and !versterkerhoofdbool) {
            versterkertels = 0; versterkerwacht = 0;
          }
          else {
            if (vT >= 36000) {
              if (!versterkerhoofdbool) {
                digitalWrite (pinversterker[1], LOW); versterkerdestand = 0;

                client.print("@VRUT+"); dodTelMes = 24000;

                versterkerhoofdbool = 1;
              }
              else {
                if (vT >= 37000) {
                  digitalWrite (pinversterker[0], LOW); versterkertel = 0 - tDiff; versterkerhoofdbool = 0; versterkerwachtst = 1;
                }
              }
            }
          }
          break;
        case 1:
          if (vT >= 1000) {
            versterkerwacht = 0; versterkertels = 0; versterkerstand = 0;
          }
          break;
      }
    }
  }
}

/*
  void versterkerloop() {
  if (!versterkerstand) {
    if (!versterkerwacht)
      if (speakerstand[0] or speakerstand[1] or speakerstand[2] or speakerstand[3] or speakerstand[4]) {
        digitalWrite(pinversterker[0], HIGH); versterkerdestand = 1;
        if (slam) client.print("@VR01+"); versterkerwacht = versterkertels = 1, versterkertel = (versterkerwachtst = 0);
      } else
        switch (versterkerwachtst) {
          case 0:
            if (versterkertel >= 300) {
              digitalWrite(pinversterker[1], HIGH); versterkertel = 0; versterkerwachtst = 1;
            }
            break;
          case 1:
            if (versterkertel >= 1000) {
              versterkerwacht = 0; versterkertels = 0; versterkerstand = 1;
            } break;
        }
  }
  else if (!versterkerwacht) {
    if (!speakerstand[0] and !speakerstand[1] and !speakerstand[2] and !speakerstand[3] and !speakerstand[4]) versterkerwacht = versterkertels = 1, versterkertel = (versterkerwachtst = 0);
  } else
    switch (versterkerwachtst) {
      case 0:
        if ((speakerstand[0] or speakerstand[1] or speakerstand[2] or speakerstand[3] or speakerstand[4]) and !versterkerhoofdbool) versterkertels = versterkerwacht = 0;
        else if (versterkertel >= 36000)
          if (!versterkerhoofdbool) {
            digitalWrite (pinversterker[1], LOW); versterkerdestand = 0;
            if (slam) client.print("@VR00+"); versterkerhoofdbool = 1;
          } else if (versterkertel >= 37000) digitalWrite (pinversterker[0], LOW), versterkertel = versterkerhoofdbool = 0, versterkerwachtst = 1;
        break;
      case 1: if (versterkertel >= 1000) versterkerwacht = versterkertels = versterkerstand = 0; break;
    }
  }

*/

boolean speakervoorwaarde (byte no) {
  boolean uitkomst = 0; switch (no) {
    case 1: if (speakerstand[0]) uitkomst = 1; break;
    case 2: if (speakerstand[1]) uitkomst = 1; break;
    case 3: if (speakerstand[2] or speakerstand[3] or speakerstand[4]) uitkomst = 1; break;
    case 4: break;
  } return uitkomst;
}

void tijdproc () {
  if (!tijdloopbool) return;
  if ((distel >= 100 and tijdtemp < 2) or ((distel != 0 and distel % 25 == 0) && tijdtemp == 2))
    if (pauzetel == 4 and tijdtemp < 2) tijdtemp++, pauzetel = 0, pteloud = 88;
    else if (pauzetel >= 19 + datwacht[0] + datwacht[1] && tijdtemp >= 2) tijdtemp = 0, pauzetel = 0, oudminuut = 88, ouduur = 88, distel = 0;
    else pauzetel++;
  if (tijdtel >= 100) tijdtel = 0, secondetel();
  if (distel >= 100) distel = 0;
  if (!tijdtemp)
    if (minuut != oudminuut or uur != ouduur || dispnu) {
      if (dispnu) dispnu = 0; if (distel < 50) tijddigit(1), zettijd(), ouduur = uur, oudminuut = minuut; else tijddigit(0), zettijd(), ouduur = uur, oudminuut = minuut;
    } else {
      if (distel == 0) tijddigit(1), zettijd(); else if (distel == 50) tijddigit(0), zettijd();
    } else if (tijdtemp == 1) if (temp != oudtemp || dispnu) {
      if (dispnu) dispnu = 0; if (distel < 50) tempdigit(1), zettijd(); else tempdigit(0), zettijd(); oudtemp = temp;
    } else {
      if (distel == 0) tempdigit(1), zettijd(); else if (distel == 50) tempdigit(0), zettijd();
    } else if (tijdtemp == 2 && (pteloud != pauzetel || dispnu)) {
    if (dispnu) dispnu = 0; pteloud = pauzetel, datproc();
  } tijdtel++; distel++;
}

void datproc() {
  if (pauzetel == 0) zetdatdigit(); byte wachter = datwacht[0] + datwacht[1]; if (dispniet()) return; for (byte tel = 0; tel < 4; tel++) digit[tel] = digitdat[(tel + pauzetel) % (19 + wachter)]; zettijd();
}

void zetdatdigit() {
  digitdat[8] = converter[weekdag]; byte tel = 11; datwacht[0] = wachtdat(dag, tel), datwacht[1] = wachtdat(maand, tel), digitdat[tel++] = converter[jaar / 1000], digitdat[tel++] = converter[(jaar / 100) % 10], digitdat[tel++] = converter[(jaar / 10) % 10], digitdat[tel++] = converter[jaar % 10];
}

boolean wachtdat(byte x, byte& tel) {
  boolean uitkomst = 0; if (x / 10) uitkomst = 1, digitdat[tel++] = converter[x / 10]; digitdat[tel++] = converter[x % 10], digitdat[tel++] = 0x40; return uitkomst;
}

void secondetel() {
  if (!minuuttel()) seconde++; else seconde = 0;
}

boolean minuuttel() {
  boolean uitkomst = false; if (seconde == 59 && (uitkomst = true))
    if (!uurtel()) minuut++;
    else minuut = 0; return uitkomst;
}

boolean uurtel() {
  boolean uitkomst = false; if (minuut == 59 && (uitkomst = true))
    if (uur == 23) uur = 0, dagtel();
    else uur++; return uitkomst;
}

void dagtel() {
  weekdag = weekdag % 7 + 1; if (!maandtel()) dag++; else dag = 1;
}

boolean maandtel() {
  if (!((maand < 8 && ((maand % 2 && dag == 31) || (maand != 2 && !(maand % 2) && dag == 30) || (maand == 2 && ((!jaar % 4 && dag == 29) || (jaar % 4 && dag == 28))))) || (maand > 7 && ((maand % 2 && dag == 30) || (!(maand % 2) && dag == 31))))) return false;
  if (maand < 12) maand++; else maand = 1; return true;
}

void telnetloop () {
  if (delayWacht) {
    if (delayReady) delayWacht = false, delayReady = false, netwerksetup();
    return;
  }
  piScreenC.func();
  if (udpLeesL = udpBroadcast::readPack(udpLees, Udp)) {
    int x = leesnaaractieUdp();
    verwerkcode(x, x == 4 ? udpLees + 1 : udpLees, *udpLees - '@');
  }
  if (client.available()) dodTelMes = 24000, verwerkcode(leesnaaractie(client.read()), lees, leesaanv);
  boolean uit = false;
  for (byte t = 0; !client.connected() && (t < 20 || !(uit = true)); t++);
  if (uit)
    dodTelMes = 0, client.stop(), dod2F([](void* X, int y) {
    delayTel = 0 - y;
  }, NULL), delayWacht = true;
}

void statofadresdoorgeefproc () {
  if (thisMes) Serial.write('|'), ldPlSr.strtW = 1, thisMes = thisMes0 = false, thisMesB = thisMesBl = 0;
  switch (ldPlSr.strtW) {
    case 3: ldPlSr.openF(); break;
    case 4: if (thisMes1) break; else if (thisMes0) thisMes1 = true; else ldPlSr.openF(); break;
  }
}


void ANUTproc (int lees) {
  if (digitalRead(lees)) {
    client.print("AN");
  }
  else {
    client.print("UT");
  }
}

void ANUTproc2 (boolean debool) {
  if (debool) {
    client.print("AN");
  }
  else {
    client.print("UT");
  }
}

void strDelSw::telF(int tDiff) {
  if (!tels) return;
  dod2F([](void* X, int y) {
    *((int*)X) += y;
  }, &(tDiff += tel));
  if (tDiff < 999) {
    tel++; if (serBeginBool0 && tDiff > 50 && pin == A4) serBeginBool0 = false, Serial.begin(74880), serBeginBool1 = true;
  }
  else tel = 0, tels = false, telt = true;
}

void dodBelF(int tDiff) {
  if (!bel[1]) return;
  int belTT;
  dod2F([](void* X, int y) {
    *((int*)X) = y;
  }, &belTT), belTT += tDiff + beltel++;
  if (belTT >= 280)
    if (!bel[0]) {
      bel[1] = 0, dispnu = 1, /*belTone = false, noTone(piezopin), */beltel = 0; return;
    }
    else beltel = 1 - (((belTT - tDiff) - (beltel - 1)) + tDiff), belTT = 0;
  byte AA; int UUU = belTT;
  if (((((((belTT < 35) || (belTT >= 70 && belTT < 105) || (belTT >= 140 && belTT < 175)) && (AA = 2)) || (belTT >= 210 && belTT < 245 && !(AA = 0))) && (belTT = 1)) || ((belTT < 210 && (AA = 1) && !(belTT = 0)) || !(AA = belTT = 0))) && belStStr == (boolean)(belTT))
    [&] {
    if (AA);// belTone = true, tone(piezopin, AA * 600);
    //else if (belTone) belTone = false, noTone (piezopin);
  }(), zetbel(belStStr = !belStStr);
}


void dod2F (void (*x)(void*, int), void* y) {
  dodBool2 = false;
  if (dodBool1 && !(dodBool1 = false))
    if (!dodBool) dodBool = true;
    else if (dodTel > 2000) dodTel = 0;
    else dodTel++;
  x(y, dodBool * 1 + dodTel), dodBool2 = true;
  if (dodBool3) dodBool3 = false, dodBool1 = true;
}


void dodeman (int tDiff, boolean tijdup) {
  if (dodTelMes && !--dodTelMes) dodTelMes = 24000, client.print("@DM+");
  piScreenC.cntF(tDiff);
  if (tijdup) {
    uur = uurup; seconde = secondeup; tijdtemp = timerup[0];
    pauzetel = timerup[1];
    distel = kwartup;
    ouduur = 88; tijdtel = 0; tijdup = 0;
  }
  tijdproc(); if (dim && dimtel < 512 && [](int tDifff) {
  dod2F([](void* X, int y) {
    *((int*)X) += y;
    }, &tDifff); return tDifff;
  }(++dimtel + tDiff) >= 512) dimCheck = true, dimtel = 512;
  if (versterkertels) {
    versterkertel++;
  }
  for (byte t = 0; t < 2; t++) appSw[t].telF(tDiff);
  if (delayWacht && !delayReady) if (delayTel++ >= 100) delayTel = 0, delayReady = true;
  dodBelF(tDiff);
}

byte cv16waarde(char char0, char char1) {
  byte uitkomst;
  if (char1 >= 'a' && char1 <= 'f') {
    uitkomst = 10 + (char1 - 'a');
  }
  else {
    uitkomst = char1 - '0';
  }
  if (char0 >= 'a' && char0 <= 'f') {
    uitkomst = uitkomst + ((10 + (char0 - 'a')) * 16);
  }
  else {
    uitkomst = uitkomst + ((char0 - '0') * 16);
  }
  return uitkomst;
}

char cv16char0(byte waarde) {
  byte reken = waarde / 16;
  if (reken >= 10) {
    return reken - 10 + 'a';
  }
  else {
    return reken + '0';
  }
}

char cv16char1(byte waarde) {
  byte reken = waarde % 16;
  if (reken >= 10) {
    return reken - 10 + 'a';
  }
  else {
    return reken + '0';
  }
}
void zettijd () {
  if (!dispniet()) {
    uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
  }
}

void tijddigit(boolean pnt) {
  if (!dispniet()) {
    if (pnt) {
      if (uur / 10 < 1) {
        digit[0] = 0x80;
      }
      else {
        digit[0] = converterpnt[uur / 10];
      }
      digit[1] = converterpnt[uur % 10]; digit[2] = converterpnt[minuut / 10]; digit[3] = converterpnt[minuut % 10];
    }
    else {
      if (uur / 10 < 1) {
        digit[0] = 0x00;
      }
      else {
        digit[0] = converter[uur / 10];
      }
      digit[1] = converter[uur % 10]; digit[2] = converter[minuut / 10]; digit[3] = converter[minuut % 10];
    }
  }
}

void zettemp () {
  uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
}

void tempdigit (boolean celsio) {
  if (!dispniet()) {
    if (celsio) {
      digit[3] = 0x39; digit[2] = 0x63;
    }
    else {
      digit[3] = 0x00; digit[2] = 0x00;
    }
    // if (temp < 0) {
    //   digit[0] = 0x40; digit[1] = converter[(temp * (-1)) % 10];
    //  }
    //  else {
    if (temp / 10 < 1) {
      digit[0] = 0x00;
    }
    else {
      digit[0] = converter[temp / 10];
    }
    digit[1] = converter[temp % 10];
    //  }
  }
}

byte cvintcharint (char x) {
  if (x <= '9') {
    return x - '0';
  }
  else {
    if (x <= 'Z') {
      return x - 55;
    }
    else {
      return x - 61;
    }
  }
}

char cvintcharchar(byte aant) {
  if (aant < 10) {
    return aant + '0';
  }
  else {
    if (aant < 36) {
      return aant + 55;
    }
    else {
      return aant + 61;
    }
  }
}

unsigned int macht(byte macht, byte x) {
  byte uitkomst = macht;
  if (x == 0) {
    return 1;
  }
  else {
    for (byte tel = 0; tel < x - 1; tel++) {
      uitkomst = uitkomst * macht;
    }
    return uitkomst;
  }
}

boolean dispniet() {
  if (bel[1]) {
    return 1;
  }
  return 0;
}

void zetbel (boolean stand) {
  byte beldat [4] = {0x00, 0x7C, 0x79, 0x38};
  for (byte tel = 0; tel < 4; tel++) {
    digit[tel] = beldat[(tel + !stand) % 4];
  }
  uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
}
