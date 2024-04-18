#ifndef _RECIEVE_H
#define _RECIEVE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include "__globals__.h"
#include "pins.h"
#include <stdint.h>

/**************************************************
 * |             NEW PROTOCOL                   | *
 * ---------------------------------------------- *
 * | < | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | * |   *
 * ---------------------------------------------- *
 *                                                *
 * packet begins with "<" character               *
 * | < |                                          *
 *                                                *
 * 0x00 - is the motor  byte                      *
 * the motor byte is used to identify which       *
 * motor the packet is intended for               *
 * | 0x00 |                                       *
 *                                                * 
 * 0x01 - is the state byte                       *
 * | 0x01 |                                       *
 *                                                *
 * 0x02 - is the direction byte                   *
 * | 0x02 |                                       *
 *                                                *
 * 0x03 - is the steps byte                       *
 * | 0x03 |                                       *
 *                                                *   
 * 0x04 - is the speed byte                       *
 * | 0x04 |                                       *
 *                                                * 
 * 0x05 - is the end byte                         *
 * the end byte is used to identify the end       *
 * of a packet                                    *
 * | * |                                          *
 * 
 * ***********************************************/
 * 
 **************************************************/





/****************************************************
 * DIMITAR'S CODE ((START)) 
 * DO NOT TOUCH
 * **DEPRECATED**
 ***************************************************/


inline const uint8_t NUM_CHARS = 32;
inline uint8_t receivedChars[NUM_CHARS];
inline bool markerFlags[20] { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
inline bool newData { false };

void receiveMarkers(); // NOTE: this protocol relies on there being no random strings thrown at the board
void processReceivedData();
int identifyReceivedMarker();

void setRotationSpeed(int);
void setStepsNumber(int);

#endif //_RECIEVE_H
