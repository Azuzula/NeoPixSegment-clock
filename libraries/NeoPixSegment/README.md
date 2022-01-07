# NeoPixSegment display
==========
This is an [Arduino](http://arduino.cc) library for 7 segment LED display based on WS2812/Neopixel.

Dependency
----------
Our library depends on the [Adafruit_Neopixel](https://github.com/adafruit/Adafruit_NeoPixel) library.

Documentation
-------------
-Init			Initialize display NeoPixSegment.Init(Arduino Pin, format of the display, debug mode).
-SetColor		Sets color of each or only one segment. NeoPixSegment.SetColor(R, G, B, index).
-SetBlinkInterval	Sets interval for blinking. Time between 100ms and 5000ms. 500ms is default.
-SetBrightness		Sets brightness whole display. 0 to 255, 60 is default.
-WriteCharacter		Write one character to the display at index. NeoPixSegment.WriteCharacter('8', index). Char only.
-WriteDotColon		Show or hide colon NeoPixSegment.WriteDotColon(state on/off, index).
-WriteNumber		Write number to the display. NeoPixSegment.WriteNumber(12, index). Int and byte only.
-WriteText		Write char array to the display. NeoPixSegment.WriteText('12', index). Char only.
-Clear			Clear display or one segment. NeoPixSegment.Clear(index).
-Show			Send data to show on the display. NeoPixSegment.Show().
-SegmentBlink		Set blink with segment NeoPixSegment.SegmentBlink(blink on/off, index).
-ALL_SEGMENTS		Use instead of index, Usable with SetColor, Clear and SegmentBlink.

Download
--------
The lastest binary version of the Library is always available from the 
Azuzula GitHub repository [https://github.com/Azuzula/NeoPixSegment/](https://github.com/Azuzula/NeoPixSegment/)

Install
-------
Unzip the content to your Arduino Library folder or use Add .Zip library from menu Sketch.

Support
-------
Support us by buying this product at our e-shop [www.vokolo.cz/neopixsegment](https://www.vokolo.cz/neopixsegment/) for Czech and Slovakia customers only.
Or send your request to the email azuzula.cz@gmail.com and we can make a deal.