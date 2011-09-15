/*
   injector.pde - Example usage of NabaztagInjector Arduino Library
   Copyright (c) 2011 Tobias Bielohlawek.  All right reserved.
*/

#include <Nabaztag.h>

int cnt = 0;

void setup() {
  Nabaztag.init(12);
}

/* inject every 4 second the current counter value to Nabaztag */

void loop() {
  delay(4000);

  Nabaztag.inject(cnt);
  cnt++;
}
