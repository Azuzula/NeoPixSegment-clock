#include <NeoPixSegment.h>

/* !!! USER DEFINITION !!! */
#define PIN 3 /* Communication pin */
#define LEDS_PER_SEGMENT 2 /* Leds in segment*/
#define DISPLAY_FORMAR "88:88"
#define Debug 1 /* 0 = Dubug in Serial monitor is disabled, 1 = Dubug in Serial monitor is enabled */

/* Create an object of MyClass (this will call the constructor) */
NeoPixSegment NP_Segment; 

/* Global variables*/
unsigned long LocTime = 0, Timer = 0, FunIndex = 0;
char myChar[6];


/*                !!! IMPORTANT !!!! 
 * SegmentIndex represents each number, dot and colon 
 * EXAMPLE:  
 * Display format is: 88:88
 * SegmentIndex:      12345 
 */


void setup() {
  /*  ---< Default settings after display initialization >---
   *  Color - Red
   *  Blinking - Blinking all segments is disabled
   *  Blinking Interval - 500ms
   */
  NP_Segment.Init(PIN, LEDS_PER_SEGMENT, DISPLAY_FORMAR, Debug); /* Display initialization*/

  LocTime = 1000000 + micros();
}

void loop() {

  // put your main code here, to run repeatedly:
  /* Second timer*/
  if(LocTime < micros()){
    LocTime = 1000000 + micros();
    Timer++;
    NP_Segment.Clear(ALL_SEGMENTS);
  }
  /* Displays actual function on display */
  if(Timer <= 1){
    NP_Segment.SegmentBlink(0, ALL_SEGMENTS);
    NP_Segment.SetColor(50, 0, 0, ALL_SEGMENTS);
    NP_Segment.SetBlinkInterval(500); /* Def. blink interval 500ms*/
    NP_Segment.SetBrightness(60);
    sprintf(myChar, "Fn-%1i",FunIndex);
    NP_Segment.WriteText(myChar,RIGHT_ALIGMENT);
  }
  else{
    /* Demo functions */
    switch (FunIndex) {
      case 0: /*Fn - 0*/
        /* Demonstrates left alignment when using the WriteText function */
        NP_Segment.WriteText("Hi",LEFT_ALIGMENT);
        break;
      case 1: /*Fn - 1*/
        /* Demonstrates right alignment when using the WriteText function */
        NP_Segment.WriteText("Hi",RIGHT_ALIGMENT);
        break; 
      case 2: /*Fn - 2*/
        /* Demonstrates the use of the WriteNumber function to control individual segments */
        NP_Segment.WriteNumber(1, 0);
        NP_Segment.WriteNumber(2, 1);
        NP_Segment.WriteNumber(3, 3);
        NP_Segment.WriteNumber(4, 4);
        break;
      case 3: /*Fn - 3*/
        /* Demonstrates the use of the WriteCharracter function to control individual segments */
        NP_Segment.WriteCharacter('1', 0);
        NP_Segment.WriteCharacter('2', 1);
        NP_Segment.WriteCharacter('3', 3);
        NP_Segment.WriteCharacter('4', 4);
        NP_Segment.WriteDotColon(1, 2);
        break;
      case 4: /*Fn - 4*/
        /* Demonstrates the WriteText function with display brightness settings */
        NP_Segment.SetBrightness(250);
        NP_Segment.WriteText("12:34",RIGHT_ALIGMENT);
        break;
      case 5: /*Fn - 5*/
        /* Demonstrates the WriteText function with the ability to blink any segment */
        NP_Segment.WriteText("12:34",RIGHT_ALIGMENT);
        NP_Segment.SegmentBlink(1, 2);
        break;  
      case 6: /*Fn - 6*/
        /* Demonstrates the WriteText function with the ability to blink all segments */
        NP_Segment.WriteText("12:34",RIGHT_ALIGMENT);
        NP_Segment.SegmentBlink(1, ALL_SEGMENTS);
        break;
      case 7: /*Fn - 7*/
        /* Demonstrates the WriteText function with the ability to blink all segments and set the blink rate */
        NP_Segment.WriteText("12:34",RIGHT_ALIGMENT);
        NP_Segment.SetBlinkInterval(100);
        NP_Segment.SegmentBlink(1, ALL_SEGMENTS);
        break;
      case 8: /*Fn - 8*/
        /* Demonstruje SetColor funkci a nastavení barvy segmentu */
        NP_Segment.WriteText("12:34",RIGHT_ALIGMENT);
        NP_Segment.SetColor(50, 0, 0, 0);
        NP_Segment.SetColor(0, 50, 0, 1);
        NP_Segment.SetColor(0, 0, 50, 2);
        NP_Segment.SetColor(50, 0, 50, 3);
        NP_Segment.SetColor(0, 50, 50, 4);
        break;               
    }
  }
  
  /* Change demo function */
  if(Timer >= 5){
    Timer = 0;
    FunIndex++;
    if(FunIndex >= 9)FunIndex = 0;
  }
  
  NP_Segment.Show();     
}
