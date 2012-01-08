# NabaztagInjector

Example Injector

Read data from Serial port and 'inject' via RFID into Nabaztag rabbit.

## Compilation

There are to ways to compile this example:

### Arduino IDE
Put the Nabaztag, and the [ByteBuffer](http://siggiorn.com/wp-content/uploads/libraries/ArduinoByteBuffer.zip) library into your Arduino lib folder.

### Command Line (Arduino.mk)
A custom version of **Arduino.mk** is included as submodule. Run:

    $ git submodule init
    $ git submodule update
    $ make
    $ make upload

For more on git submodules, check: http://help.github.com/submodules/
