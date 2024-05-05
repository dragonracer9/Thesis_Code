#ifndef _RECIEVE_H
#define _RECIEVE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include "../include/__globals__.h"
#include "../include/pins.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>     /* atoi */


/*************************************************
 * |             NEW PROTOCOL                   | *
 * ---------------------------------------------- *
 * | ~ | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | * |   *
 * ---------------------------------------------- *
 *                                                *
 * packet begins with "~" character               *
 * this is to ensure that the packet is not       *
 * confused with dimitar's protocol               *
 * | ~ |                                          *
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
 * 0x03 - is the steps byte                       *
 * | 0x03 |                                       *
 *                                                *
 * 0x04 - is the speed byte                       *
 * | 0x04 |: this is in [deg/s]                   *
 *                                                *
 * 0x05 - is the end byte                         *
 * the end byte is used to identify the end       *
 * of a packet                                    *
 * | * |                                          *
 *                                                *
 *************************************************/

// packet structure
typedef struct packet {
    uint8_t motor;
    uint8_t state;
    uint8_t direction;
    uint8_t steps;
    uint8_t speed;
    packet()
        : motor(0)
        , state(0)
        , direction(0)
        , steps(0)
        , speed(0)
    {
    }

    inline int to_string(char* const buffer) const
    {
        return sprintf(buffer, "motor: %d, state: %d, direction: %d, steps: %d, speed: %d", motor, state, direction, steps, speed);
    }
} packet_t;

inline bool new_data { false };

ret_t handle_transmission(packet_t* const);
ret_t recieve_packet /*__attribute__((unused))*/ (uint8_t* const);
ret_t parse_packet /*__attribute__((unused))*/ (uint8_t* const, packet_t* const);

/****************************************************
 * DIMITAR'S CODE ((START))
 * DO NOT TOUCH
 * **DEPRECATED**
 ***************************************************/
/*
#define STEPS_PER_REVOLUTION 200
#define MICROSTEPPING_FACTOR 16

#define CALC_HALF_PULSE_FACTOR(MICROSTEPPING_FACT) (1000000L * 360L / 2L / STEPS_PER_REVOLUTION / MICROSTEPPING_FACT)
#define HALF_PULSE_FACTOR CALC_HALF_PULSE_FACTOR(MICROSTEPPING_FACTOR)

float dimitar_speedRotation[NR_MOTORS] = { 100, 100, 100, 100, 100 };
long dimitar_half_pulse_duration_us[NR_MOTORS] = { 562, 562, 562, 562, 562 };
unsigned long dimitar_time_of_next_half_pulse[NR_MOTORS] = { 0, 0, 0, 0, 0 };

int dimitar_stepsToMove[NR_MOTORS] = { 0, 0, 0, 0, 0 }; // Add this global variable to store steps for each motor.

inline const uint8_t NUM_CHARS = 32;
inline char receivedChars[NUM_CHARS]; // requires char type for atof()
inline bool markerFlags[20] { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
inline bool newData { false };

void receiveMarkers(); // NOTE: this protocol relies on there being no random strings thrown at the board
void processReceivedData();
int identifyReceivedMarker();

void setRotationSpeed(int);
void setStepsNumber(int);
 */
#endif //_RECIEVE_H
