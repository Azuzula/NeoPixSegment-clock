/*
   Final clock code for NeoPixSegment WS2812 7 segment LED display
   with DS3231 real time module and TEMT6000 light sensor.
   Library GIT: https://github.com/Azuzula/NeoPixSegment
   Dependency: Adafruit NeoPixel https://github.com/adafruit/Adafruit_NeoPixel
   This code GIT: https://github.com/Azuzula/NeoPixSegment-clock
   web: https://www.vokolo.cz
   From example by Azuzula, 2020

   DS3231: Real-Time Clock.
   Read more: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds3231.html
   GIT: https://github.com/jarzebski/Arduino-DS3231
   Web: http://www.jarzebski.pl
   (c) 2014 by Korneliusz Jarzebski

   Streaming: Library for Serial print messages, wery usefull and simple to use.
   Source: http://arduiniana.org/libraries/streaming/

  Optional sensors:
  AM612 - PIR sensor.
*/

// Sorry for Czech and English mixed variable names and comments.
// Will fix it in future versions.
// Also I will fix some dirty code parts and some unecesary Serial print debug lines.

#include <NeoPixSegment.h>
#include <EEPROM.h>
#include <Wire.h>
#include <DS3231.h>
#include <Streaming.h>

//***** Datum dokončení, verze tohoto FW
#define DATE_n_VERSION "5.1.2022 v2.05"

//***** EEPROM address
#define FIRST_START 1
#define COLOR_INDEX 3
#define COLON 4
#define COLON_BLINK 5
#define SVIT 9
#define PIR 10
#define JAS_MOD 11
#define JAS_MIN 12
#define JAS_MAX 13
#define NCOLON 14
#define RED 20
#define GREEN 21
#define BLUE 22
#define NOC 23
#define NRED 24
#define NGREEN 25
#define NBLUE 26

//***** time
DS3231 clock;
RTCDateTime dt;

//***** time var
char myChar[5];
int myNum[4];

//***** PIR and sleep
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
#define sampl 25
//byte measureCount = 0;
byte r1 = 0;
byte Jas = 1;
byte JasTed;
byte mod = EEPROM.read(JAS_MOD);
byte minimum = EEPROM.read(JAS_MIN);
byte maximum = EEPROM.read(JAS_MAX);

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
//ResponsiveAnalogRead analog(temt6000Pin, false);
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
  Serial << F("Datum vytvoření FW a verze: ") << DATE_n_VERSION << endl;
  Serial << F("Datum a čas poslední kompilace kódu ") << __DATE__ << " " << __TIME__ << endl;
  Serial << F("Pro víc informací napiš příkaz help") << endl;
  Serial << F("*************") << endl << endl;
  delay(100);

  // Initialize clock module DS3231
  clock.begin();
  clock.forceConversion();
  Serial << F("Teplota:") << clock.readTemperature() << endl;
  if (EEPROM.read(FIRST_START) != 1) {
    Serial << F("První start, provádím reset") << endl;
    clock.setDateTime(__DATE__, __TIME__);
    RESET();
    EEPROM.update(FIRST_START, 1);
  }

  // Initialize display color
  Pix.Init(displayPin, LedsPerSegment, Format, Debug);

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
    if (!digitalRead(settPin)) {
      //setOn = 1;
      cSetup++;
    }
    else {
      if (cSetup > 0) {
        showTemp();
      }
//      setOn = 0;
      cSetup = 0;
    }

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

  //if (!digitalRead(settPin)) { // držet alespoň 2s pro vstup do nastavení času
  if (cSetup == 20) {
    Pix.SegmentBlink(0, 2);
    Pix.WriteDotColon(1, 2);
    while (!digitalRead(settPin)) { // zabrání tomu, aby se ihned po přepnutí měnily hodiny
      delay(1);
    } // do nastavení se přejde až po uvolnění tlačítka SET
    Settings();
  }

  // nastavení barvy čísel
  if (!digitalRead(plusPin)) {
    delay(debounce);
    colorIndex++;
    if (colorIndex > colorArray) {
      colorIndex = 1;
    }
    //Serial << F ("color index = ") << colorIndex << endl;
    setColor();
    colorSave = 1;
  }
  if (!digitalRead(minusPin)) {
    delay(debounce);
    colorIndex--;
    if (colorIndex == 0) {
      colorIndex = colorArray;
    }
    //Serial << F ("color index = ") << colorIndex << endl;
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
    if (demo > 20) {
      demo = 1;
      colorIndex ++;
      if (colorIndex == colorArray-1) {
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
    if ((cSleep == (svit * 10)) && (!Sleeping)) {
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
