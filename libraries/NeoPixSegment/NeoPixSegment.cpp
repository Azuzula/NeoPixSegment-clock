/*!
 * @file NeoPixSegment.cpp
 */

#include <stdlib.h>

#include "NeoPixSegment.h"
#include "SegmentsDefinition.h"
#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel Pixels = Adafruit_NeoPixel();


/*******************************************************************************
  @Function  Init
  @brief     Display initialization
  
  @param     ComPin - Communication pin with display
  @param     SegmentLeds - Number of LEDS in a segment
  @param     *DisplayFormats - String representation of a display 
  @param     DebugOn - 0 = Debug information off, 1 = Debug information on

*******************************************************************************/
void NeoPixSegment::Init(unsigned char ComPin, unsigned char SegmentLeds,const char *DisplayFormats, bool DebugOn) {
  
  unsigned char i,Leds;
  
  /* Debud init */
  if(DebugOn){
    Serial.begin(9600);
  }

  /* Display format detection */
  Leds = 0;
  for(i = 0; (i < MAX_DISPLAY_SEGMENTS) && (DisplayFormats[i] != '\0'); i++){
    switch(DisplayFormats[i]){
      case '8':
        DisplayDefinition[i] = DF_NUMBER;
        Leds += SegmentLeds * 7;
        break;
      case ':':
        DisplayDefinition[i] = DF_COLON;
        Leds += 2;
        break;
      case '.':
        DisplayDefinition[i] = DF_DOT;
        Leds += 1;
        break;
    }
  }
  DisplayDefinition[i] = DF_END;
  DisplaySegments = i;
  if(DisplayDefinition[DisplaySegments] != '\0'){
    DebugInfo("Incorrect display format");
  }
     
  //for(i = 0; i < DisplaySegments;i++){
    //Serial.println(DisplayDefinition[i]);
  //}

  
  /* NeoPixel init */
  Pixels.begin();
  Pixels.updateType(NEO_GRB + NEO_KHZ800);
  Pixels.updateLength(Leds);
  Pixels.setPin(ComPin);
  Pixels.show(); /* Write to display */
  

  /* NeoPixSegmentInit*/
  LedsPerSegment = SegmentLeds;
  Debug = DebugOn;
  SetBrightness(60);
  Clear(ALL_SEGMENTS); /* Clear display memory*/
  SetColor(50,0,0,ALL_SEGMENTS); /* Def. colour for all segments*/
  SegmentBlink(0,ALL_SEGMENTS); /* Disable segments blink */

  SetBlinkInterval(500); /* Def. blink interval 500ms*/
  Blink.LastBlinkTime = millis();

}
 
/*******************************************************************************
  @Function  SetBlinkInterval
  @brief     Blink interval of segments, if blinking of segments is enabled
  
  @param     BlinkInterval - Blink interval in ms

*******************************************************************************/ 
void NeoPixSegment::SetBlinkInterval(unsigned long BlinkInterval){

  if((BlinkInterval > 5000) || (BlinkInterval < 100)){
    DebugInfo("BlinkInterval is out of range 100 ... 5000");
    return;   
  }

  Blink.Interval = BlinkInterval;
}

/*******************************************************************************
  @Function  SetBrightness
  @brief     Function sets display brightness
  
  @param     Brightness - Value of brightness in interval 0 ... 255

*******************************************************************************/  
void NeoPixSegment::SetBrightness(unsigned char Brightness){
  Pixels.setBrightness(Brightness); 
}

/*******************************************************************************
  @Function  SegmentBlink
  @brief     Enabling or disabling segment blinking
  
  @param     Blink - 0 = segment blinking is disabled, 1 = segment blinking is enabled
  @param     SegmentIndex - 0 ... (DisplaySegments - 1) = Selected segment blink or 
                            ALL_SEGMENTS = All segments blink               

*******************************************************************************/  
void NeoPixSegment::SegmentBlink(bool Blink, unsigned char SegmentIndex){
  
  unsigned short i;
  
  if((SegmentIndex >= DisplaySegments) && (SegmentIndex !=  ALL_SEGMENTS)){
    DebugInfo("SegmentIndex is out of range");
    return;   
  }
  
  if(SegmentIndex == ALL_SEGMENTS){  /* Settings of all segments */
    for(i = 0; i < MAX_DISPLAY_SEGMENTS; i++){
      SegmentState[i].BlinkIsActive = Blink;
    }
  }
  else{ /* Settings of chosen segment */
    SegmentState[SegmentIndex].BlinkIsActive = Blink;
  }
}

/*******************************************************************************
  @Function  Clear
  @brief     Clear segment memory
  
  @param     SegmentIndex - 0 ... (DisplaySegments - 1) or  ALL_SEGMENTS = All segments blink        
*******************************************************************************/ 
void NeoPixSegment::Clear(unsigned char SegmentIndex) {

  unsigned short i;

  if((SegmentIndex >= DisplaySegments) && (SegmentIndex !=  ALL_SEGMENTS)){
    DebugInfo("SegmentIndex is out of range");
    return;   
  }

  if( SegmentIndex == ALL_SEGMENTS ){
    for(i = 0; i < MAX_DISPLAY_SEGMENTS; i++) {
      SegmentState[i].Value = 0;
    }
  }
  else{
    SegmentState[SegmentIndex].Value = 0;
  }
}

