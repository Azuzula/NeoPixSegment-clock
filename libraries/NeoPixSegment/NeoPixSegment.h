#ifndef NEO_PIX_SEGMENT_H
#define NEO_PIX_SEGMENT_H

/***********************************************************************************/
/*                                ---< Project definition  >---                    */
/***********************************************************************************/
#define MAX_DISPLAY_SEGMENTS  10
#define ALL_SEGMENTS          0xFF

#define DF_END    0
#define DF_NUMBER 1
#define DF_DOT    2
#define DF_COLON  3
#define DF_EMPTY  4

#define LEFT_ALIGMENT   0
#define RIGHT_ALIGMENT  1

/***********************************************************************************/
/*                                     ---< Class  >---                            */
/***********************************************************************************/

class NeoPixSegment {
public:
    unsigned long width;
    unsigned long height;

    void Init(unsigned char ComPin, unsigned char LesdPerSegment,const char *DisplayFormat,bool DebugOn);
    void Clear(unsigned char SegmentIndex);
    void WriteCharacter(char Character, unsigned char SegmentIndex);
    void WriteNumber(char Number, unsigned char SegmentIndex);
    void SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char SegmentIndex);
    void Show(void);
    void WriteDotColon(bool Shine, unsigned char SegmentIndex);
    void SegmentBlink(bool Blink, unsigned char SegmentIndex);
    void SetBrightness(unsigned char Brightness);
    void SetBlinkInterval(unsigned long BlinkInterval);
    void WriteText(const char *Text, bool Right);
         
private:
  unsigned char DisplayDefinition[MAX_DISPLAY_SEGMENTS];
  unsigned char LedsPerSegment;
  unsigned char DisplaySegments;
  bool Debug;
  
  void DebugInfo(const char *DebugText);
  unsigned char GetChar(char Character);
  
  struct SegmentStateStruct{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char Value;
    bool          BlinkIsActive;
  }SegmentState[MAX_DISPLAY_SEGMENTS];

  struct BlinkStruct{
    unsigned long LastBlinkTime;
    unsigned long Interval;
    bool State;
  }Blink;
};
 
#endif // NEO_PIX_SEGMENT_H
