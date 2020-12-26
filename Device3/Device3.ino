#define TEMP_AR_SIZE 50

#include <SPI.h>
#include <Ethernet.h>

#include <TM1637Display.h>
#include "serKnop.h"
#define PIN_COS_KN 6
#include "piScreen.h"
#include "UdpBroadcast.hpp"

int dodTelMes = 0;
EthernetUDP Udp;
char udpLees[11];
int udpLeesL = 0;

byte hetadres = 3; // zoek naar verander
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xEE, 0xED
};

IPAddress ip(192, 168, 179, 205);
byte ledknpwm = 255;
volatile boolean dispnu = 0;
const int CLK = 7; const int DIO = 4;
const int pintemp = A5;
TM1637Display display(CLK, DIO);
byte converter[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; byte converterpnt[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
byte ouduur = 88;
byte oudminuut = 88;
boolean tijdloopbool = 0;
byte seconde;
byte minuut;
byte dimNu = 0;

unsigned int datgezet = 9999;
volatile byte uur;
byte pteloud = 88;
byte leesaanv;
byte dag;
byte maand;
byte digitdat[21] = {0x00, 0x00, 0x00, 0x00, 0x5E, 0x77, 0x6F, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte timerup[2];
unsigned int jaar;
byte weekdag;
byte tijdtel;
unsigned int distel;
boolean tijdgeupdated = 0;
byte digit[4] = {0x00, 0x00, 0x00, 0x00};
byte secondeup;
byte uurup;
byte kwartup;
const int pinlamp[2] = {A3, 5};
const int pinspeaker = 8;
const int piezopin = 11;
boolean bel [2] = {0, 0};
boolean opstart = 1;
char lees[31];
int leestel;
int leesGeteld;
boolean lezen = 0;
int beltel = 0;
byte tijdtemp = 0;
int convertemp[42] = {785, 775, 766, 756, 746, 735, 725, 714, 704, 693, 682, 671, 660, 648, 637, 626, 614, 603, 591, 580, 568, 557, 545, 534, 523, 512, 500, 489, 478, 467, 456, 445, 434, 424, 413, 403, 393, 383, 373, 364, 354, 345};
byte temprekentel2 = 0;
int tempreken = 0;
byte tempreken2[TEMP_AR_SIZE];
byte temprekentel = 0;
volatile byte temp = 40;
volatile byte oudtemp = 0;
byte pauzetel = 0;
boolean datwacht[2];
boolean tempzend = 0;
boolean teldodeman = false;
boolean delayWacht = false, delayReady = false;
int delayTel = 0;
boolean dim = false, dim2 = false;
int dimtel = 0;
byte dimstand[1] = {0};
boolean dimCheck = false;
boolean belStStr;


// Enter the IP address of the server you're connecting to:
IPAddress server(192, 168, 179, 19);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 23):
EthernetClient client;

piScreenCls piScreenC(A4, PSNO_0), piScreenBed(A2, PSNO_1);

volatile boolean dodBool, dodBool3, dodBool2, dodBool1 = dodBool = dodBool3 = !(dodBool2 = true);
unsigned int dodTel = 0, dodTel2, dodTel3 = 0;



void setup() {
  pinMode (2, INPUT_PULLUP);
  pinMode (pinlamp[0], OUTPUT);
  pinMode (pinlamp[1], OUTPUT);
  pinMode (pinspeaker, OUTPUT);
  pinMode (pintemp, INPUT);
  pinMode (PIN_COS_KN, OUTPUT);
  display.setBrightness(0x0d);





  serKn.setF();
  Ethernet.begin(mac, ip);
  Udp.begin(1235);
  // Open serial communications and wait for port to open:

  // give the Ethernet shield a second to initialize:
  delay(1000);

  attachInterrupt(digitalPinToInterrupt(2), [] {
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
    char snd[] {'|', '-', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), '+', '|', '#', 'S', 'V', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'R', '+', '\0'};
    client.print(snd);
    dodTelMes = 24000;
  }

  netwerksetup ();

}

void netwerksetup () {
  // give the Ethernet shield a second to initialize:
  if (!opstart) {
    tempzend = 0;
    dodTel2 = 0;
    dim = false, dim2 = false;
    dimtel = 0;
    dimCheck = false;
    teldodeman = false;
    tijdloopbool = 0;
    ouduur = 88;
    oudminuut = 88;
    tijdgeupdated = 0;
    dispnu = 0;
    beltel = 0;
    bel[0] = 0;
    leestel = 0;
    lezen = 0;
    serKn.netwSet();

    if (client.connect(server, 10002)) {

      char snd[] {'|', '-', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), '+', '|', '#', 'S', 'V', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'R', '+', '\0'};
      client.print(snd);
      zendtemp();
    }
  }
  opstart = 0;
}







boolean leesstandaardproc(char thisChar) {
  boolean uitkomst = 0;
  if (thisChar >= '@' and thisChar <= '@' + 2 and !lezen) {
    leesaanv = thisChar - '@';
    lezen = 1; leestel = 0; lees[0] = '\0';
  }
  else if (thisChar == '+' and lezen) {
    leesGeteld = leestel; lezen = 0; leestel = 0; uitkomst = 1;
  }
  else {
    lees[leestel++] = thisChar; lees[leestel] = '\0';
  }
  return uitkomst;
}



boolean regulieruitvoergelezen () {
  boolean uitkomst = 0;
  if ((lees[0] == 'U' and lees[1] == 'V') or (lees[0] == 'V' and lees[1] == 'U')) {
    uitkomst = 1;
  }
  return uitkomst;
}

boolean tijdgelezenproc(char* lees, int leesGeteld) {
  boolean uitkomst = 0;
  if (leesGeteld > 3 && (lees[0] == 'T' || lees[0] == 't') and (lees[1] == 'T' or lees[1] == 'U' or lees[1] == 't' || lees[1] == 'u')) {
    uitkomst = 1;
  }
  return uitkomst;
}

boolean SWgelezenproc() {
  boolean uitkomst = 0;
  if (lees[0] == 'S' and lees[1] == 'W') {
    uitkomst = 1;
  }
  return uitkomst;
}

boolean PSgelezenproc() {
  if (lees[0] == 'P' && lees[1] == 'S') return true; return false;
}

int leesnaaractie(char thisChar) {
  int uitkomst = 0;
  if (leesstandaardproc(thisChar)) {
    if (regulieruitvoergelezen ()) {
      uitkomst = 1;
    }
    else if (tijdgelezenproc(lees, leesGeteld)) {
      uitkomst = 2;
    }
    else if (SWgelezenproc()) {
      uitkomst = 3;
    }
    else if (PSgelezenproc()) uitkomst = 4;
  }
  return uitkomst;
}

int leesnaaractieUdp() {
  if (tijdgelezenproc(udpLees + 1, udpLeesL)) return 2;
  return 0;
}

boolean aanuitstuitv() {
  boolean uitkomst = 0;
  if (lees[4] == 'A' and lees[5] == 'N') {
    uitkomst = 1;
  }
  return uitkomst;
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




void verwerkcode(int switchvar, char* lees, byte leesaanv) {
  byte depin;
  byte jaarlen;
  switch (switchvar) {
    case 1:
      depin = uitvoerpin();
      if (depin == A4) piScreenC.set = aanuitstuitv();
      else if (depin == A2) piScreenBed.set = aanuitstuitv();
      else if (depin == piezopin) {
        bel[0] = aanuitstuitv(); if (bel[0] and !bel[1]) {
          bel[1] = 1, dod2F([](void* X, int y) {
            beltel = 0 - ((int)X + y);
          }, NULL), zetbel(belStStr = false);
        }
      }
      else if (depin == pinlamp[1])
        if ((lees[4] == 'u' && lees[5] == 't' && !(dim2 = false)) || (lees[4] == 'a' && lees[5] == 'n' && (dim2 = true))) {
          dim = true, dod2F([](void* X, int y) {
            *((int*)X) = 0 - y;
          }, &dimtel); if (dimstand[0] != dim2 * 255) analogWrite(depin, dimNu = dimstand[0] + dim2 * 1 - !dim2 * 1);
        }
        else dim = false, analogWrite(depin, dimNu = dimstand[0] = cv16waarde(lees[4], lees[5])), dimtel = 0;
      else digitalWrite(depin, aanuitstuitv());
      break;
    case 2:
      kwartup = 0;
      if (lees[0] == 'T') {
        kwartup = kwartup + 50;
      }
      if (lees[1] == 'U' || lees[1] == 'T') {
        kwartup = kwartup + 25;
      }
      if (tijdloopbool) {
        dod2F([](void* X, int y) {
          dodTel2 = y + 1;
        }, NULL);
        uurup = cvintcharint(lees[2]); secondeup = cvintcharint(lees[3]);
        timerup[0] = leesaanv;
        timerup[1] = cvintcharint(lees[4]);
        //kwartoud = kwartup;
        serKn.tijdF(secondeup);
      }
      else {
        if (lees[1] == 'T' || lees[1] == 't') {
          uur = cvintcharint(lees[3]) % 24; minuut = cvintcharint(lees[4]); seconde = cvintcharint(lees[5]); weekdag = (cvintcharint(lees[3]) / 24) * 6 + (cvintcharint(lees[6]) / 31) * 3 + (cvintcharint(lees[7]) / 12) + 1; dag = cvintcharint(lees[6]) % 31 + 1; maand = cvintcharint(lees[7]) % 12 + 1;
          jaar = 2000;
          jaarlen = 0;
          for (unsigned int tel = 8; lees[tel] != '\0'; tel++) {
            jaarlen++;
          }
          for (byte tel = 0; tel < jaarlen; tel++) {
            jaar = jaar + cvintcharint(lees[tel + 8]) * (macht(60, jaarlen - 1 - tel));
          }
          tijdtel = 0;
          tijdtemp = leesaanv;
          pauzetel = cvintcharint(lees[2]);
          distel = kwartup;
          //kwartoud = kwartup;
          zetdatdigit();
          serKn.tijdF(seconde);
          tijdloopbool = 1;
        }
      }
      break;
    case 3:
      depin = uitvoerpin();
      if (depin == piezopin) {
        bel[0] = aanuitstuitv();
      }
      else serKn.verwerk(depin);
      break;
    case 4:
      if (lees[4] == 'C' && lees[5] == 'U' && lees[6] == '0')
        if (lees[7] == '2')
          piScreenC.confirm(false);
        else if (lees[7] == '4')
          piScreenBed.confirm(false);
      break;
  }
}

unsigned int macht(byte y, byte x) {
  if (x-- == 0) return 1;
  for (byte z = y, t = 0; t < x; t++) y *= z;
  return y;
}

int uitvoerpin () {
  int uitkomst = 0;

  if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinlamp[0];
  }
  else if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '1') {
    uitkomst = pinlamp[1];
  }
  else if (lees[6] == 'B' and lees[7] == 'B' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = piezopin;
  }
  else if (lees[6] == 'S' and lees[7] == 'P' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinspeaker;
  }
  else if (lees[6] == 'K' and lees[7] == 'N') {
    uitkomst = 30;
  }
  else if (lees[6] == 'P' && lees[7] == 'I' && lees[8] == '0')
    if (lees[9] == '0') uitkomst = A4;
    else if (lees[9] == '1') uitkomst = A2;
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
    if (dimNu < 255 && [&] { if (dimT >= 255 - dimstand[0]) return !(dimNu == 255 || !(dimNu = 255)); byte DD = dimstand[0] + dimT; return dimNu != DD && ((dimNu = DD) || true); }()) analogWrite(pinlamp[1], dimNu);
  }
  else if (dimNu > 0 && [&] { if (dimT >= dimstand[0]) return !(!dimNu || (dimNu = 0)); byte DD = dimstand[0] - dimT; return dimNu != DD && ((dimNu = DD) || true); }()) analogWrite(pinlamp[1], [&] { if (dimT >= dimstand[0]) return 0; return dimstand[0] - dimT; }());
}

