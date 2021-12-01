/*
   Final clock code for NeoPixSegment WS2812 7 segment LED display
   with DS3231 real time module and TEMT6000 light sensor.
   GIT: https://github.com/Azuzula/NeoPixSegment
   web: https://www.vokolo.cz
   From example by Azuzula, 2020

   DS3231: Real-Time Clock. Simple example
   Read more: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds3231.html
   GIT: https://github.com/jarzebski/Arduino-DS3231
   Web: http://www.jarzebski.pl
   (c) 2014 by Korneliusz Jarzebski

   Streaming: Library for Serial print messages, wery usefull and simple to use.
   Source: http://arduiniana.org/libraries/streaming/

  Optional sensors:
  AM612 - PIR sensor.
  SHT31 - Temperature and humidity sensor - Not implemented yet.
  2x capacitive sensors - Not implemented yet.
*/

// Sorry for Czech and English mixed variable names and comments.
// Will fix it in future versions.
// Also I will fix some dirty code parts and some unecesary Serial print debug lines.

#include <NeoPixSegment.h>
#include <EEPROM.h>
#include <Wire.h>
#include <DS3231.h>
#include <Streaming.h>
#include <ResponsiveAnalogRead.h>

//***** Datum dokončení, verze tohoto FW
#define DATUMaVERZE "13.10.2021 v2.03"

//***** EEPROM adresy
#define COLOR_INDEX 3
#define COLON 4
#define COLON_BLINK 5
#define SVIT 9
#define PIR 10
#define JAS_OFSET 11
#define JAS_MIN 12
#define NCOLON 13
#define RED 20
#define GREEN 21
#define BLUE 22
#define NOC 23
#define NRED 24
#define NGREEN 25
#define NBLUE 26

//***** hodiny
DS3231 clock;
RTCDateTime dt;

//***** uložený čas
char myChar[5];
int myNum[4];

//***** PIR a spánek
#define pirPin 2
bool pirActive = EEPROM.read(PIR);
byte toSleep = 0;
bool Sleeping = 0;
byte svit = EEPROM.read(SVIT);

//***** navigační tlačítko
#define plusPin 5
#define settPin 6
#define minusPin 7

//***** senzor osvětlení a jas
#define temt6000Pin A0
//#define vzorky 60
//byte reading[vzorky];
byte measureCount = 0;
byte r1 = 0;
byte Jas = 1;
byte JasTed;
float mod = (float)EEPROM.read(JAS_OFSET) / 10;
byte minimum = EEPROM.read(JAS_MIN);

//***** NeoPix displej
#define displayPin 4 /* Communication pin */
#define LedsPerSegment 2 /* Leds in segment*/
#define Format "88:88" /* "88:88:88", "8888" etc. */
// Debug for NeoPix library only. Will fix it later or delete all debug lines in library.
#define Debug 0 /* 0 = debug in Serial monitor is disabled, 1 = Debug in Serial monitor is enabled */
bool colon = EEPROM.read(COLON);
byte cColon = 0;
bool swi = true;
byte dtBlik = EEPROM.read(COLON_BLINK);

NeoPixSegment Pix;
ResponsiveAnalogRead analog(temt6000Pin, false);
//***** debouncing delay for buttons
#define debounce 500

//***** barva číslic
#define colorArray 13 // je počet definovaných barev pro přepínání (+ 1 je samostatná pro noční režim)
byte myColor[colorArray + 1][3] = {{255, 0, 0}, {255, 40, 0}, {255, 200, 0}, {125, 255, 0}, {0, 255, 0}, {0, 255, 50}, {0, 255, 200}, {0, 125, 255}, {0, 0, 255}, {80, 0, 255}, {255, 0, 255}, {255, 0, 80}, {EEPROM.read(RED), EEPROM.read(GREEN), EEPROM.read(BLUE)}, {EEPROM.read(NRED), EEPROM.read(NGREEN), EEPROM.read(NBLUE)}};
byte colorIndex = EEPROM.read(COLOR_INDEX);
byte colorSave = 0;

