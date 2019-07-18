# D70Box

D70Box

## Required Hardware

The circuit is based around the Arduino Pro Micro. This was purchased via ebay
from the URL https://www.ebay.co.uk/itm/1-2-5-10PCS-Pro-Micro-ATmega32U4-16MHz-5V-Replace-Pro-Mini-ATmega328-For-Arduino/283052304952?ssPageName=STRK%3AMEBIDX%3AIT&var=583438190183&_trksid=p2057872.m2749.l2649 but
it must be noted that other vendors supply this device too! Other vendors, especially if you are willing to wait, sell these much cheaper!

The HC-05 Bluetooth module was also purchased via ebay from the URL https://www.ebay.co.uk/itm/HM-10-BLE-Bluetooth-4-0-CC2540-CC2541-Serial-Wireless-Module-Arduino-F/123600639430?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649 
but it must be noted that other vendors supply this device too! The device is advertised as the HM-10 but it should be fine.

## Installing the Build Software

Start by installing the Arduino IDE. This can be found at:

https://www.arduino.cc/en/Main/Software

For the purpose of this build the Windows version Arduino 1.8.8 was used. 
All default installation options were accepted.

## Compiling the source code

- Download the associated ZIP file and uncompress it on your drive
- Open the Arduino IDE and then open the D70Box.ino project file in the Source\D70Box folder
- Navigate to Tools > Boards and select "Arduino Leonardo"
- Click the "Verify" button (Round circle with a tick mark in it - top left), if successful you will see something like 
"Sketch uses 14522 bytes (50%) of program storage space. Maximum is 28672 bytes.
Global variables use 741 bytes (28%) of dynamic memory, leaving 1819 bytes for local variables. Maximum is 2560 bytes."
though the number may not be exact.
- Make sure that the Arduino is plugged into your PC. Navigate to Tools > Port and make sure that the correct port is selected.
- Click the "Upload" button (Right pointing arrow - top left) to up load the software.
- The device should now be programmed correctly.

## PCB Revisions

- V1.0 - April 5th, 2019 - Initial board design.
- V1.1 - ?
- V1.2 - May 24th, 2019 - Added BlueTooth support.
- V1.3 - June 4th, 2019 - Fixed legend for T1, should read BFY51 and not BFY71.

## Notes

The Delay and Speed setting may seem wrong relative to the knob position. The best way to check this is to use the BlueTooth
interface and look for the Delay and Speed setting. Adjust the two control until the values displayed match values on the
legend. Then adjust the knobs so that the indicator is as close as possible to the legend value. The attained proximity will 
depend on the potentiometer shaft type, smooth or knurled / serrated etc.

The BFY51 transistor can be substituted with a 2N3019. See https://ham.stackexchange.com/questions/8990/can-anyone-please-suggest-a-substitute-for-the-bfy51-transistor/9031