void loop() {
  telnetloop(), serKn.procKnop();
  if (!delayWacht && dimCheck) {
    dimCheck = false;
    char snd[] { '#', 'S', 'V', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'R', '+', '\0'};
    client.print(snd); dodTelMes = 24000;
  }
  if (dodBool) dodF();
  else if (Serial.available()) serKn.updateF();
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
      }, NULL), knopledupdatetimer(), dimproc(tel - dTel), dodTempF();
      if (Serial.available()) serKn.updateF();
    }
  }
  if (dodTel2 > 10) dodTel2 -= 11;
  else if (dodTel2) dodTel2 = 0;
  if (routtBB && dodTel3 > 10) dodTel3 -= 11;
}

void dodTempF() {
  if (temprekentel >= 1) {
    temprekentel = 0;
    tempreken /= 1;
    if (temp != 40 && tempreken <= convertemp[temp] + 3 && tempreken > convertemp[temp + 1] - 3) tempreken2[temprekentel2++] = temp;
    else for (byte tel = 0; tel < 41; tel++)
        if (tempreken <= convertemp[tel] and tempreken > convertemp[tel + 1]) {
          tempreken2[temprekentel2++] = tel;
          break;
        }
    if (temprekentel2 >= TEMP_AR_SIZE && tempmoduspr()) tempzend = 1;
    tempreken = 0;
  }
  tempreken += analogRead(pintemp); temprekentel++;
}