//***** nápověda a příkazy v konzoli
//String inputString[4];
bool stringComplete = false;
//String prikaz = "";
//String hodnota = "";
byte demo = 0;
byte test = 0;

void setup()
{
  Serial.begin(9600);
  Serial << F("*************") << endl;
  Serial << F("NeoPixSegment hodiny") << endl;
  Serial << F("Vyrobila Zuzana Nyiri") << endl;
  Serial << F("E-shop: www.vokolo.cz") << endl;
  Serial << F("Twitter: @AzuzulaCZ") << endl;
  Serial << F("Github: https://github.com/Azuzula/NeoPixSegment") << endl;
  Serial << F("Thingiverse: https://www.thingiverse.com/thing:4780832") << endl;
  Serial << F("*************") << endl;
  Serial << F("HW použitý ke stavbě hodin:") << endl;
  Serial << F("Zobrazovací displej číslice NeoPixSegment osazené ledkami WS2812b") << endl;
  Serial << F("Arduino nano v3.0 ATmega328p Nový bootloader") << endl;
  Serial << F("Modul hodin DS3231") << endl;
  Serial << F("Senzor osvětlení TEMT6000") << endl;
  //Serial << F("Volitelné senzory:") << endl;
  //Serial << F("Senzor pohybu PIR AM612") << endl;
  //Serial << F("Senzor teploty SHT31") << endl;
  //Serial << F("2x kapacitní snímač MTCH101T") << endl;
  Serial << F("*************") << endl;
  Serial << F("Datum vytvoření FW a verze: ") << DATUMaVERZE << endl;
  Serial << F("Datum a čas kompilace kódu (reálná doba dokončení těchto hodin) ") << __DATE__ << " " << __TIME__ << endl;
  Serial << F("Pro víc informací napiš příkaz help") << endl;
  Serial << F("*************") << endl << endl;
  delay(100);

  // Initialize display color
  Pix.Init(displayPin, LedsPerSegment, Format, Debug);

  // Initialize clock module DS3231
  clock.begin();

  // Set sketch compiling time
  //clock.setDateTime(__DATE__, __TIME__); // coment after first upload
  pinMode(pirPin, INPUT);
  pinMode(temt6000Pin, INPUT);
  pinMode(plusPin, INPUT_PULLUP);
  pinMode(settPin, INPUT_PULLUP);
  pinMode(minusPin, INPUT_PULLUP);

  Jas = Jas_read();
   
  if (Jas <= 2 && (bool)EEPROM.read(NOC)) {
    Pix.SetColor(myColor[13][0], myColor[13][1], myColor[13][2], ALL_SEGMENTS);
    Pix.WriteDotColon(!(bool)EEPROM.read(NCOLON), 2);
    colon = EEPROM.read(NCOLON);
  }
  else {
    Pix.SetColor((myColor[colorIndex - 1][0]), (myColor[colorIndex - 1][1]), (myColor[colorIndex - 1][2]), ALL_SEGMENTS); // sets display color (R, G, B, index)
    Pix.WriteDotColon(!(bool)EEPROM.read(COLON), 2);
    colon = EEPROM.read(COLON);
  }
  WakeUp();
}

unsigned long clockMillis = 0;
byte cSetup = 0;
byte cSleep = 0;
byte cBrightness = 0;

