/*
   Nabaztag.cpp - Arduino library to send data to Nabaztag via I2C (RFID injected)
   Copyright (c) 2011 Tobias Bielohlawek.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Nabaztag.h>
#include <Wire.h>
#include <utility/twi.h>

void receiveCallback(int length)
{
  Nabaztag.processReceive(length);
}

void requestCallback()
{
  Nabaztag.processRequest();
}

volatile boolean NabaztagInjector::inited = false;
volatile boolean NabaztagInjector::sendEnabled = false;
int NabaztagInjector::rfidPort = 0;

byte NabaztagInjector::in[16];
uint8_t NabaztagInjector::out[32];
size_t NabaztagInjector::outSize = 0;

ByteBuffer NabaztagInjector::sendBuffer;

// Constructors ////////////////////////////////////////////////////////////////

NabaztagInjector::NabaztagInjector()
{
}

// Public //////////////////////////////////////////////////////

void NabaztagInjector::init(int _rfidPort)
{
  rfidPort = _rfidPort;
  if(rfidPort > 0) {
    pinMode(rfidPort, OUTPUT);
    digitalWrite(rfidPort, LOW);
  }

  sendBuffer.init(SEND_BUFFER_SIZE);

  Wire.begin(CRX14_ADR);
  Wire.onReceive(receiveCallback);
  Wire.onRequest(requestCallback);

  enableRFID();
}


void NabaztagInjector::inject(uint8_t* data, uint8_t length)
{
  for(int i = 0; i < length; i++) {
    sendBuffer.put(data[i]);
  }
  disableRFID(); //aka start injection :-)
}

void NabaztagInjector::inject(uint8_t data)
{
  inject(&data, 1);
}

void NabaztagInjector::inject(char* data)
{
  inject((uint8_t*)data, strlen(data));
}


void NabaztagInjector::inject(int data)
{
  inject((uint8_t)data);
}

// Callbacks //////////////////////////////////////////////////////

void NabaztagInjector::processReceive(int length) {
  noInterrupts();

  for(int i = 0; i < length; i++) {
    byte data = Wire.read();
    in[i] = data;
  }

  byte cmd = getCommand(length);

  if( cmd == CMD_INIT ) inited = true;
  if( cmd == CMD_CLOSE ) inited = false;

  if( inited ) {
    if( cmd == CMD_READ ) sendEnabled = true; //aparently a send is always expected after a read
    if( cmd == CMD_INITATE ) { //not sure for CMD_SELECT, but code shows that it need response
      byte s = ( sendBuffer.getSize() > 0 ) ? 1 : 0;
      byte outNew[] = { s, 1 };
      outSize = 2;
      memcpy(out, outNew, outSize);
    }
    if( cmd == CMD_SLOT ) {
      byte outNew[] = { 18,  0x01, 0x00,   0x0F, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0 }; //length, BitMask, TagID
      outSize = 19;
      memcpy(out, outNew, outSize);
    }
    if( cmd == CMD_SELECT ) { //not sure for CMD_SELECT, but code shows that it need response
      byte outNew[] = { 1, 1 };
      outSize = 2;
      memcpy(out, outNew, outSize);
    }
    if( cmd == CMD_GET_UID ) {
      outSize = 9;
      prepareOutBuffer();
    }
  }

  interrupts();
}

void NabaztagInjector::processRequest() {
  if( sendEnabled && outSize > 0 ) {
    Wire.write(out, outSize);

    if( sendBuffer.getSize() < 1 ) { //all data is send
      inited = false;
      enableRFID();
    }
    outSize = 0;
    sendEnabled = false;
  }
}

// Private //////////////////////////////////////////////////////

void NabaztagInjector::enableRFID() {
  if(rfidPort > 0) {
    twi_setAddress(DUMMY_ADR);
    digitalWrite(rfidPort, HIGH);
  }
}

void NabaztagInjector::disableRFID() {
  if(rfidPort > 0) {
    twi_setAddress(CRX14_ADR);
    digitalWrite(rfidPort, LOW);
  }
}

byte NabaztagInjector::getCommand(int length) {
  switch( length ) {
    case 4:
      if( in[0] == 0x01 && in[1] == 0x02 && in[2] == 0x06 && in[3] == 0x00 ) return CMD_INITATE;
      if( in[0] == 0x01 && in[1] == 0x02 && in[2] == 0x0E ) return CMD_SELECT; //forth byte is variable
      break;
    case 3:
      if( in[0] == 0x01 && in[1] == 0x01 && in[2] == 0x0B ) return CMD_GET_UID;
      if( in[0] == 0x01 && in[1] == 0x01 && in[2] == 0x0F ) return CMD_COMP;
      break;
    case 2:
      if( in[0] == 0x00 && in[1] == 0x10 ) return CMD_INIT;
      if( in[0] == 0x00 && in[1] == 0x00 ) return CMD_CLOSE;
      break;
    case 1:
      if( in[0] == 0x01 ) return CMD_READ;
      if( in[0] == 0x03 ) return CMD_SLOT;
      break;
   }
  return CMD_UNKNOWN;
}

void NabaztagInjector::prepareOutBuffer() {
  out[0] = 0x00; //Dummy first Byte
  for( int i = 8; i > 0; i-- ) {
    if( sendBuffer.getSize() > 0 ) {
      out[i] = sendBuffer.get();
    }
    else {
      out[i] = 0xFF; //fill up with bits
    }
  }
}
// Preinstantiate Objects //////////////////////////////////////////////////////

NabaztagInjector Nabaztag = NabaztagInjector();