void tijdproc () {
  if (tijdloopbool) {
    if ((distel >= 100 and tijdtemp < 2) or ((distel != 0 and distel % 25 == 0) && tijdtemp == 2)) {
      if (pauzetel == 4 and tijdtemp < 2) {
        tijdtemp++;
        pauzetel = 0; pteloud = 88;
      }
      else if (pauzetel >= 19 + datwacht[0] + datwacht[1] && tijdtemp >= 2) {
        tijdtemp = 0; pauzetel = 0; oudminuut = 88; ouduur = 88; distel = 0;
      }
      else {
        pauzetel++;
      }
    }
    if (tijdtel >= 100) {
      tijdtel = 0;
      secondetel();
    }
    if (distel >= 100) {
      distel = 0;
    }
    if (!tijdtemp) {
      if (minuut != oudminuut or uur != ouduur or dispnu) {
        if (dispnu) {
          dispnu = 0;
        }
        if (distel < 50) {
          tijddigit(1); zettijd();
          ouduur = uur; oudminuut = minuut;
        }
        else {
          tijddigit(0); zettijd();
          ouduur = uur; oudminuut = minuut;
        }
      }
      else {
        if (distel == 0) {
          tijddigit(1); zettijd();
        }
        else if (distel == 50) {
          tijddigit(0); zettijd();
        }
      }
    }
    else if (tijdtemp == 1) {
      if (temp != oudtemp or dispnu) {
        if (dispnu) {
          dispnu = 0;
        }
        if (distel < 50) {
          tempdigit(1); zettijd();
        }
        else {
          tempdigit(0); zettijd();
        }
        oudtemp = temp;
      }
      else {
        if (distel == 0) {
          tempdigit(1); zettijd();
        }
        else if (distel == 50) {
          tempdigit(0); zettijd();
        }
      }
    }
    else if (tijdtemp == 2) {
      if (pteloud != pauzetel || dispnu) {
        if (dispnu) {
          dispnu = 0;
        }
        pteloud = pauzetel;
        datproc();
      }
    }
    tijdtel++; distel++;
  }
}

