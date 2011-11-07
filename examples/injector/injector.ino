/*
   injector.pde - Example usage of NabaztagInjector Arduino Library
   Copyright (c) 2011 Tobias Bielohlawek.  All right reserved.
*/

// uncomment when using Arduino IDE
// #include <Wire.h>
// #include <ByteBuffer.h>

#include <Nabaztag.h>

int cnt = 0;

void setup() {
  Nabaztag.init(26); // Analog Port 03
}

/* inject every 4 second the current counter value to Nabaztag */

void loop() {
  delay(10000);

  Nabaztag.inject(cnt);
  cnt++;
}
