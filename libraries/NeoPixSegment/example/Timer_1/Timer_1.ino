#include <NeoPixSegment.h>

#define Pin 3 /* Communication pin */
#define LedsPerSegment 2 /* Leds in segment*/
#define Format "88:88" /* "88:88:88", "8888" etc. */
#define Debug 1 /* 0 = Debug in Serial monitor is disabled, 1 = Debug enabled */

NeoPixSegment Pix;

unsigned long myMillis = 10;
byte mm = 0;
byte ss = 0;
char myChar[6];

void setup() {
  Serial.begin(9600);

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

void loop() {
  if ((millis() - myMillis) >= 500) {
    myMillis = millis();
    mm = (myMillis / 1000) / 60;
    ss = (myMillis / 1000) % 60;

    sprintf(myChar, "%02i:%02i", mm, ss); // filling up char array, %02i sets the leading zero if the number is less than 10

    for (byte i = 0; i < 5; i++) {
      Serial.print("myChar"); Serial.print(i); Serial.print(" = "); Serial.println(myChar[i]);

      //feed color data to the variables
      if (i != 2) {
        byte a = myChar[i] - '0';
        byte r = color[a][0];
        byte g = color[a][1];
        byte b = color[a][2];
        Pix.SetColor(r, g, b, i);
      }
      Pix.WriteCharacter(myChar[i], i);

      // don't show zero if MM is less than 10
      if (myChar[0] == '0') {
        Pix.Clear(0);
      }
    }
    //Show numbers on the display
    Pix.WriteDotColon(colon = !colon, 2);
    Pix.Show();
    Serial.println("");
  }
}