void datproc() {
  if (pauzetel == 0) {
    zetdatdigit();
  }
  byte wachter = datwacht[0] + datwacht[1];
  if (!dispniet()) {
    for (byte tel = 0; tel < 4; tel++) {
      digit[tel] = digitdat[(tel + pauzetel) % (19 + wachter)];
    }
    zettijd();
  }
}

void zetdatdigit() {
  digitdat[8] = converter[weekdag];
  byte tel = 11;
  datwacht[0] = wachtdat(dag, tel);
  datwacht[1] = wachtdat(maand, tel);
  digitdat[tel++] = converter[jaar / 1000];
  digitdat[tel++] = converter[(jaar / 100) % 10];
  digitdat[tel++] = converter[(jaar / 10) % 10];
  digitdat[tel++] = converter[jaar % 10];
}



boolean wachtdat(byte x, byte & tel) {
  boolean uitkomst = 0;
  if (x / 10) {
    uitkomst = 1;
    digitdat[tel++] = converter[x / 10];

  }
  digitdat[tel++] = converter[x % 10];
  digitdat[tel++] = 0x40;
  return uitkomst;
}

void secondetel() {
  if (!minuuttel()) {
    seconde++;
  }
  else {
    seconde = 0;
  }
  //  Serial.write(seconde / 10 + '0');
  //  Serial.write(seconde % 10 + '0');
  //  Serial.print("\n");
}


boolean minuuttel() {
  boolean uitkomst = 0;
  if (seconde == 59) {
    if (!uurtel()) {
      minuut++;
    }
    else {
      minuut = 0;
    }
    uitkomst = 1;
  }
  return uitkomst;
}

boolean uurtel() {
  boolean uitkomst = 0;
  if (minuut == 59) {
    if (uur == 23) {
      uur = 0;
      dagtel();
    }
    else {
      uur++;
    }
    uitkomst = 1;
  }
  return uitkomst;
}

void dagtel() {
  weekdag = weekdag % 7 + 1;
  if (!maandtel()) {
    dag++;
  }
  else {
    dag = 1;
  }
}

