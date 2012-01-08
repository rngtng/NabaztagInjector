# NabaztagInjector

An Arduino library to send data to Violet Nabaztag via I2C (RFID injected)

## What it does

Let's use our Nabaztag Rabbit to send some data from Arduino to the Internet. Huh! - But how?

The [Nabaztag](http://en.wikipedia.org/wiki/Nabaztag) is (was) Rabbit connected to the Internet. Well, actually it's an ARM7 processor equipped with WIFI and a couple of output and input sensors.
Those input sources are:

  - a microphone
  - RFID reader
  - a button
  - tow ears which can be moved

On first sight it doesn't seam there's any option to connect an external device for sending data to the rabbit. But a closer look on the RFID reader showed, that the used CRX14 chip talks via I2C bus to the processor, and luckily the very same bus is also available on the Arduino.

The RFID reader works in a way, that on demand by the processor, any nearby RFID tags are discovered, requested for their 8 Byte long UID and reported back to the processor.

And that's where **NabaztagInjector** hooks in: It pushes the Arduino to act in the very same way the RFID chip would, but instead of returning RFID tag UIDs it sends back any custom 8 Bytes of data (make sure to check _Caveats_ below)

As the nature of I2C, each device has a unique address on the bus. So either **NabaztagInjector** or the reader can be connected at the same time - unless we are smart:

To have both devices talking to the rabbit without interference we simply switch the reader on and off and change I2C addresses on the Arduino. So either RFID reader acts as normal and Arduino has a dummy I2C address, or the reader is turned off and Arduino takes its address. To archive this, the Arduino uses a OUTPUT pin for powering the RFID reader (see_setup_) and therefore has full control over its power state.


## Usage

**NabaztagInjector** is a static class with a very simple interface reduced down to two methods:

  1. `Nabaztag.begin(<pin number>)`
  2. `Nabaztag.inject(<data>)` or `Nabaztag.inject(<*data>, length)`

The first one, `Nabaztag.begin(<pin number>)` initializes the I2C bus and turns on the RFID reader. `<pin number>` is the pin on Arduino where you connected _Vcc_ to (_see setup_). To send data to your Rabbit, simply call `Nabaztag.inject(<data>)` which puts the data into send buffer, and will be send as soon it's requested. As data is internally buffered (size 128 bytes), method call is non-blocking and can be called several times, even if data didn't got yet delivered. The default Nabaztag Firmware request a RFID about every second.

For real-live usecases, check out the examples folder for more.


## Installation

Put the library into your Arduino libraries folder, and do same with [ByteBuffer](http://siggiorn.com/wp-content/uploads/libraries/ArduinoByteBuffer.zip).


## Compilation

**NabaztagInjector** compiles with the latest Arduino IDE, as well as with the [Arduino.mk Makefile](http://mjo.tc/atelier/2009/02/arduino-cli.html). See examples folder.


## Setup

See the picture:

<img src="http://www.rngtng.com/files/2012/01/nabaztag-arduino-rfid-hack.jpg" width="500">

Starting from the Nabaztag mainboard: the RFID reader connector is on top left (Q2). From top to bottom the pins are: _VCC (Q2)_, _GND (Blue)_, _SDA (Red)_, _SCL (Green)_. Connect _SCL_ to Arduino _Analog Pin 5_, _SDA_ to _Analog Pin 4_, _GND_ to _GND_. _VCC_ can be left blank if Arduino has its own power supply, otherwise connect to Arduino power input. Connected RFID readers _VCC_ to the pin given in `Nabaztag.begin(<pin number>)`. Go to examples folder, compile and upload sketch.

That's it! Happy injecting!


## Dependencies

For I2C communication, **NabaztagInjector** depends on the Arduino core library _Wire_. The circular send buffer depends on the [_ByteBuffer_ library](http://siggiorn.com/?p=460) by Siggi, and is included within the example code.


## Caveats

As always there are some Caveats:

### Performance
The RFID chip and the Nabaztag firmware support up 16 devices UIDs per request. Unfortunately the default bootcode doesn't take advantage of that, so no way to speed it up. (unless you write your own bootcode, see _Custom bootcode_ below)

### Speed
The rabbit request rfid about every 750-1000ms. The 128 byte Buffer helps here to process high frequent data peaks, but obviously constant high frequent data will result in data loss.

### Data
The default bootcode take only IDS as valid when upper 3 Bytes are larger than zero, fill up empty data with `0xFF` is used as workaround here.

### Power
I couldn't manage to power Arduino + RFID solely by the rabbit. Arduino in standalone mode worked fine though!  Maybe simple amplifier circuit would help?

### Nabaztag version
I used a Nabaztag V2. Not sure if this works with V1 or [Karotz](http://www.karotz.com/home) as well.

### Custom bootcode
Yes, it's possible to write your own custom bootcode which allows to get rid of most of the constraints. Pls check my blogpost on that.

### Supported OS
Software developed on Mac OS X. In theory is should compile on Linux & Win smoothly. Please confirm.

### RFID Connector
Tests showed that there was actually no need to turn off the RFID reader chip while Arduino was sending. It
was fully working by connecting just both _VCC_. Actually it felt even more stable.


## Contributors

Big thanks to **ccb23** for helping me to reverse engineer the IC2 communication and reading & understanding the original [Nabaztag sources](http://code.google.com/p/nabaztag-source-code/) and the [RFID reader CRX14 specs](http://www.datasheetcatalog.org/datasheet/stmicroelectronics/8880.pdf).


## Contributing

I'll check out your contribution if you:

  - Have a clear and documented rationale for your changes.
  - Package these up in a pull request.

I'll do my best to help you out with any contribution issues you may have.


## License

The license is included as LICENSE in this directory.