/*******************************************************************************
  @Function  WriteDotColon
  @brief     Turning on or turning off dot or colon
  
  @param     Shine - 0 = Dot or colon don't shine, 1 = Dot or colon shine
  @param     SegmentIndex - Index on dot or colon 0 ... (DisplaySegments - 1

*******************************************************************************/
void NeoPixSegment::WriteDotColon(bool Shine, unsigned char SegmentIndex){

  /* Check input parameters */
  if(SegmentIndex >= DisplaySegments){
    DebugInfo("SegmentIndex is out of range");
    return;   
  }
  
  if((DisplayDefinition[SegmentIndex] != DF_DOT) && (DisplayDefinition[SegmentIndex] != DF_COLON)) {
    DebugInfo("SegmentIndex is not set on Dot or Colon");
    return;  
  }
  
  SegmentState[SegmentIndex].Value = Shine;
}

/*******************************************************************************
  @Function  WriteCharacter
  @brief     Shows Ascii chracter in segment 
  
  @param     Character - Ascii character
  @param     SegmentIndex - Index where should be character showed 0 ... (DisplaySegments - 1)

*******************************************************************************/ 
void NeoPixSegment::WriteCharacter(char Character, unsigned char SegmentIndex) {

  /* Check input parameters */
  if(SegmentIndex > DisplaySegments) {
    DebugInfo("SegmentIndex is out of range");
    return;
  }

  if(DisplayDefinition[SegmentIndex] != DF_NUMBER) {
    DebugInfo("SegmentIndex is not set on number segment");
    return;
  }
 
  SegmentState[SegmentIndex].Value = GetChar(Character); /* Converts a character to segment bits*/
}

/*******************************************************************************
  @Function  WriteNumber
  @brief     Shows number in segment 
  
  @param     Number - Number 0 ... 9 ( Not Ascii )
  @param     SegmentIndex - Index where should be number showed 0 ... (DisplaySegments - 1)

*******************************************************************************/
void NeoPixSegment::WriteNumber(char Number, unsigned char SegmentIndex) {

  /* Check input parameters */
  if((Number < 0) ||  (Number > 9)) {
    DebugInfo("The number must be in the range 0 - 9");
    return;
  }

  SegmentState[SegmentIndex].Value = GetChar(Number + 0x30); /* Converts a character to segment bits*/
}

/*******************************************************************************
  @Function  WriteText
  @brief     Writes string on display
  
  @param     *Text - Text to show
  @param     Right - LEFT_ALIGMENT = Left aligment, RIGHT_ALIGMENT = Right aligment

*******************************************************************************/
void NeoPixSegment::WriteText(const char *Text, bool Right){

  unsigned char i,j,k,TextLength,LocalDefinition[DisplaySegments],Index,MyCharacter;
  
  /* Check Text length */
  for(i = 0 ; i < DisplaySegments; i++){
    if(Text[i] == '\0')break;
  }
  TextLength = i;
  
  if(Text[TextLength] != '\0'){ /* Debug info*/
    DebugInfo("Text is too long");
  }


  /* Check if we can the text show */
  for(j = 0; j <=  (DisplaySegments - TextLength);j++){
    memset(LocalDefinition, DF_EMPTY, sizeof(LocalDefinition));
    if(Right == true){
      Index = (DisplaySegments - TextLength) - j;
    }
    else{
      Index = j;
    }
    for(i = 0 ;i < TextLength; i++){
      switch(Text[i]){
        case '.':
          MyCharacter = DF_DOT;
          break;
        case ':':
          MyCharacter = DF_COLON;
          break;
        default:  /* If it is not a dot or a colon, it must be a number...*/
          MyCharacter = DF_NUMBER;
          break; 
      }
      if(DisplayDefinition[Index] == MyCharacter){
        LocalDefinition[Index] = MyCharacter;
      }
      else{
        if(MyCharacter == DF_NUMBER){ 
          for(k = Index; k < DisplaySegments;k++){
            if(DisplayDefinition[k] == DF_NUMBER){
              LocalDefinition[k] = DF_NUMBER;
              Index = k; 
              break;
            }
          }        
        }
      }
      if(DisplayDefinition[Index] == MyCharacter){
        Index++;
      }
      else{
        break; /*The text cannot be displayed yet*/ 
      }
    }
    if(i == TextLength)  { /* I can display the text*/
      Index = 0;
      for(i = 0 ;i < DisplaySegments; i++){
        switch(LocalDefinition[i]){
          case DF_DOT:
            WriteDotColon(1,i);
            Index++;
            break;
          case DF_COLON:
            WriteDotColon(1,i);
            Index++;
            break;
          case DF_NUMBER:  /*If it is not a dot or a colon, it must be a number...*/
            SegmentState[i].Value = GetChar(Text[Index]);
            Index++;
            break;
          default:
            SegmentState[i].Value = 0;
            break;  
        }  
      }
      break;
    }
  }
}

