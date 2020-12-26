#include <SPI.h>
#include <Ethernet.h>
#include <TM1637Display.h>
#include <TimerOne.h>
#include "UdpBroadcast.hpp"


char udpLees[11];
int udpLeesL = 0;
int leesGeteld;
int dodTelMes = 0;

const int CLK = 9; const int DIO = 8;
byte converter[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; byte converterpnt[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
TM1637Display display(CLK, DIO);
byte ouduur = 88;
byte oudminuut = 88;
const int hetadres = 0; // zoek naar verander
byte mac[] = {
  0xDE, 0xBD, 0xBE, 0xEF, 0xFE, 0xEE
};
IPAddress ip(192, 168, 179, 202);
byte catstel = 0;
byte decat = 1;
unsigned int cattel = 0;
boolean dispnu = 0;
const int pinspeaker = A0;
const int pinlamp[3] = {2, 4, 6};
const int piezopin = 3;
const int pinledkn[1] = { 5 };
const int pinkn[1] = { 7 };
byte cataant = 0;
boolean knop[9];
byte dimNu = 0;
char knopMem;
boolean knopst[9];
int knoptel2[9];
boolean knopuitgestuurd2[9];
boolean knopuitDimgestuurd[9] { false, false, false, false, false, false, false, false, false };
boolean knopdim2[9];
boolean bel [2] = {0, 0};
boolean opstart = 1;
boolean tijdloopbool = 0;
unsigned int datgezet = 9999;
byte pteloud = 88;
byte leesaanv;
byte dag;
byte maand;
byte digitdat[21] = {0x00, 0x00, 0x00, 0x00, 0x5E, 0x77, 0x6F, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte timerup[2];
byte kwartup;
byte tijdtemp = 0;
unsigned int jaar;
byte weekdag;
unsigned int distel;
byte seconde;
byte minuut;
byte uur;
byte tijdtel;
char lees[31];
boolean tijdgeupdated = 0;
int leestel;
boolean lezen = 0;
int actieuvbufferaant = 0;
int actieuvbuffer[100];
int beltel = 0;
boolean teldodeman = 0;
boolean knopwacht = 0;
byte wachtknop = 88;
int knoptel = 0;
boolean knopdim[1] = {0};
boolean standknop = 0;
boolean knopwachter = 0;
boolean knoptelst = 0;
byte digit[4] = {0x00, 0x00, 0x00, 0x00};
byte secondeup;
byte minuutup;
byte uurup;
boolean knopuitgestuurd = 0;
boolean knoptels = 0;
boolean knop1nawacht = 0;
boolean knopnawacht[9];
int convertemp[42] = {785, 775, 766, 756, 746, 735, 725, 714, 704, 693, 682, 671, 660, 648, 637, 626, 614, 603, 591, 580, 568, 557, 545, 534, 523, 512, 500, 489, 478, 467, 456, 445, 434, 424, 413, 403, 393, 383, 373, 364, 354, 345};
int tempreken[2] = {0, 0};
byte temprekentel[2] = {0, 0};
byte temp = 40;
byte oudtemp = 0;
byte pauzetel = 0;
boolean datwacht[2];


boolean dim = false, dim2 = false;
int dimtel = 0;
byte dimstand[1] = {0};
volatile boolean dimCheck = false;
volatile boolean delayWacht = false, delayReady = false;
int delayTel = 0;
volatile boolean dodBool, dodBool3, dodBool2, dodBool1 = dodBool = dodBool3 = !(dodBool2 = true);
unsigned int dodTel = 0, dodTel2, dodTel3 = 0;

boolean belStStr;
byte belTone = 0;
// Enter the IP address of the server you're connecting to:
IPAddress server(192, 168, 179, 19);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 23):
EthernetClient client;
EthernetUDP Udp;

void setup() {
  for (byte tel = 0; tel < 9; tel++) {
    knop[tel] = 0;
    knopst[tel] = 0;
    knoptel2[tel] = 0;
    knopuitgestuurd2[tel] = 0;
    knopdim2[tel] = 0;
    knopnawacht[tel] = 0;
  }
  pinMode (piezopin, OUTPUT);
  pinMode (pinlamp[0], OUTPUT);
  pinMode (pinlamp[1], OUTPUT);
  pinMode (pinlamp[2], OUTPUT);
  pinMode (pinspeaker, OUTPUT);
  pinMode (pinkn[0], INPUT);
  pinMode (pinledkn[0], OUTPUT);
  display.setBrightness(0x0d);
  noTone(piezopin);

  Timer1.initialize(10000), Timer1.stop(), Timer1.restart();

  Ethernet.begin(mac, ip);
  // Open serial communications and wait for port to open:
  Serial.begin(74880);
  while (!Serial) {
    // ; // wait for serial port to connect. Needed for native USB port only
  }
  char inChar = '~';
  Serial.println("S");
  boolean esc = false;
  while (!esc) {
    while (Serial.available()) {
      if ((inChar = (char)Serial.read()) != '\n') {
        if (inChar != '\r') knopMem = inChar;
      }
      else if (knopMem == 'r') {
        Serial.println("s");
        esc = true;
        break;
      }
    }
  }

  // give the Ethernet shield a second to initialize:
  delay(1000);

  Timer1.start(); Timer1.attachInterrupt([] {
    if (!dodBool2 && (dodBool3 = true)) return;
    if (dodBool1 && (dodBool1 = false))
      if (!dodBool) dodBool = true;
      else if (dodTel > 2000) dodTel = 0;
      else dodTel++;
    if (dodBool)
      if (dodTel > 2000) dodTel = 0;
      else dodTel++;
    dodBool = true;
  });
  if (client.connect(server, 10002)) {
    char snd[] {'|', '-', hetadres / 10 + '0', hetadres % 10 + '0', '+', '|', '#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', 'R', '+', '\0'};
    client.print(snd); dodTelMes = 24000;
  }
  Udp.begin(1235);


  netwerksetup ();

}



void netwerksetup () {
  // give the Ethernet shield a second to initialize:
  if (!opstart) {
    dispnu = 0;
    cattel = 0;
    catstel = 0;
    beltel = 0;
    dodTel2 = 0;
    dim = false, dim2 = false;
    dimCheck = false;
    tijdloopbool = 0;
    ouduur = 88;
    knop1nawacht = 0;
    oudminuut = 88;
    knoptels = 0;
    bel[0] = 0;
    teldodeman = 0;
    knopwacht = 0;
    tijdgeupdated = 0;
    knoptelst = 0;
    wachtknop = 88;
    standknop = 0;
    knoptel = 0;
    knopdim[0] = 0;
    for (byte tel = 0; tel < 9; tel++) {
      knopdim2[tel] = 0;
      knoptel2[tel] = 0;
      knopnawacht[tel] = 0;
    }
    knopwachter = 0;
    knopuitgestuurd = 0;
    leestel = 0;
    lezen = 0;
    if (client.connect(server, 10002)) {
      char snd[] {'|', '-', hetadres / 10 + '0', hetadres % 10 + '0', '+', '|', '#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', 'R', '+', '\0'};
      client.print(snd); dodTelMes = 24000;
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
    lees[leestel] = thisChar; lees[leestel + 1] = '\0'; leestel++;
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

boolean KCgelezenproc() {
  boolean uitkomst = 0;
  if (lees[0] == 'K' and lees[1] == 'C') {
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

boolean tijdgelezenproc(char* lees, int leesGeteld) {
  boolean uitkomst = 0;
  if (leesGeteld > 3 && (lees[0] == 'T' || lees[0] == 't') and (lees[1] == 'T' or lees[1] == 'U' or lees[1] == 't' || lees[1] == 'u')) {
    uitkomst = 1;
  }
  return uitkomst;
}

boolean tempgelezenproc() {
  if (lees[0] == 'T' and lees[1] == 'E') {
    return 1;
  }
  return 0;
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
    else if (tempgelezenproc()) {
      uitkomst = 4;
    }
    else if (KCgelezenproc()) {
      uitkomst = 5;
    }
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
  unsigned int reken;
  byte jaarlen;
  byte depin;
  switch (switchvar) {
    case 1:
      depin = uitvoerpin();
      if (depin == piezopin) {
        bel[0] = aanuitstuitv(); if (bel[0] and !bel[1]) {
          bel[1] = 1, dod2F([](void* X, int y) {
            beltel = 0 - ((int)X + y);
          }, NULL), zetbel(belStStr = false); if (belTone != 2) belTone = 2, tone(piezopin, 1200);
        }
      }
      else if (depin == pinlamp[2])
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
        Serial.write(secondeup % 4 + 't'), Serial.println();
      }
      else {
        if (lees[1] == 'T' || lees[1] == 't') {
          uur = cvintcharint(lees[3]) % 24; minuut = cvintcharint(lees[4]); seconde = cvintcharint(lees[5]) ; weekdag = (cvintcharint(lees[3]) / 24) * 6 + (cvintcharint(lees[6]) / 31) * 3 + (cvintcharint(lees[7]) / 12) + 1; dag = cvintcharint(lees[6]) % 31 + 1; maand = cvintcharint(lees[7]) % 12 + 1;
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
          Serial.write(seconde % 4 + 't'), Serial.println();
          tijdloopbool = 1;
        }
      }
      break;
    case 3:
      depin = uitvoerpin();
      if (depin == piezopin) {
        bel[0] = aanuitstuitv();
      }
      else {
        reken = (lees[8] - '0') * 10 + (lees[9] - '0');
        Serial.write(reken / 9 + 'j'), Serial.println();
        if (lees[4] == 'U' && lees[5] == 'T') lees[4] = lees[5] = '0'; else if (lees[4] == 'A' && lees[5] == 'N') lees[4] = lees[5] = 'f';
        else if (lees[4] == 'a' && lees[5] == 'n') lees[4] = lees[5] = 'X';
        else if (lees[4] == 'u' && lees[5] == 't') lees[4] = lees[5] = 'x';
        Serial.println(lees[4]), Serial.println(lees[5]);
        Serial.write(reken % 9 + 'A'), Serial.println();
      }
      break;
    case 4:
      temp = cvintcharint(lees[3]) - 20;
      break;
    case 5:
      cataant = lees[2] - '0';
      break;
  }
}

int uitvoerpin () {
  int uitkomst = 0;
  if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinlamp[0];
  }
  else if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '1') {
    uitkomst = pinlamp[1];
  }
  else if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '2') {
    uitkomst = pinlamp[2];
  }
  else if (lees[6] == 'S' and lees[7] == 'P' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinspeaker;
  }
  else if (lees[6] == 'B' and lees[7] == 'B' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = piezopin;
  }
  else if (lees[6] == 'K' and lees[7] == 'N') {
    uitkomst = 30;
  }
  return uitkomst;
}

void serialEventF() {
  char inChar = '~';
  while (Serial.available())
    if ((inChar = (char)Serial.read()) != '\n') {
      if (inChar != '\r') knopMem = inChar;
    }
    else if (knopMem >= 'a' and knopMem <= 'i') knop[knopMem - 'a'] = 0;
    else if (knopMem >= 'A' and knopMem <= 'I') knop[knopMem - 'A'] = 1;
    else if (knopMem == 'r') {
      Serial.println("s");
      Serial.write(decat + 73);
      Serial.write('\n');
      char snd[] {'#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', '+', '\0'};
      client.print(snd); dodTelMes = 24000;
    }
}

void dimproc(int tDiff) {
  if (!dim) return;
  int dimT = dimtel + tDiff;
  dod2F([](void* X, int y) {
    *((int*)X) += y;
  }, &dimT);
  if (dimT < 2) return;
  else if (dim2) {
    if (dimNu < 255 && [&] { if (dimT >= 255 - dimstand[0]) return !(dimNu == 255 || !(dimNu = 255)); byte DD = dimstand[0] + dimT; return dimNu != DD && ((dimNu = DD) || true); }()) analogWrite(pinlamp[2], dimNu);
  }
  else if (dimNu > 0 && [&] { if (dimT >= dimstand[0]) return !(!dimNu || (dimNu = 0)); byte DD = dimstand[0] - dimT; return dimNu != DD && ((dimNu = DD) || true); }()) analogWrite(pinlamp[2], [&] { if (dimT >= dimstand[0]) return 0; return dimstand[0] - dimT; }());
}

void loop() {
  telnetloop(), procknop();
  for (byte tel = 0; tel < 9; tel++) procknop2(tel);
  if (!delayWacht && dimCheck) {
    dimCheck = false;
    char snd[] { '#', 'S', 'V', (char)(hetadres / 10 + '0'), (char)(hetadres % 10 + '0'), 'R', '+', '\0'};
    client.print(snd); dodTelMes = 24000;
  }
  if (dodBool) dodF();
  else serialEventF();
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
      }, NULL), knopledupdatetimer(), dimproc(tel - dTel);
      serialEventF();
    }
  }
  if (dodTel2 > 10) dodTel2 -= 11;
  else if (dodTel2) dodTel2 = 0;
  if (routtBB && dodTel3 > 10) dodTel3 -= 11;
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
      if (temp != oudtemp || dispnu) {
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

boolean wachtdat(byte x, byte& tel) {
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

void procknop() {
  int plusTel;
  dod2F([](void* X, int y) {
    *((int*)X) = y;
  }, &plusTel);
  if (knopwacht) {
    if (knopproc(plusTel) == 99) {
      knoptels = 0;
      knopdim[wachtknop] = !knopdim[wachtknop]; wachtknop = 88; knoptel = 0 - plusTel; knopuitgestuurd = 0; knop1nawacht = 1;
    }
  }
  else {
    if (knop1nawacht && knoptel + plusTel > 30) {
      knop1nawacht = 0; knoptel = 0;
    }
    else {
      for (byte tel = 0; tel < 1; tel++) {
        if (knopproc(plusTel) == tel) {
          if (!catstel) {
            catstel = 1;

          }
          else {
            if (catstel == 1) {
              catstel++;
            }
            for (byte tel = 0; tel < 9; tel++) {
              eindprocknop2(tel, plusTel);
            }
            decat = decat % cataant + 1;
            Serial.write(decat + 73); Serial.write('\n');
          }
          cattel = 0;
          zettemp();
          knoptel = 0 - plusTel; knoptels = 1; knopuitgestuurd = 0;
          wachtknop = tel;
          break;
        }
      }
    }
  }
}

void procknop2(byte no) {
  int tDiff, knopTelN;
  dod2F([](void* x, int y) {
    *((int*)x) = y;
  }, &tDiff); knopTelN = tDiff + knoptel2[no];
  if (knopst[no]) {
    if (knopTelN >= 100 && !knopuitgestuurd2[no]) {
      char snd[32] = {'@', 'S', 'I', '0', '0', 'U', 'T', 'K', 'N' };
      knopuitgestuurd2[no] = 1; sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000;
    }
    if ((!knop[no] && knopTelN > 3) || knopTelN >= 255) {
      char snd[32] = {'@', 'S', 'I', '0', '0', 'U', 'T', 'K', 'N' };
      if (!knopuitDimgestuurd[no]) {
        knopuitDimgestuurd[no] = true;
        snd[5] = cv16char0(knopTelN), snd[2] = 'u';
        if (!knopdim2[no]) snd[6] = cv16char1(knopTelN); else snd[6] = knopTelN % 16 + 'A';
        sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000;
      }
      if (knop[no] || knopTelN <= 3) return;
      knopst[no] = 0;
      if (knopTelN < 100) snd[2] = snd[5] = 'I', snd[6] = 'D', sluitknpr(no, snd + 9), client.print(snd), dodTelMes = 24000; 
      knopdim2[no] = !knopdim2[no]; knoptel2[no] = -tDiff; knopuitDimgestuurd[no] = knopuitgestuurd2[no] = 0; knopnawacht[no] = 1;
    }
  }
  else {
    if (knopnawacht[no] && knopTelN >= 30) {
      knopnawacht[no] = 0; knoptel2[no] = 0;
    }
    else {
      if (knop[no]) {
        char snd[32] = {'@', 'S', 'u', '0', '0', 'A', 'N', 'K', 'N' };
        if (!knopdim2[no]) snd[5] = 'U', snd[6] = 'T';
        sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000; knoptel2[no] = - tDiff; knopst[no] = 1; knopuitgestuurd2[no] = 0;
      }
    }
  }
}

void eindprocknop2(byte no, int plusTel) {
  int plusTel2 = plusTel + knoptel2[no];
  if (knopst[no]) {
    char snd[32] = {'@', 'S', 'I', '0', '0', 'U', 'T', 'K', 'N' };
    if (plusTel2 >= 100 and !knopuitgestuurd2[no]) {
      sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000;
    }
    knopst[no] = 0;
    if (!knopuitDimgestuurd[no]) {
      snd[5] = cv16char0(plusTel), snd[2] = 'u';
      if (!knopdim2[no]) snd[6] = cv16char1(plusTel2); else snd[6] = plusTel2 % 16 + 'A';
      sluitknpr(no, snd + 9); client.print(snd); dodTelMes = 24000;
    }
    if (plusTel2 < 100) snd[2] = snd[5] = 'I', snd[6] = 'D', sluitknpr(no, snd + 9), client.print(snd), dodTelMes = 24000;
    knopdim2[no] = !knopdim2[no]; knoptel2[no] = 0 - plusTel; knopuitDimgestuurd[no] = 0; knopuitgestuurd2[no] = 0; knopnawacht[no] = 1;
  }
}

void sluitknpr(byte no, char* ls) {
  byte y = (decat - 1) * 9 + no;
  *(ls++) = y / 10 + '0'; *(ls++) = y % 10 + '0'; *(ls++) = '+'; *ls = '\0';
}


byte knopproc(int plusTel) {
  byte uitkomst = 88;
  if (!knopwacht) {
    for (byte tel = 0; tel < 1; tel++) {
      if (knops(tel)) {
        uitkomst = tel; knopwacht = 1;
        break;
      }
    }
  }
  else {
    if (!digitalRead (pinkn[0]) and knoptel + plusTel > 3) { // and !digitalRead (knop[1]) and !digitalRead (knop[2])) {
      //  knoptelst = 1;
      knopwacht = 0; uitkomst = 99;
    }
  }
  return uitkomst;
}

boolean knops(byte no) {
  boolean uitkomst = 0;
  // byte niet[2];
  // switch (no) {
  //  case 0:
  //    niet[0] = 1; niet[1] = 2;
  ////    break;
  //  case 1:
  //  niet[0] = 0; niet[1] = 2;
  //   break;
  // case 2:
  //   niet[0] = 0; niet[1] = 1;
  //   break;
  // }
  if (digitalRead(pinkn[no])) { // and !digitalRead(knop[niet[0]]) and !digitalRead(knop[niet[1]])) {
    uitkomst = 1;
  }
  return uitkomst;
}


void telnetloop () {
  if (delayWacht) {
    if (delayReady) delayWacht = false, delayReady = false, netwerksetup ();
    return;
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


  for (int tel = 0; tel < 3; tel++) {
    client.write('@'); client.print("SD"); client.write(hetadres / 10 + '0'); client.write(hetadres % 10 + '0');

    if (tel == 0) {
      ANUTproc (pinlamp[0]);
      client.print ("LA00");
    }
    else if (tel == 1) {
      ANUTproc (pinlamp[1]); //Verander
      client.print ("LA01");

    }
    else {
      ANUTproc (pinspeaker); //Verander
      client.print ("SP00");
    }

    client.write('+'); dodTelMes = 24000;
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

void dodBelF(int tDiff) {
  if (!bel[1]) return;
  int belTT;
  dod2F([](void* X, int y) {
    *((int*)X) = y;
  }, &belTT), belTT += tDiff + beltel++;
  if (belTT >= 280)
    if (!bel[0]) {
      bel[1] = 0, dispnu = 1, belTone = 0; if (belTone) noTone(piezopin); beltel = 0; return;
    }
    else beltel = 1 - (((belTT - tDiff) - (beltel - 1)) + tDiff), belTT = 0;
  byte AA; int UUU = belTT;
  if (((((((belTT < 35) || (belTT >= 70 && belTT < 105) || (belTT >= 140 && belTT < 175)) && (AA = 2)) || (belTT >= 210 && belTT < 245 && !(AA = 0))) && (belTT = 1)) || ((belTT < 210 && (AA = 1) && !(belTT = 0)) || !(AA = belTT = 0))) && belStStr == (boolean)(belTT))
    [&] {
    if (AA != belTone) if (AA) belTone = AA, tone(piezopin, AA * 600);
      else belTone = 0, noTone (piezopin);
  }(), zetbel(belStStr = !belStStr);
}


void dodeman (int tDiff, boolean tijdup) {
  if (dodTelMes && !--dodTelMes) dodTelMes = 24000, client.print("@DM+");
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

  if ((knoptels or knop1nawacht) and knoptel < 255) {
    knoptel++;
  }
  if (catstel) {
    if (cattel >= 300) {
      catstel = 0; cattel = 0; dispnu = 1;
    }
    else {
      if (catstel == 2 && (cattel != 0 && !(cattel % 10))) {
        zettemp();
      }
      else {
        if (dispnu) {
          zettemp();
          dispnu = 0;
        }
      }
      cattel++;
    }
  }
  for (byte tel = 0; tel < 9; tel++) {
    if ((knopst[tel] or knopnawacht[tel]) and knoptel2[tel] < 255) {
      knoptel2[tel]++;
    }
  }
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
  if (!bel[1]) {
    digit[0] = 0x39; digit[1] = 0x77; digit[2] = 0x78;
    if ((cattel / 10) % 2 && catstel == 2) {
      digit[3] = 0x00;
    }
    else {
      digit[3] = converter[decat];
    }
    uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
  }
}

void zetbel (boolean stand) {
  byte beldat [4] = {0x00, 0x7C, 0x79, 0x38};
  for (byte tel = 0; tel < 4; tel++) {
    digit[tel] = beldat[(tel + !stand) % 4];
  }
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
  if (catstel || bel[1]) {
    return 1;
  }
  return 0;
}

void knopledupdatetimer () {
  byte TT; if (!tijdtel) TT = 99; else TT = tijdtel - 1;
  analogWrite (pinledkn[0], (int)100 * (1 + cos((TT + [&] {
    if (seconde % 2) return 50;
    else if (TT >= 50) return - 50; //terug
    else return 150; //terug
  }()) * 2 * PI / 200)) + 55);
}
