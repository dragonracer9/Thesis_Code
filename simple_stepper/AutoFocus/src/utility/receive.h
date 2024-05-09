#ifndef _RECIEVE_H
#define _RECIEVE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include "../include/__globals__.h"
#include "../include/pins.h"
#include <stdint.h>
#include <stdlib.h> /* strtoul */
#include <string.h>

/*************************************************
 * |             NEW PROTOCOL                   | *
 * ---------------------------------------------- *
 * | ~~ | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 |-     *
 * | 0x05 | 0x06 | 0x07 | 0x08 | ** |             *
 * ---------------------------------------------- *
 *                                                *
 * packet begins with the "~" character repeated  *
 * twice this is to ensure that the packet is not *
 * confused with dimitar's protocol               *
 * | ~~ |                                         *
 *                                                *
 * 0x00 - is the motor  byte                      *
 * the motor byte is used to identify which       *
 * motor the packet is intended for               *
 * | 0x00 |:  values 1-4                          *
 *                                                *
 * 0x01 - is the state byte                       *
 * | 0x01 |:  values 1-4                          *
 *                                                *
 * 0x02 - is the direction byte                   *
 * | 0x02 |:  values: 0 || 1, where 0 is FWD      *
 *                                                *
 * 0x03 - 0x07 - are the steps bytes              *
 * | 0x03 | - | 0x07 |                            *
 *                                                *
 * 0x08 - is the speed byte                       *
 * | 0x08 |: this is in [deg/s]                   *
 *                                                *
 * 0x09 - is the end byte                         *
 * the end byte is used to identify the end       *
 * of a packet (terminator is '*' repeated once)  *
 * | ** |                                         *
 *                                                *
 *                                                *
 * We then need to send back an ACK packet        *
 * | ~~ | ACK | ** |                              *
 * (ACK - is the ACK byte, plaintext "ACK")       *
 *                                                *
 * ALL WHITESPACE SHOULD BE IGNORED               *
 * (but for safety, pls only send coherent        *
 * packets with no whitespace)                    *
 *************************************************/

// packet structure
typedef struct packet {
    uint8_t motor;
    uint8_t state;
    uint8_t direction;
    uint32_t steps;
    uint8_t speed;
    packet()
        : motor(0)
        , state(0)
        , direction(0)
        , steps(0)
        , speed(0)
    {
    }

    void print()
    {
        Serial.print("Motor: ");
        Serial.println(motor);
        Serial.print("State: ");
        Serial.println(state);
        Serial.print("Direction: ");
        Serial.print(direction);
        Serial.print(" ");
        dir_t dir = int_to_dir(direction);
        Serial.println(dir_to_str(dir));
        Serial.print("Steps: ");
        Serial.println(steps);
        Serial.print("Speed: ");
        Serial.println(speed);
    }

} packet_t;

inline bool new_data { false };

ret_t handle_transmission(packet_t* const);
ret_t recieve_packet /*__attribute__((unused))*/ (char* const);
ret_t parse_packet /*__attribute__((unused))*/ (char* const, packet_t* const);
const uint32_t parse_token /*__attribute__((unused))*/ (char* const);
ret_t tokenize /*__attribute__((unused))*/ (char* const, uint8_t&, uint8_t&, uint8_t&, uint32_t&, uint8_t&);

#endif //_RECIEVE_H
