# NabaztagInjector

An Arduino library to send data to Nabaztag via I2C (RFID injected)

## What it does

Let's use our Nabaztag Rabbit to send some data from Arduino to the Internet. Huh! - But how?

The [Nabaztag](http://en.wikipedia.org/wiki/Nabaztag) is (was) Rabbit connected to the Internet. Well, actually it's an ARM7 processor equipped with WIFI and a couple of output and input sensors.
Those input sources are:

- a microphone
- RFID reader
- a button
- tow ears which can be moved

On first sight it doesn't seam there's any option to connect an external device for sending data to the rabbit. But a closer look on the RFID reader shows, that the used CRX14 chip talk via I2C bus to the processor, and luckily the very same bus is also available on the Arduino.

The RFID reader works in a way, that on demand by the processor, any nearby RFID tags are discovered, requested for their 8 Byte long UID and reported back to the processor.

And that's where **NabaztagInjector** hooks in: It pushes the Arduino to act in the very same way like the RFID chip, but instead of returning RFID tag UIDs it sends back any custom 8 Bytes of data.

As the nature of I2C, each device has a unique address on the bus. So either **NabaztagInjector** or the reader can be connected at the same time - unless we are smart:

To have both devices talking to the rabbit without interference we simply switch the reader on and off and change I2C addresses on the Arduino. So either RFID reader acts as normal and Arduino has a dummy I2C address, or the reader is turned off and Arduino takes its address. To archive this, the Arduino uses a OUTPUT pin for powering the RFID reader (see_setup_) and therefore has full control over its power state.

## Usage

**NabaztagInjector** is a static class with a very simple interface reduced down to two methods:

1. `Nabaztag.init(<pin number>)`
2. `Nabaztag.inject(<data>)`

The first one, `Nabaztag.init(<pin number>)` initializes the I2C bus and turns on the RFID reader. `<pin number>` is the pin on Arduino where you connected _Vcc_ to (_see setup_). To send data to your Rabbit, simply call `Nabaztag.inject(<data>)` which puts the data into send buffer, and will be send as soon it's requested. As data is internally buffered (size 128 bytes), method call in non-blocking and can be called several times, even if data didn't got yet delivered.

For real-live usecases, check out the examples folder for more.


## Installation
Put the library into your Arduino libraries folder, and do same with [ByteBuffer](http://siggiorn.com/wp-content/uploads/libraries/ArduinoByteBuffer.zip).


## Compilation
**NabaztagInjector** compiles with the latest Arduino IDE, as well as with the [Arduino.mk Makefile](http://mjo.tc/atelier/2009/02/arduino-cli.html). See examples folder.


## Setup
See the picture:


Start from the Nabaztag mainboard, facing the rabbit: the RFID reader connector is on top right (Q2). From left to right: SCL, SDA, GND, VCC. Connect SCL to Arduino Pin 5, SDA to Pin 4, GND to GND. VCC can be left blank if Arduino has its own power supply, otherwise connect to Arduino power input. In this case RFID reader needs a small amplifier (see TODO). Connected RFID reader VCC to pin given in `Nabaztag.init(<pin number>)` (best is 12 here).

That's it!

## Dependencies

For I2C communication, **NabaztagInjector** depends on the Arduino core library _Wire_. The circular send buffer depends on the [_ByteBuffer_ library](http://siggiorn.com/?p=460) by Siggi, and is included within the example code.


## Todo

- Testing:
  - Speed: Especially the behavior on several fast injections within short time is yet not tested. I expect data loss here, as the rabbit is just to slow and drops the packages. Some delays would may help here.
  - OS: Software developed on Mac OS X. In theory is should compile on Linux & Win smoothly. Please confirm.
  - Nabaztag version: I used a Nabaztag V2. Not sure if this works with V1 or [Karotz](http://www.karotz.com/home) as well.
- the RFID response allows to embed up to 16 devices UIDs. Check if this works and promises a speedup??
- it's "Little Endian" vs. "Big Endian" so check need of reversing data input
- as seen while testing, at least the three lower bytes have to be set (to be detected as valid RFID UID?). Check how to handle this in code nice (by now we fill up with 0xFF)
- when Arduino is just powered by Nabaztag, the pin output voltage seams to be too low to the power RFID reader. A simple amplifier circuit would be nice here.


## Contributors

Big thanks to **ccb23** for helping me to reverse engineer the IC2 communication and reading & understanding the original [Nabaztag sources](http://code.google.com/p/nabaztag-source-code/) and the [RFID reader CRX14 specs](http://www.datasheetcatalog.org/datasheet/stmicroelectronics/8880.pdf).


## Contributing

We'll check out your contribution if you:

- Have a clear and documented rationale for your changes.
- Package these up in a pull request.

We'll do our best to help you out with any contribution issues you may have.


## License

The license is included as LICENSE in this directory.