/**
 * @file simple_stepper.h
 * @author Vikram Damani (vdamani@ethz.ch)
 * @brief Simple generic stepper motor driver
 * @version 0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#if !defined(STP)
#define STP

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include <stdint.h>
#include "pins.h"


typedef enum STEPPER_RETURN {
    SUCCESS,
    WRONG_FMT,
    LOCKED,
    ERROR,
    SPICY
} ret_t;

typedef enum MOTOR_STATE {
    FREE,
    LOCKED,
} motor_t;

typedef enum direction {
    FWD,
    BWD
} dir_t;

typedef uint8_t pin;


ret_t  move(uint32_t, dir_t, pin, pin);
#endif // !STP