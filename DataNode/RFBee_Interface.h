//  Firmware for rfBee 
//  see www.seeedstudio.com for details and ordering rfBee hardware.

//  Copyright (c) 2010 Hans Klunder <hans.klunder (at) bigfoot.com>
//  Author: Hans Klunder, based on the original Rfbee v1.0 firmware by Seeedstudio
//  Version: July 14, 2010
//
//  This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with this program; 
//  if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#ifndef RFBEE_INTERFACE_H
#define RFBEE_INTERFACE_H
#define FIRMWAREVERSION 11 // 1.1  , version number needs to fit in byte (0~255) to be able to store it into config



//leftovers from the RFBee_Interface that we cant get rid of


struct RFBee
{
  void Startup(void);
  boolean Update(void);
  void sendData(char* data, uint8_t len);
  void getMessage(char* message);
 private:
  int myAddress; 
  //char vector[63];
  char messageRecieved[63] = {0};
  uint8_t recievedBits = 0;
  long int firstPackageRecieved = 0;
  //functions
  void rfSensorUpdate(char* dataOutput);
  void rfBeeInit(void);
  void rftoSerial(byte* dataOutput);
};
#endif
