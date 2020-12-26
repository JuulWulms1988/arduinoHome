#define TEMP_AR_SIZE 50
#include "Router.h"
#include <TimerOne.h>
#include <TM1637Display.h>
#include "UdpBroadcast.hpp"
EthernetUDP Udp;
char udpLees[11];
int udpLeesL = 0;
int dodTelMes = 0;
routerCls routC;
const int hetadres = 2; // zoek naar verander
const int pintemp = A5;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC
};
IPAddress ip(192, 168, 179, 204);
const int CLK = 9; const int DIO = 6;
TM1637Display display(CLK, DIO);
byte converter[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; byte converterpnt[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
byte ouduur = 88;
byte oudminuut = 88;
volatile boolean tijdloopbool = 0;
volatile boolean tijdup = false;
unsigned int datgezet = 9999;
byte pteloud = 88;
byte leesaanv;
volatile byte dag;
volatile byte maand;
byte digitdat[21] = {0x00, 0x00, 0x00, 0x00, 0x5E, 0x77, 0x6F, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte timerup[2];
byte kwartup;
volatile unsigned int jaar;
byte weekdag;
unsigned int distel;
volatile byte seconde;
volatile byte minuut;
volatile byte uur;
byte tijdtel;
byte digit[4] = {0x00, 0x00, 0x00, 0x00};
byte secondeup;
byte uurup;
const byte routPin = 2;
const int pinlamp[1] = {7};
const int pinspeaker = A0;
byte tijdtemp = 0;
int convertemp[42] = {785, 775, 766, 756, 746, 735, 725, 714, 704, 693, 682, 671, 660, 648, 637, 626, 614, 603, 591, 580, 568, 557, 545, 534, 523, 512, 500, 489, 478, 467, 456, 445, 434, 424, 413, 403, 393, 383, 373, 364, 354, 345};
byte temprekentel2 = 0;
int tempreken = 0;
byte tempreken2[TEMP_AR_SIZE];
byte temprekentel = 0;
byte temp = 40;
byte oudtemp = 0;
byte pauzetel = 0;
boolean datwacht[2];
boolean opstart = 1;

char lees[31];
int leestel;
int leesGetelt;
boolean lezen = 0;
boolean tempzend = 0;
boolean delayWacht = false, delayReady = false;
int delayTel = 0;
volatile boolean dodBool, dodBool3, dodBool2, dodBool1 = dodBool = dodBool3 = !(dodBool2 = true);
unsigned int dodTel = 0, dodTel2, dodTel3 = 0;
// Enter the IP address of the server you're connecting to:
IPAddress server(192, 168, 179, 19);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 23):
EthernetClient client;

int belTel; boolean belStaat = false; volatile boolean volBel = false, readBel = false;

void setup() {
  pinMode (routPin, OUTPUT), digitalWrite (routPin, LOW);
  pinMode (4, INPUT);
  pinMode (pintemp, INPUT);
  pinMode (pinlamp[0], OUTPUT), digitalWrite(pinlamp[0], LOW);
  //  pinMode (pinlamp[1], OUTPUT);
  pinMode (pinspeaker, OUTPUT), digitalWrite(pinspeaker, LOW);
  display.setBrightness(0x0d);
  Timer1.initialize(10000);


  stopdetimer ();

  Ethernet.begin(mac, ip);
  Udp.begin(1235);
  // Open serial communications and wait for port to open:
  //  Serial.begin(9600);
  //  while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  //}


  // give the Ethernet shield a second to initialize:
  delay(1000);

  Timer1.start(); Timer1.attachInterrupt([] {
    volBel = (readBel = digitalRead(4)) || volBel;
    if (tijdup) {
      uur = uurup; seconde = secondeup; tijdtemp = timerup[0];
      pauzetel = timerup[1];
      distel = kwartup;
      oudminuut = 88; ouduur = 88; tijdtel = 0; tijdup = 0;
    }
    tijdproc();
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
    char snd[] {'|', '-', hetadres / 10 + '0', hetadres % 10 + '0', '+', '|', '@', 'S', 'I', '0', '2', 'U', 'T', 'B', 'L', '0', '0', '+', '#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', 'R', '+', '\0'};
    client.print(snd); dodTelMes = 24000;
  }


  netwerksetup ();


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

void netwerksetup () {
  // give the Ethernet shield a second to initialize:
  if (!opstart) {
    dodTel2 = 0;
    tijdloopbool = 0;
    ouduur = 88;
    oudminuut = 88;
    tijdup = false;
    leestel = 0;
    lezen = 0;
    tempzend = 0;
    if (client.connect(server, 10002)) {
      char snd[] {'|', '-', hetadres / 10 + '0', hetadres % 10 + '0', '+', '|', '@', 'S', 'I', '0', '2', 'U', 'T', 'B', 'L', '0', '0', '+', '#', 'S', 'V', hetadres / 10 + '0', hetadres % 10 + '0', 'R', '+', '\0'};
      if (belStaat) snd[11] = 'I', snd[12] = 'D'; client.print(snd); zendtemp();
    }
  }
  opstart = 0;
}


void knoploop () {
  if ((volBel && !(volBel = false) || readBel) == belStaat || belTel && [](bool x) {
  dod2F([](void* X, int y) {
    *((boolean*)X) = belTel > y;
    }, &x); return x;
  }(false)) return;
  if (!delayWacht) {
    if (belStaat) client.print("@SI02UTBL00+");
    else client.print("@SI02IDBL00+");
    dodTelMes = 24000;
  }
  belStaat = !belStaat;
  dod2F([](void* X, int y) {
    belTel = y + 2;
  }, NULL);
}



boolean leesstandaardproc(char thisChar) {
  boolean uitkomst = 0;
  if (thisChar >= '@' and thisChar <= '@' + 2 and !lezen) {
    leesaanv = thisChar - '@';
    lezen = 1; leestel = 0; lees[0] = '\0';
  }
  else if (thisChar == '+' and lezen) {
    leesGetelt = leestel; lezen = 0; leestel = 0; uitkomst = 1;
  }
  else {
    lees[leestel] = thisChar; lees[leestel + 1] = '\0'; leestel++;
  }
  return uitkomst;
}

boolean regulieruitvoergelezen () {
  boolean uitkomst = 0;
  if (lees[0] == 'U' and lees[1] == 'V') {
    uitkomst = 1;
  }
  return uitkomst;
}

boolean tijdgelezenproc(char* lees, int leesGetelt) {
  boolean uitkomst = 0;
  if (leesGetelt > 3 && (lees[0] == 'T' || lees[0] == 't') and (lees[1] == 'T' or lees[1] == 'U' or lees[1] == 't' || lees[1] == 'u')) {
    uitkomst = 1;
  }
  return uitkomst;
}

int leesnaaractie(char thisChar) {
  int uitkomst = 0;
  if (leesstandaardproc(thisChar)) {
    if (regulieruitvoergelezen ()) {
      uitkomst = 1;
    }
    else if (tijdgelezenproc(lees, leesGetelt)) {
      uitkomst = 2;
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

void verwerkcode(int switchvar, char* lees, byte leesaanv) {
  byte jaarlen;
  switch (switchvar) {
    case 1:
      digitalWrite(uitvoerpin(), aanuitstuitv());
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
        tijdup = true;
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
          tijdloopbool = 1;
        }
      }
      break;
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

int uitvoerpin () {
  int uitkomst = 0;

  if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinlamp[0];
  }
  else if (lees[6] == 'S' and lees[7] == 'P' and lees[8] == '0' and lees[9] == '0') {
    uitkomst = pinspeaker;
  }
  //  if (lees[6] == 'L' and lees[7] == 'A' and lees[8] == '0' and lees[9] == '1') {
  //    uitkomst = pinlamp[1];
  //  }
  return uitkomst;
}

void loop() {
  telnetloop(), knoploop();
  if (dodBool) dodF();
}

void dodF() {
  auto x = [] {
    if (dodBool3) dodBool3 = false, dodBool1 = true;
  };
  dodBool = dodBool2 = false;
  if (dodBool1) dodBool1 = false, dodTel++;
  boolean routtBB = true;
  byte tel; if (dodTel > 10) {
    tel = 10, dodTel -= 11;
    dodBool2 = dodBool = true;
    x();
  } else tel = dodTel, dodTel = 0, dodBool2 = true, x();
  {
    for (unsigned int dTel = 0; dTel <= tel; dTel++) {
      dodeman(tel - dTel, [&] { if (dodTel2 && dodTel2 - 1 == dTel) return true; return false; }());
      if (routtBB && dTel == dodTel3) {
        routtBB = false, dod2F([](void* X, int y) {
          if (y) dodTel3 = y - 1; else dodTel3 = 0;
        }, NULL), dodTempF();
        if (!routC.flagBl) routC.func();
      }
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
    if (temp != 40 && tempreken <= convertemp[temp] + 3 && tempreken > convertemp[temp + 1] - 3) tempreken2[temprekentel2++] = temp; else
      for (byte tel = 0; tel < 41; tel++)
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
      if (minuut != oudminuut or uur != ouduur) {
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
      if (temp != oudtemp) {
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
      if (pteloud != pauzetel) {
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
  for (byte tel = 0; tel < 4; tel++) {
    digit[tel] = digitdat[(tel + pauzetel) % (19 + wachter)];
  }
  zettijd();
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
  if (routC.flagBl) routC.flagF();
  if (delayWacht) {
    if (!delayReady) return;
    delayWacht = false, delayReady = false;
    //   if (regresettel >= 10) {
    //   digitalWrite (pinreset, HIGH);
    //  }
    netwerksetup ();
    return;
  }
  if (tempzend) {
    zendtemp();
    tempzend = 0;
  }
  if (udpLeesL = udpBroadcast::readPack(udpLees, Udp)) {
    int x = leesnaaractieUdp();
    verwerkcode(x, x == 2 ? udpLees + 1 : udpLees, *udpLees - '@');
  }
  if (client.available()) {
    char c = client.read();
    //   Serial.write(c);
    dodTelMes = 24000;
    verwerkcode(leesnaaractie(c), lees, leesaanv);
  }



  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:





  // Serial.println (telkaas);

  // if the server's disconnected, stop the client:

  if (routC.flagBl) routC.flagF();
  boolean uit = false;
  for (byte t = 0; !client.connected() && (t < 20 || !(uit = true)); t++);
  if (uit) {
    client.stop();
    dodTelMes = 0;
    dod2F([](void* X, int y) {
      delayTel = 0 - y;
    }, NULL);
    delayWacht = true;
  }
}

void statofadresdoorgeefproc () {

}

void zendtemp() {
  char zend[7] = "@TE2";
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
void stopdetimer () {
  Timer1.stop();        //stop the counter
  Timer1.restart();
}
boolean tempmoduspr() {
  byte cnt[TEMP_AR_SIZE]; for (byte t = 0; t < TEMP_AR_SIZE; t++) cnt[t] = 1;
  for (byte t1 = 0; t1 < TEMP_AR_SIZE - 1; t1++) for (byte t2 = t1 + 1; t2 < TEMP_AR_SIZE; t2++) if (tempreken2[t2] == tempreken2[t1]) cnt[t1]++;
  byte modus = 0; for (byte t = 1; t < TEMP_AR_SIZE; t++) if (cnt[t] > cnt[modus]) modus = t; modus = tempreken2[modus];
  temprekentel2 = 0; if (temp == modus) return false;
  temp = modus; return true;
}


void dodeman (byte tDiff, boolean tijdaup) {
  if (dodTelMes && !--dodTelMes) dodTelMes = 24000, client.print("@DM+");
  if (belTel) belTel--;
  if (delayWacht && !delayReady) if (delayTel++ >= 100) delayTel = 0, delayReady = true;
}

void zettijd () {
  uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
}

void tijddigit(boolean pnt) {
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

void zettemp () {
  uint8_t data[] = { digit[0], digit[1], digit[2], digit[3] }; display.setSegments(data);
}

void tempdigit (boolean celsio) {
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