void loop() {
  //******** counter
  if ((millis() - clockMillis) > 100) {
    clockMillis = millis();
    cSetup++;
    cBrightness++;

    if (pirActive) {
      cSleep++;
    }

    if (colon) {
      cColon++;
      if (cColon >= dtBlik) {
        cColon = 0;
        Pix.WriteDotColon(swi = !swi, 2);
      }
    }
    else {
      Pix.WriteDotColon(1, 2);
    }

    Jas = Jas_read();
    if (Jas <= 2) {
      if ((bool)EEPROM.read(NOC)) {
        Pix.SetColor(myColor[13][0], myColor[13][1], myColor[13][2], ALL_SEGMENTS);
      }
      colon = (bool)EEPROM.read(NCOLON);
    }
    else {
      colon = (bool)EEPROM.read(COLON);
      setColor();
    }

    if (colorSave > 0) {
      colorSave++;
      if (colorSave >= 100) {
        EEPROM.update(3, colorIndex);
        colorSave = 0;
        Serial << F("Nastavení výběru barvy uloženo.") << endl;
      }
    }

    if (demo > 0) {
      demo++;
    }
  }
  //******** end counter

  if (!digitalRead(settPin)) { // držet alespoň 2s pro vstup do nastavení času
    if (cSetup == 20) {
      Pix.SegmentBlink(0, 2);
      Pix.WriteDotColon(1, 2);
      while (!digitalRead(settPin)) { // zabrání tomu, aby se ihned po přepnutí měnily hodiny
        delay(1);
      } // do nastavení se přejde až po uvolnění tlačítka SET
      Settings();
    }
  }
  else {
    cSetup = 0;
  }

  // nastavení barvy čísel
  if (!digitalRead(plusPin)) {
    delay(debounce);
    colorIndex++;
    if (colorIndex > colorArray) {
      colorIndex = 1;
    }
    Serial.print("color index = "); Serial.println(colorIndex);
    setColor();
    colorSave = 1;
  }
  if (!digitalRead(minusPin)) {
    delay(debounce);
    colorIndex--;
    if (colorIndex == 0) {
      colorIndex = colorArray;
    }
    Serial.print("color index = "); Serial.println(colorIndex);
    setColor();
    colorSave = 1;
  }

  dt = clock.getDateTime();
  myNum[0] = dt.hour / 10;
  myNum[1] = dt.hour % 10;
  myNum[3] = dt.minute / 10;
  myNum[4] = dt.minute % 10;
  ParseTime();

  if (demo > 0) {
    if (test) {
      Pix.WriteText("88:88", LEFT_ALIGMENT);
    }
    if (demo > 10) {
      demo = 1;
      colorIndex ++;
      if (colorIndex == colorArray) {
        colorIndex = 0;
      }
      setColor();
    }
  }

  if (pirActive) {
    if (digitalRead(pirPin)) {
      //Serial.println("Já nespím!");
      cSleep = 0;
      Sleeping = 0;
    }
    if ((cSleep == (svit * 10))&&(!Sleeping)) {
      //Serial.println("ZZZzzzZZZzzz...");
      Sleeping = 1;
      SleepNow();
    }
  }

  //Send data to the display
  if (!Sleeping) {
    Pix.Show();
    Pix.SetBrightness(Jas);
  }
}

void SleepNow() {
  byte JasStep = Jas / 10;
  //Serial.print("Jas_arr = "); Serial.println(JAS);
  //Serial.print("step = "); Serial.println(JasStep);
  JasTed = Jas;

  for (byte i = 0; i < 10; i++) {
    JasTed = JasTed - JasStep;
    //Serial.print("JasTed = "); Serial.println(JasTed);
    Pix.SetBrightness(JasTed);
    Pix.Show();
    delay(40);
  }
  Pix.SetBrightness(0);
  Pix.Show();
}

void WakeUp() {
  //Serial.println("Vstávám");
  byte JasStep = Jas / 10;
  JasTed = 0;

  for (byte i = 0; i < 10; i++) {
    JasTed = JasTed + JasStep;
    //Serial.print("JasTed = "); Serial.println(JasTed);
    Pix.SetBrightness(JasTed);
    Pix.Show();
    delay(40);
  }
}

byte Jas_read() {
  analog.update();
  //Serial << "raw data = " << analog.getRawValue() << endl;
  //Serial << "smooth data = " << analog.getValue() << endl;
  int x = analog.getValue();
  measureCount++;
  if ((analog.hasChanged()) && ( measureCount >= 20)) {
    measureCount = 0;
    x = (x * mod);
    if (x <= minimum) {
      x = minimum;
    }
    else if (x >= 255) {
      x = 255;
    }
    return x;
  }
  else {
    return Jas;
  }
}

