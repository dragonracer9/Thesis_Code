#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#include "simple_stepper.h"


motor_t motors[NR_MOTORS];
memset();

ret_t move(uint32_t steps, dir_t dir, pin motorPin, pin dirPin)
{
    if (motors[motorPin] == motor_t::LOCKED) return ret_t::LOCKED;
    digitalWrite(dirPin, dir); // Set the direction of the motor movement
    return ret_t::SUCCESS;
}