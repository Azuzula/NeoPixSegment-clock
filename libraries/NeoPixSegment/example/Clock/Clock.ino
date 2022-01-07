/*
 * Clock example for NeoPixSegment WS2812 7 segment LED display 
 * with DS3231 real time module
 * GIT: https://github.com/Azuzula/NeoPixSegment
 * web: https://www.vokolo.cz/neopixsegment
 * Example by Azuzula, 2020
 * Depends on Adafruit Neopixel https://github.com/adafruit/Adafruit_NeoPixel
 * 
 * DS3231: Real-Time Clock. Simple example
 * Read more: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds3231.html
 * GIT: https://github.com/jarzebski/Arduino-DS3231
 * Web: http://www.jarzebski.pl
 * (c) 2014 by Korneliusz Jarzebski
 *
*/

#include <NeoPixSegment.h>
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;

#define Pin 3 /* Communication pin */
#define LedsPerSegment 2 /* Leds in segment*/
#define Format "88:88" /* "88:88:88", "8888" etc. */
#define Debug 0 /* 0 = Debug in Serial monitor is disabled, 1 = Debug in Serial monitor is enabled */
bool colon = true;
unsigned long myMillis;
char myChar[5];

NeoPixSegment Pix;

void setup()
{
  // Initialize display
  Pix.Init(Pin, LedsPerSegment, Format, Debug);
  Pix.SetColor(100, 0, 0, ALL_SEGMENTS); // sets display color (R, G, B, index)
  Pix.SetBrightness(10); // sets brightness for entire display, default is 60

  // Initialize DS3231
  clock.begin();

  // Set sketch compiling time
  //clock.setDateTime(__DATE__, __TIME__); // coment after first upload
}

void loop()
{
  dt = clock.getDateTime();

  sprintf(myChar, "%02i:%02i", dt.hour, dt.minute); // filling up char array, %02i sets the leading zero if the number is less than 10

  for (byte i = 0; i < 5; i++) {
      Pix.WriteCharacter(myChar[i], i);
      
      // don't show zero if MM is less than 10
      if (myChar[0] == '0') {
        Pix.Clear(0);
      }
    }
    //Send data to the display
    Pix.WriteDotColon(colon = !colon, 2);
    Pix.Show();
  delay(500);
}
