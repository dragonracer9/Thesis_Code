#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#include "simple_stepper.h"

inline ret_t init_steppers()
{
    for (uint8_t i = 0; i < NR_MOTORS; i++) {
        pinMode(motorPins[i], OUTPUT);
        pinMode(dirPins[i], OUTPUT);
        /* digitalWrite(motorPins[i], LOW);
        digitalWrite(dirPins[i], LOW); */  //TODO: determine if this is necessary
    } // Set the motor pins as outputs
    // since all motors are locked, we can set the enable pin to low
    return ret_t::SUCCESS;
}


ret_t _move(uint32_t steps, dir_t dir, pin motorPin, pin dirPin)
{
    if (motors[motorPin] == state_t::LOCKED)
        return ret_t::LOCKED;
    digitalWrite(dirPin, dir); // Set the direction of the motor movement

    for (uint32_t i = 0; i < steps; i++) {
        digitalWrite(motorPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(motorPin, LOW);
        delayMicroseconds(500);
    }
    return ret_t::SUCCESS;
}