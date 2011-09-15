/*
  Nabaztag.h -  Inject RFID library for Arduino & Wiring
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

#ifndef Nabaztag_h
#define Nabaztag_h

#include "WProgram.h"
#include <ByteBuffer.h>

#define SEND_BUFFER_SIZE 128

#define CRX14_ADR    80
#define DUMMY_ADR    23

#define CMD_INIT     120 // cmd: 0x00 0x01
#define CMD_INITATE  121 // cmd: 0x01 0x02 0x06 0x00
#define CMD_SLOT     122 // cmd: 0x03
#define CMD_SELECT   123 // cmd: 0x01 0x02 0x0E
#define CMD_GET_UID  124 // cmd: 0x01 0x01 0x0B
#define CMD_COMP     125 // cmd: 0x01 0x01 0x0F
#define CMD_READ     126 // cmd: 0x01
#define CMD_CLOSE    127 // cmd: 0x00 0x00
#define CMD_UNKNOWN  128

class NabaztagInjector
{
  private:
    static volatile boolean inited;       // init comand received, ok to go
    static volatile boolean sendEnabled;  // only response when true
    static int rfidPort;                  // pin where RFID chip is conected

    static byte in[];                     // in buffer for reading
    static uint8_t out[];                 // out buffer for sending
    static int outSize;                   // size of Buffer to send

    static ByteBuffer sendBuffer;         // actual data to send

    byte getCommand(int length);          // map received data to command byte
    void enableRFID();
    void disableRFID();
    void prepareOutBuffer();              // move from to sendBuffer int out buffer
  public:
    NabaztagInjector();
    void init(int);                       // pin number where RFID chip is connected
    void inject(uint8_t);
    void inject(uint8_t*, uint8_t);
    void inject(int);
    void inject(char*);

    void processReceive(int length);
    void processRequest();
};

extern NabaztagInjector Nabaztag;

#endif

