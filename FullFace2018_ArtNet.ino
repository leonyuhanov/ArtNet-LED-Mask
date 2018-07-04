/*
      //Pin out for module
      D5  CLK   -   Red
      D7  DATA  -   Black
*/
#include "SPI.h"
#include "apa102LEDStrip.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "artNetPacket.h"

const char * ssid = "SSID";
const char * password = "PASSWORD";
unsigned int artNetPort = 6454;
const short int maxPacketBufferSize = 530;
char packetBuffer[maxPacketBufferSize];
WiFiUDP udp;
short int packetSize=0;
artNetPacket dmxData;
const byte numberOfDMXUniverses = 3;
const byte universeRange[2] = {0,2};
const unsigned short int artNetFrames[numberOfDMXUniverses][2] = {{1,170},{171,170},{341,38}};
byte frameChecks[numberOfDMXUniverses][2]= {{0,0},{1,0},{2,0}};

const short int numLeds = 378+1;
const byte bytesPerLed = 4;
apa102LEDStrip leds;
byte tempColour[3];

unsigned short int pxIndex=0, dmxIndex=0, frameCntBreak=0, frameTicks=0, frameTCnt=0,innerFTCnt=0;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.hostname("LEDMASK");
  while (WiFi.status() != WL_CONNECTED)
  {
        delay(100);
        Serial.print(".");
  }
  Serial.print("\r\nONLINE\t");
  Serial.print(WiFi.localIP());
  udp.begin(artNetPort);
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setFrequency(1000000);
  leds.init(numLeds, bytesPerLed, 255);
  renderLEDs();
}


void renderLEDs()
{
  SPI.writeBytes(leds.LEDs, leds._frameLength);
}

void loop()
{   
  pollDMX();
  if(dmxData.hasChanged)
  {
    dmxData.hasChanged=0;
    artNetToSPI(dmxData.universe[0]);
  }
  yield();

}

void pollDMX()
{
     packetSize = udp.parsePacket();
     if(packetSize==maxPacketBufferSize)
     {
        udp.read(packetBuffer, maxPacketBufferSize);
        if(packetBuffer[14]>=universeRange[0] && packetBuffer[14]<=universeRange[1])
        {
          dmxData.parseArtNetPacket(packetBuffer);
        }
     }
     udp.flush();
}

void artNetToSPI(byte panelID)
{
  dmxIndex = 0;
  for(pxIndex = artNetFrames[panelID][0]; pxIndex<artNetFrames[panelID][0]+artNetFrames[panelID][1]; pxIndex++)
  {
      tempColour[0] = dmxData.data[dmxIndex];
      tempColour[1] = dmxData.data[dmxIndex+1];
      tempColour[2] = dmxData.data[dmxIndex+2];
      leds.setPixel(pxIndex, tempColour);
      dmxIndex+=3;
  }
  if(checkFrame(panelID))
  {
    renderLEDs();
  }
}

byte checkFrame(byte dmxFrameID)
{
  frameTicks = 0;
  
  //If all frames are OLD begin full check
  if(frameCntBreak==0)
  {
    frameCntBreak=1;
  }
  
  //Go through all frames and see which ones data has arrive
  for(frameTCnt=0; frameTCnt<numberOfDMXUniverses; frameTCnt++)
  {
    if( frameChecks[frameTCnt][0] == dmxFrameID && frameChecks[frameTCnt][1]==0)
    {
      //Current frame was old and is now NEW
      frameChecks[frameTCnt][1] = 1;
      break;
    }
    else if(frameChecks[frameTCnt][0] == dmxFrameID && frameChecks[frameTCnt][1]==1)
    {
      //Current frame was already writen to in this round
      //Frames are out of sync reject render this time round
      frameCntBreak=0;
      //clear all frame checks
      for(innerFTCnt=0; innerFTCnt<numberOfDMXUniverses; innerFTCnt++)
      {
        frameChecks[innerFTCnt][1]=0;
      }
      break;
    }
  }
  //if still checking
  if(frameCntBreak)
  {
    for(frameTCnt=0; frameTCnt<numberOfDMXUniverses; frameTCnt++)
    {
      if(frameChecks[frameTCnt][1]==1)
      {
        frameTicks++;
      }
    }
    //if all frames are checked
    if(frameTicks==numberOfDMXUniverses)
    {
      frameCntBreak=0;
      //Frames IN sync
      //clear checks
      for(frameTCnt=0; frameTCnt<numberOfDMXUniverses; frameTCnt++)
      {
        frameChecks[frameTCnt][1]=0;
      }
      //return render
      return 1;
    }
  }
  //return DO NOT render
  return 0;
}
