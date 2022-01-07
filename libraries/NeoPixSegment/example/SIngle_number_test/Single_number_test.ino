#include <NeoPixSegment.h>

#define Pin 3 /* Communication pin */
#define LedsPerSegment 2 /* Leds in segment*/
#define Format "8" /* "88:88:88", "8888" etc. */
#define Debug 0 /* 0 = Debug in Serial monitor is disabled, 1 = Debug enabled */

NeoPixSegment Pix;

unsigned long myMillis;

void setup() {
  Pix.Init(Pin, LedsPerSegment, Format, Debug);
  Pix.SetColor(255, 0, 0, ALL_SEGMENTS); // sets display color (R, G, B, index)
  Pix.SetBrightness(10); // sets brightness for entire display, default is 60
}
bool colon = true;
// some colors to show off
const uint8_t color[10][3] {
  {255, 0, 0}, //0
  {255, 80, 0}, //1
  {255, 255, 0}, //2
  {0, 255, 0}, //3
  {0, 255, 80}, //4
  {0, 255, 255}, //5
  {0, 0, 255}, //6
  {80, 0, 255}, //7
  {255, 0, 255}, //8
  {255, 255, 255}, //9
};

byte a = 0;
void loop() {
  if ((millis() - myMillis) >= 1000) {
    myMillis = millis();
    Pix.WriteNumber(a, 0); //WriteNumber(number, index)
    //set colors
    byte r = color[a][0];
    byte g = color[a][1];
    byte b = color[a][2];
    Pix.SetColor(r, g, b, 0);
    a++;
    if (a == 10) {
      a = 0;
    }
  }
  //Show numbers on the display
  Pix.Show();
}