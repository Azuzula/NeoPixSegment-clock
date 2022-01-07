#include <NeoPixSegment.h>

/* !!! USER DEFINITION !!! */
#define PIN 3 /* Communication pin */
#define LEDS_PER_SEGMENT 2 /* Leds in segment*/
#define DISPLAY_FORMAT "88:88"
#define Debug 1 /* 0 = Debug in Serial monitor is disabled, 1 = Dubug in Serial monitor is enabled */

/* Create an object of MyClass (this will call the constructor) */
NeoPixSegment NP_Segment; 

/* Global variables*/
unsigned long time_start_ms;
unsigned long time_now_s;


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
  NP_Segment.Init(PIN, LEDS_PER_SEGMENT, DISPLAY_FORMAT, Debug); /* Display initialization*/

  /*Example_1 - For use uncomment this example and comments other Example_x*/
  NP_Segment.SegmentBlink(1,2);

  /*Example_2 - For use uncomment this example and comments other Example_x*/
//  NP_Segment.SegmentBlink(1,2);
//  NP_Segment.SetColor(0,255,0,ALL_SEGMENTS);

  /*Example_2 - For use uncomment this example and comments other Example_x*/
//  NP_Segment.SegmentBlink(1,ALL_SEGMENTS);
//  NP_Segment.SetColor(0,0,255,ALL_SEGMENTS);
//  NP_Segment.SetBlinkInterval(1000);
}

void loop() {
  time_now_s = (millis() - time_start_ms) / 1000;
  char time_string[] = {'0', '0', ':', '0', '0', '\0'};
  time_string[0] = time_now_s / 60 / 10 + '0';
  time_string[1] = time_now_s / 60 % 10 + '0';
  time_string[3] = time_now_s % 60 / 10 + '0';
  time_string[4] = time_now_s % 60 % 10 + '0';

  NP_Segment.WriteText(time_string,RIGHT_ALIGMENT);
  NP_Segment.Show();
  
}
