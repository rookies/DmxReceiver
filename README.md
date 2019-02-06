DmxReceiver
===========

This is a small Arduino library for receiving
[DMX512](https://en.wikipedia.org/wiki/DMX512) signals.

It is based on [dmx_serial_sender](https://github.com/ktgow/dmx_serial_sender),
I basically just wrapped a class around the code of this sketch to make it
usable in an easy and clean way.

You need to connect a DMX receiver (e.g. MAX485) to a pin of your Arduino to
use this library.

Tested with Arduino Nano (ATmega328P), but it should also work with other
boards using the same microcontroller.

## Example code
```cpp
#include <DmxReceiver.h>

/* Define the pin where the DMX receiver is connected: */
const byte pinDmx = 6;
/* Define the number of channels we want to receive: */
const byte channels = 3;
/* NOTE: Currently this means we will receive data on
         the first three channels, an option to define
         a channel offset is planned. */

DmxReceiver dmx(pinDmx, channels);

void setup() {
  /* Do your setup ... */
}

void loop() {
  /* Check for new DMX data: */
  if (dmx.poll()) {
    byte channel0 = dmx.getValue(0);
    byte channel1 = dmx.getValue(1);
    byte channel2 = dmx.getValue(2);
    /* Do things with the data we just received ... */
  }
}
```