void Settings() {
  byte setNum = 0;
  ParseTime();
  Pix.SetBlinkInterval(300);
  Pix.SegmentBlink(1, 0);

  while (setNum < 5) {
    if (!digitalRead(settPin)) {
      delay(debounce);
      Pix.SegmentBlink(0, ALL_SEGMENTS);
      setNum++;
      if (setNum == 2) {
        setNum = 3;
      }
      Pix.SegmentBlink(1, setNum);
    }

    if (!digitalRead(plusPin)) {
      //Serial.println("+ btn");
      delay(debounce);
      myNum[setNum]++;

      if (setNum == 0) { // desítky hodin 0-2
        if (myNum[setNum] > 2) {
          myNum[setNum] = 0;
        }
      }
      else if (setNum == 1) { // jednotky hodin
        if (myNum[0] < 2) { // hodiny  < 20
          if (myNum[setNum] > 9) {
            myNum[setNum] = 0;
          }
        }
        else if (myNum[0] == 2) { // hodiny > 20
          if (myNum[setNum] > 3) {
            myNum[setNum] = 0;
          }
        }
      }
      else if (setNum == 3) { // desítky minut 0-6
        if (myNum[setNum] > 5) {
          myNum[setNum] = 0;
        }
      }
      else if (setNum == 4) { // jednotky minut 0-9
        if (myNum[setNum] > 9) {
          myNum[setNum] = 0;
        }
      }
      //Serial << "+číslo = " << myNum[setNum] << endl;
    }

    if (!digitalRead(minusPin)) {
      //Serial.println("- btn");
      delay(debounce);
      myNum[setNum]--;
      if (setNum == 0) { // desítky hodin 0-2
        if (myNum[setNum] < 0) {
          myNum[setNum] = 2;
        }
      }
      else if (setNum == 1) { // jednotky hodin
        if ((myNum[0] < 2)) { // hodiny  < 20
          if (myNum[setNum] < 0) {
            myNum[setNum] = 9;
          }
        }
        else if (myNum[0] == 2) { // hodiny > 20
          if (myNum[setNum] < 0) {
            myNum[setNum] = 3;
          }
        }
      }
      else if (setNum == 3) { // desítky minut 0-6
        if (myNum[setNum] < 0) {
          myNum[setNum] = 5;
        }
      }
      else if (setNum == 4) { // jednotky minut 0-9
        if (myNum[setNum] < 0) {
          myNum[setNum] = 9;
        }
      }
      //Serial << "-číslo = " << myNum[setNum] << endl;
    }
    ParseTime();
    Pix.Show();
  }
  byte setHour = (myNum[0] * 10) + myNum[1];
  byte setMinute = (myNum[3] * 10) + myNum[4];

  // Manual (YYYY, MM, DD, HH, II, SS
  //Serial << "Čas uložen: " << setHour << ":" << setMinute << endl;
  clock.setDateTime(dt.year, dt.month, dt.day, setHour, setMinute, 0);
  Pix.SegmentBlink(0, ALL_SEGMENTS);
  Pix.Show();
}

void ParseTime() {
  Pix.WriteNumber(myNum[0], 0);
  Pix.WriteNumber(myNum[1], 1);
  Pix.WriteNumber(myNum[3], 3);
  Pix.WriteNumber(myNum[4], 4);
  if (myNum[0] == 0 & cSetup < 20) { // pokud je míň hodin než 10, první číslo zhasne
    Pix.Clear(0);
  }
}

void setColor () {
  byte R = (myColor[colorIndex - 1][0]);
  byte G = (myColor[colorIndex - 1][1]);
  byte B = (myColor[colorIndex - 1][2]);
  Pix.SetColor(R, G, B, ALL_SEGMENTS);

  //Serial << "Barva = " << R << ", " << G << ", " << B << " " << endl;
}