/*******************************************************************************
  @Function  Show
  @brief     Makes shows on display - It must be called at least once in loop !!! 

*******************************************************************************/
void NeoPixSegment::Show(void){
  
  unsigned short i,j,k,PixelIndex;
  unsigned long LocBlinkTime;
  bool SegmentShine;
  
  /* Segment Blink timer*/
  LocBlinkTime = millis();
  if(LocBlinkTime >= (Blink.LastBlinkTime + (Blink.Interval))){
     Blink.LastBlinkTime = LocBlinkTime - (LocBlinkTime % 10);
     Blink.State = !Blink.State;
  }
  
  PixelIndex = 0;
  for(i = 0 ; (i < DisplaySegments); i++){
    if(SegmentState[i].BlinkIsActive) SegmentShine = Blink.State;
    else SegmentShine = true;
    
    switch(DisplayDefinition[i]){
      case DF_NUMBER:
        for(k = 0; k < 7 ; k++){
          if((SegmentState[i].Value & (0x80 >> k)) && SegmentShine){
            for(j = 0; j < LedsPerSegment; j++){
              Pixels.setPixelColor(PixelIndex + j, Pixels.Color(SegmentState[i].R, SegmentState[i].G, SegmentState[i].B));
            }
          }
          else{
            for(j = 0; j < LedsPerSegment; j++){
              Pixels.setPixelColor(PixelIndex + j, Pixels.Color(0, 0, 0));
            }
          }
          PixelIndex += LedsPerSegment;
        }
        break;    
      case DF_DOT:
        if(SegmentState[i].Value && SegmentShine){
          Pixels.setPixelColor(PixelIndex, Pixels.Color(SegmentState[i].R, SegmentState[i].G, SegmentState[i].B));
        }
        else{
          Pixels.setPixelColor(PixelIndex, Pixels.Color(0, 0, 0));
        }
        PixelIndex += 1;
        break;
      case DF_COLON:
        if(SegmentState[i].Value && SegmentShine){
          for(j = 0; j < 2; j++){
            Pixels.setPixelColor(PixelIndex + j, Pixels.Color(SegmentState[i].R, SegmentState[i].G, SegmentState[i].B));
          }
        }
        else{
          for(j = 0; j < 2; j++){
            Pixels.setPixelColor(PixelIndex + j, Pixels.Color(0, 0, 0));
          }
        }
        PixelIndex += 2;
        break;
    }
  }
  Pixels.show();
}

/*******************************************************************************
  @Function  SetColor
  @brief     Makes shows on display - It must be called at least once in loop !!!

  @param    R - Red color 0 ... 255
  @param    G - Green color 0 ... 255
  @param    B - Blue color 0 ... 255
  @param     SegmentIndex - 0 ... (DisplaySegments - 1) = Selected segment blink or 
                            ALL_SEGMENTS = All segments blink    

*******************************************************************************/ 
void NeoPixSegment::SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char SegmentIndex) {

  unsigned char i;
  
    
  if((SegmentIndex >= DisplaySegments) && (SegmentIndex !=  ALL_SEGMENTS)){
    DebugInfo("SegmentIndex is out of range");
    return;   
  }

  if(SegmentIndex == ALL_SEGMENTS) { // Set the same colour for all segments
    for(i = 0; i < DisplaySegments; i++) {
      SegmentState[i].R = R;
      SegmentState[i].G = G;
      SegmentState[i].B = B;
    }
  }
  else {
    SegmentState[SegmentIndex].R = R;
    SegmentState[SegmentIndex].G = G;
    SegmentState[SegmentIndex].B = B;
  }
}

/*******************************************************************************
 *                   ---< Private functions >---
 ******************************************************************************/

/*******************************************************************************
  @Function  DebugInfo
  @brief     Shows debug information in Serial monitor
  
  @param     *DebugText - Text to show

*******************************************************************************/
void NeoPixSegment::DebugInfo(const char *DebugText){
  
  if(Debug){
    Serial.println(DebugText);
  }
}

/*******************************************************************************
  @Function  GetChar
  @brief     Convert Ascii Character to segment data 
  
  @param     Character - Ascii character
  @return    Character converted to segment data

*******************************************************************************/
unsigned char NeoPixSegment::GetChar(char Character) {

  unsigned short i;

  for(i = 0; CharTable[i].AsciiIdx != 0xFF; i++) {
    if( CharTable[i].AsciiIdx == Character )break;
  }
  
  if(CharTable[i].AsciiIdx == 0xFF){
    DebugInfo("Unknow character, please define character in SegmentsDefinition.h");
    return(0xFF);
  }

  return(CharTable[i].CharData);
}
