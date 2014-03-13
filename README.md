FaderOSC-Library
================

Library for sending OSC Messages using the analog inputs of Arduino boards.
It was designed for using it with jack-volume.

FaderOSC can be used for transmitting analog values from an Arduino board
over UDP oder serial interface as OSC messages. This requires either a serial
interface or an ethernet port or both on your board. A variable number of
analog pins can be used by this library. This number is configured with the
template argument "NUM_POTS".

Please have look at the [Arduino potentiometer tutorial](http://www.arduino.cc/en/Tutorial/Potentiometer "Arduino - Potentiometer") for how to wire up a poti to an Arduino board.
All faders/potis must be connected this way to work with FaderOSC.

The library is called "FaderOSC" because initially it was ment for using it with
analog faders as input device. Other potentiometers or sensors connected to the
analog pins might also work.

The addresses of all OSC messages contain a fixed prefix, a fader specific
suffix and a float value between 0.0 and 1.0 (including) representing the
analog value. E. g.:

|  prefix   | suffix | value |

"/faderosc/"   "2"      0.42

This library was built for controlling a jack-volume instance but can also be
used for other purposes.
