# D70Box

The D70Box project is documented in Practical Wireless, October 2019. If you require further information,
such as the associated circuit diagram or a PCB, then please contact me at martin@the-wallers.net.

# Erratum

A small design fault has been noticed. On the 74LS00 quad NAND gate pins 1,2,3,4,5,6, and 7 are all 
connected to ground. Pins 3 and 6 should have been left unconnected. This will not impact the overall 
functionality of the device but may result in slightly more current drain.

The suggested fix, for those who have soldered the 74LS00 directly onto the board and for those using 
a DIL socket, is to simply snip off pins 3 and 6.

V1.4 of the PCB is now available that will fix the problem without the need to snip off pins.

+--

A problem has been reported building the code with the Arduino 1.5.6-r2 IDE. This manifests with an
error saying "SwitchStates does not name a type". If you are facing this issue then please contact
me at martin@the-wallers.net and I'll provide the fix.

## Required Hardware

Do shop around for these devices on ebay. I've noticed that the prices associated with the links below
have risen. You should be aiming to pay around £4 - £5 for the Arduino Pro Micro, or less if you buy from
China, and £2 - £3 for the Bluetooth module.

The circuit is based around the Arduino Pro Micro. This was purchased via ebay
from the URL https://www.ebay.co.uk/itm/1-2-5-10PCS-Pro-Micro-ATmega32U4-16MHz-5V-Replace-Pro-Mini-ATmega328-For-Arduino/283052304952?ssPageName=STRK%3AMEBIDX%3AIT&var=583438190183&_trksid=p2057872.m2749.l2649 but
it must be noted that other vendors supply this device too! Other vendors, especially if you are willing to wait, sell these much cheaper!

The HC-05 Bluetooth module was also purchased via ebay from the URL https://www.ebay.co.uk/itm/HM-10-BLE-Bluetooth-4-0-CC2540-CC2541-Serial-Wireless-Module-Arduino-F/123600639430?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649 
but it must be noted that other vendors supply this device too! The device is advertised as the HM-10 but it should be fine.

Other components include:

Resistors

- R1 150
- R5 2K
- R6 1K

Variable Resistors

- R2 10K Lin
- R3 10K Lin
- R4 22K Lin (Switched)

Semicondictors

- Q1 BFY51 or 2N3019
- U2 74LS00

Miscellaneous

- U3 Switch SPDT centre off
- S1 3.5 mm switch mono socket
- S2 3.5 mm switch mono socket
- SP1 8 Ohm speaker
- Header pins as required

DIL sockets for U2
Header sockets for Arduino and Bluetooth adapter

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
- Make sure that the Arduino is plugged into your PC. Navigate to Tools > Port and make sure that the correct port is selected. See USB Issues below.
- Click the "Upload" button (Right pointing arrow - top left) to up load the software.
- The device should now be programmed correctly.

## USB Issues

Some users have reported issues connecting the Arduino to the PC USB 3.0 sockets.
It seems that the Arduino Pro-Micro is happier connected to a USB 2.0 sockets for programming.
More information can be found here: https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all

## PCB Revisions

- V1.4 - September 29th, 2019 - Fixed the issue with 7400 outputs pins 3 and 6 being tied low.
- V1.3 - June 4th, 2019 - Fixed legend for T1, should read BFY51 and not BFY71.
- V1.2 - May 24th, 2019 - Added BlueTooth support.
- V1.1 - ?
- V1.0 - April 5th, 2019 - Initial board design.

## Notes

The Delay and Speed setting may seem wrong relative to the knob position. The best way to check this is to use the BlueTooth
interface and look for the Delay and Speed setting. Adjust the two control until the values displayed match values on the
legend. Then adjust the knobs so that the indicator is as close as possible to the legend value. The attained proximity will 
depend on the potentiometer shaft type, smooth or knurled / serrated etc.

The BFY51 transistor can be substituted with a 2N3019. See https://ham.stackexchange.com/questions/8990/can-anyone-please-suggest-a-substitute-for-the-bfy51-transistor/9031

