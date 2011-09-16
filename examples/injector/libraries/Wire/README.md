# Wire Library

Library taken from Arduino Core with two small changes to make it compiling with Arduino.mk

- changed file name: `twi.c` -> `twi.cpp`
- in `Wire.cpp`: moved `#include 'twi.h'` out of `extern C` block 