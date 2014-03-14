FaderOSC - Arduino Library
==========================

Library for sending OSC Messages using the analog inputs of Arduino boards.
It was designed for using it with jack-volume.

The Idea
--------

You may want to use a hardware unit for controlling some other hardware or software.
This library turns your Arduino board into a fader panel sending
[OSC](http://en.wikipedia.org/wiki/Open_Sound_Control "Wikipedia - OSC") messages
over ethernet and RS232. Originally i designed FaderOSC for the use with [jack-volume](https://github.com/voidseg/jack-volume "github.com - jack-volume")
to control the audio volume with a piece of hardware.

How It Works
------------

FaderOSC reads the analog inputs of an Arduino board and transmits the values packed
in OSC messages over ethernet and RS232. In theory an unlimited number of analog
inputs are supported but at some point the performance will be unusable. Up to 8 faders
should work properly.

Instead of connecting faders you can use other potentiometers or even analog sensors.
Please have look at the [Arduino potentiometer tutorial](http://www.arduino.cc/en/Tutorial/Potentiometer "Arduino - Potentiometer") for how to wire up a poti to an Arduino board.
All faders/potis must be connected this way to work with FaderOSC.

How To Install
--------------

This library does not work standalone because it makes use of the OSC library [oscuino](http://cnmat.berkeley.edu/oscuino). Make sure to install oscuino too.

FaderOSC is installed the same way as other Arduino libraries. Download the files
and copy them into a folder "FaderOSC" in your Arduino library folder. Read the Arduino
documentation where to find the library folder. Then restart the Arduino IDE and you can
import FaderOSC.

How To Use
----------

It is highly recommended to copy the example sketch and modify it for your needs. Note that
it is important to include SPI.h, Ethernet.h and SLIPEncodedSerial.h in your sketch before
including FaderOSC.h. To configure the OSC address you have to define the string constant
OSC_ADDRESS BEFORE including FaderOSC.h.

The number of faders / analog pins to use must be configured in the angle brackets behind
the class name at the object declaration. E. g.:
<code>static FaderOSC\<3\> faderosc;</code>

The addresses of all OSC messages contain a fixed prefix, a fader specific suffix and a float
value between 0.0 and 1.0 (including) representing the analog value. E. g.:

|    prefix    | suffix | value |
|:------------:|:------:|:-----:|
| "/faderosc/" |   "2"  | 0.42  |

Although FaderOSC was built to control jack-volume you can send OSC messages to other targets
like [DMXControl](http://www.dmxcontrol.de/) or the refrigerator of your choise. This software comes without any warranty!