boolean maandtel() {
  if ((maand < 8 && ((maand % 2 && dag == 31) || (maand != 2 && !(maand % 2) && dag == 30) || (maand == 2 && ((!jaar % 4 && dag == 29) || (jaar % 4 && dag == 28))))) || (maand > 7 && ((maand % 2 && dag == 30) || (!(maand % 2) && dag == 31)))) {
    if (maand < 12) {
      maand++;
    }
    else {
      maand = 1;
    }
    return 1;
  }
  return 0;
}

void telnetloop () {
  if (delayWacht) {
    if (delayReady) delayWacht = false, delayReady = false, netwerksetup();
    return;
  }
  piScreenC.func();
  piScreenBed.func();
  if (tempzend) {
    zendtemp();
    tempzend = 0;
  }
  if (udpLeesL = udpBroadcast::readPack(udpLees, Udp)) {
    int x = leesnaaractieUdp();
    verwerkcode(x, x == 2 ? udpLees + 1 : udpLees, *udpLees - '@');
  }
  if (client.available()) dodTelMes = 24000, verwerkcode(leesnaaractie(client.read()), lees, leesaanv);
  boolean uit = false;
  for (byte t = 0; !client.connected() && (t < 20 || !(uit = true)); t++);
  if (uit)
    client.stop(), dodTelMes = 0, dod2F([](void* X, int y) {
    delayTel = 0 - y;
  }, NULL), delayWacht = true;
}

void statofadresdoorgeefproc () {
  for (int tel = 0; tel < 2; tel++) {
    client.write('@'); client.print("SD"); client.write(hetadres / 10 + '0'); client.write(hetadres % 10 + '0');
    if (tel == 0) {
      ANUTproc (pinlamp[0]);
      client.print ("LA00");
    }
    else {
      ANUTproc (pinspeaker);
      client.print ("SP00");
    }
    client.write('+'); dodTelMes = 24000;
  }
  zendtemp();
}

void zendtemp() {
  char zend[7] = "@TE1";
  zend[4] = cvintcharchar(temp + 20);
  zend[5] = '+'; zend[6] = '\0';
  client.print(zend); dodTelMes = 24000;
}

void ANUTproc (int lees) {
  if (digitalRead(lees)) {
    client.print("AN");
  }
  else {
    client.print("UT");
  }
}

boolean tempmoduspr() {
  byte cnt[TEMP_AR_SIZE]; for (byte t = 0; t < TEMP_AR_SIZE; t++) cnt[t] = 1;
  for (byte t1 = 0; t1 < TEMP_AR_SIZE - 1; t1++) for (byte t2 = t1 + 1; t2 < TEMP_AR_SIZE; t2++) if (tempreken2[t2] == tempreken2[t1]) cnt[t1]++;
  byte modus = 0; for (byte t = 1; t < TEMP_AR_SIZE; t++) if (cnt[t] > cnt[modus]) modus = t; modus = tempreken2[modus];
  temprekentel2 = 0; if (temp == modus) return false;
  temp = modus; return true;
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

void dodeman (int tDiff, boolean tijdup) {
  if (dodTelMes && !--dodTelMes) dodTelMes = 24000, client.print("@DM+");
  piScreenC.cntF(tDiff);
  piScreenBed.cntF(tDiff);
  serKn.timF();
  if (tijdup) {
    uur = uurup; seconde = secondeup; tijdtemp = timerup[0];
    pauzetel = timerup[1];
    distel = kwartup;
    oudminuut = 88; ouduur = 88; tijdtel = 0; tijdup = 0;
  }
  tijdproc(); if (dim && dimtel < 512 && [](int tDifff) {
  dod2F([](void* X, int y) {
    *((int*)X) += y;
    }, &tDifff); return tDifff;
  }(++dimtel + tDiff) >= 512) dimCheck = true, dimtel = 512;
  if (delayWacht && !delayReady) if (delayTel++ >= 100) delayTel = 0, delayReady = true;
  dodBelF(tDiff);
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

boolean dispniet() {
  if (bel[1]) return true;
  return false;
}

void zetbel (boolean stand) {
  byte beldat [4] = {0x00, 0x7C, 0x79, 0x38};
  for (byte tel = 0; tel < 4; tel++) {
    digit[tel] = beldat[(tel + !stand) % 4];
  }
  uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
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

void knopledupdatetimer () {
  byte TT; if (!tijdtel) TT = 99; else TT = tijdtel - 1;
  analogWrite (PIN_COS_KN, (int)100 * (1 + cos((TT + [&] {
    if (seconde % 2) return 50;
    else if (TT >= 50) return - 50; //terug
    else return 150; //terug
  }()) * 2 * PI / 200)) + 55);
}
