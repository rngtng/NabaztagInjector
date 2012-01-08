/*
   injector.pde - Example usage of NabaztagInjector Arduino Library
   Copyright (c) 2012 Tobias Bielohlawek.  All right reserved.
*/

// uncomment when using Arduino IDE
// #include <Wire.h>
// #include <ByteBuffer.h>

#include <Nabaztag.h>

void setup() {
  Serial.begin(9600);
  Nabaztag.begin(9); // User 0 for standalone
}

void loop() {

  while(Serial.available() > 0) {
    //read the incoming byte ...
    byte data = Serial.read();

    //... and inject to the rabbit
    Nabaztag.inject(data);
  }

}
