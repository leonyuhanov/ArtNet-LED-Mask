# ArtNet-LED-Mask
An LED Mask using SK9822/APA102 Addressable Pixels via an ESP8266 driven by Resolume Arena 6 via ArtNet
This is an example that can be used to apply to any mapped addresable pixel system

# LED Mask
Please modify the code with your local network WIFI details

Consists of 370 x SK9822 Pixels driven by an ESP8266 Wemos D1 Mini

The ESP8266 has a Single pixel connected very close to the board(this acts as a voltage level shifter) to Pin 5(CLOCK), Pin 7(data),+5v and GND. The pixel then connectes directly to the MASK via a 4pin JST

The Mask is laid out in a particular patern as described in the ODS Spreadsheet

The Mask requires at least 3A power supply/battery 

# Resolume Arena Set up
You will need to have Resolume Arena 6. Your machine needs to be connected to a network that has 2.4ghz b/g/n WIFI. 

Copy the fixture and Advanced Output files into your resoulme config directory in your my docs folder.

The Pixel map is intended for 1080 x 768 Scene, bu tyou can adjust as you please. The map is made to be 25 screen pixels(1:1) per LED pixel

# Getting it all running

1.  Upload the code to your microcontroller, open the serial monitor and confirm its connecetd to your WIFI network. Note the IP address
2.  Start Resolume with a blank scene, pull down the mian opacity slider to 1/4 so you dont get blinded
3.  Configure DMX output to whatever network interfecae your wish
4.  Open the Advnaced Output section of Resolume and select "Full Face 2008 - 25ppp"
5.  There are 3 Lumiverses named "Frame 1", "Frame 2" and "Frame 3" set them to tasmit to the IP address of your esp8266 module noted in step 1
6.  Set "Frame 1" to Universe 0, "Frame 2" to Universe 1 and "Frame 2" to Universe 2
7.  Activate a scene
