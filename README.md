# NabaztagInjector

An Arduino library to send data to Nabaztag via I2C (RFID injected)

## What it does

Let's use our Rabbit to send some data from Arduino to the Internet. But how?

The [Nabaztag](http://en.wikipedia.org/wiki/Nabaztag) is (was) Rabbit connected to the Internet. Well,
actually it's an ARM7 processor equipped with WIFI and a couple of output and input sensors.
Those input sources are:

- a microphone
- RFID reader
- a button
- tow ears which can be moved

On first sight it doesn't seam there's any option to connect an external device to send data to the rabbit.
But a closer look on the RFID reader show, that the chip is connected via I2C bus to the processor, and luckily
the very same bus is also available on the Arduino.

The RFID reader works in a way that on demand from the processor, is checks if any RFID tags are around, and if so,
it send their ID back to the processor. This ID can be any 8 Byte long data.

And that's what **NabaztagInjector** makes use of. It pushes the Arduino to pretend it is the RFID chip and on request
it sends 8 Byte back.

//TODO
For not using the actually RFID read feature,


## Usage

**NabaztagInjector** is a static class with a very simple interface reduced down to two methods:

1. `Nabaztag.init(<pin number>)`
2. `Nabaztag.inject(<data>)`

The first one, `Nabaztag.init(<pin number>)` initializes the I2C bus and turns on the RFID reader. `<pin number>` is the pin on Arduino
where you connected _Vcc_ to. The second one, `Nabaztag.inject(<data>)` puts the data into send buffer, and send it to the Nabaztag as soon
as it's requested. As data is internally buffered (size 128 bytes), method call in non-blocking and can be called several times, even if data
didn't got yet delivered.

For real-live usecases, check out the examples folder.


## Installation
Put the library into your Arduino libraries folder, and do same with [ByteBuffer](http://siggiorn.com/wp-content/uploads/libraries/ArduinoByteBuffer.zip).


## Compiling
**NabaztagInjector** compiles with the latest Arduino IDE, as well as with the [Arduino.mk Makefile](http://mjo.tc/atelier/2009/02/arduino-cli.html). See examples folder.


## Dependencies

For I2C communication, **NabaztagInjector** depends on the Arduino core library _Wire_. The circular send buffer, depends on the [_ByteBuffer_ library by Siggi](http://siggiorn.com/?p=460),
and included within the code.


## Todo

- lots of testing

Especially the behavior on several fast injections within short time is yet not tested. I expect data loss here, as the rabbit is just to slow
and drops the packages. Some delays would may help here.


## Contributors

Big thanks to **ccb23** for helping me to reverse engineer the IC2 communication and reading & understanding the original [Nabaztag sources]() and
the [CRX14 specs]().


## Contributing

We'll check out your contribution if you:

- Have a clear and documented rationale for your changes.
- Package these up in a pull request.

We'll do our best to help you out with any contribution issues you may have.


## License

The license is included as LICENSE in this directory.