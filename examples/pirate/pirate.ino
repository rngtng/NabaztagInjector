
/*
 * LOW to trigger the interrupt whenever the pin is low,
 * CHANGE to trigger the interrupt whenever the pin changes value
 * RISING to trigger when the pin goes from low to high,
 * FALLING for when the pin goes from high to low.
 */

#include <Nabaztag.h>

#define INIT_DATA1 DDRD = DDRD & ~0xF8 //b0000 0111 - init pins as INPUT
#define INIT_DATA2 DDRC = DDRC & ~0x0F //b1111 0000

#define READ_CHAN1 (PIND & 0xF8) //pins D4 - D7
#define READ_DATA2 (PINC & 0x0F) << 4 //pins A0 - A3

#define IR_CLK 0 //PIN 2
#define IR_STATUS 1 //PIN 3

#define CMD_STOP 0xFF
#define BUFFER_SIZE 100

volatile byte buffer[BUFFER_SIZE];
volatile int bufferRead = 0; //points to last read position
volatile int bufferWrite = 0; //points to last written position

void read_data() {
  byte data_and_stat = READ_CHAN1;

  if(data_and_stat & 0x08) { //check if stat is set
    bufferWrite = (bufferWrite + 1) % BUFFER_SIZE;
    buffer[bufferWrite] = READ_DATA2 | (data_and_stat >> 4);

  } else if(buffer[bufferWrite] != CMD_STOP) {
    bufferWrite = (bufferWrite + 1) % BUFFER_SIZE;
    buffer[bufferWrite] = CMD_STOP;

  }
}

void setup() {
  INIT_DATA1;
  INIT_DATA2;
  // Serial.begin(115200);
  Nabaztag.begin(9); // RFID Reader connected atPIN 9, leave empty for standalone

  buffer[bufferWrite] = CMD_STOP;
  attachInterrupt(IR_CLK, read_data, RISING);
  attachInterrupt(IR_STATUS, read_data, RISING);
}

void loop() {
  if( bufferWrite != bufferRead ) {
    byte data; //ANSI C
    bufferRead = (bufferRead + 1) % BUFFER_SIZE;
    data = buffer[bufferRead];
    Nabaztag.inject(data);
  }

}

