#ifndef apa102LEDStrip_h
#define apa102LEDStrip_h
#include "Arduino.h"

class apa102LEDStrip
{
  public:
    apa102LEDStrip();
    void init(unsigned short int numLEDs, byte bytesPerLED, byte globalBrightness);
    void setPixel(short int pixelIndex, byte *pixelColour);
    void getPixel(short int pixelIndex, byte *pixelColour);
    void setGlobalBrightness(byte globalBrightness);

    byte* LEDs;
    
    byte _globalBrightness;
    short int _frameLength;
    short int _endFrameLength;
    short int _numLEDs;
    byte _bytesPerLED;
    short int _counter;
    
  private:
    unsigned short int _index;
};

#endif
