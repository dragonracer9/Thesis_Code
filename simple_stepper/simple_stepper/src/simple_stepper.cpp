#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#include "../include/simple_stepper.h"
/**
 * @brief initialises stepper pins
 *
 * @return ret_t
 */
inline ret_t [[nodiscard]] init_steppers()
{
    for (uint8_t i = 0; i < NR_MOTORS; i++) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        pinMode(DIR_PINS[i], OUTPUT);
        /* digitalWrite(motorPins[i], LOW);
        digitalWrite(dirPins[i], LOW); */
        // TODO: determine if this is necessary
    } // Set the motor pins as outputs
    // since all motors are locked, we can set the enable pin to low
    return ret_t::SUCCESS;
}

/**
 * @brief calculates half pulse factor of pwm to set correct speed for motors
 *
 * @param u_step_fact
 * @return constexpr uint32_t
 */
inline constexpr uint32_t [[nodiscard]] calc_half_pulse_fact(uint8_t u_step_fact)
{
    return (uint32_t)(1000000L * 360L / 2L / steps_per_revolution / u_step_fact);
}

// half pulse factor given `microstepping_factor`
static constexpr uint32_t half_pulse_fact = calc_half_pulse_fact(microstepping_factor);

/**
 * @brief Set the speed of motor at motor index and sets it's half pulse duration for pwm control
 *
 * @param motor_index (uint8_t)
 * @param speed  (double)
 * @return constexpr ret_t
 */
inline constexpr ret_t [[nodiscard]] set_speed(uint8_t motor_index, double speed) //[[noexcept]]
{
    rotation_speeds[motor_index] = speed;
    half_pulse_duration_us[motor_index] = half_pulse_fact / speed;
    // TODO
    return ret_t::SUCCESS;
}

/**
 * @brief Set the number of steps the motor at motor index should move
 * 
 * @param motor_index 
 * @param steps 
 * @return constexpr ret_t 
 */
inline constexpr ret_t [[nodiscard]] set_steps(uint8_t motor_index, uint32_t steps)
{
    stepsToMove[motor_index] = steps;
}

/**
 * @brief moves motor at motor index for a set amount of steps
 *
 * @param steps
 * @param dir
 * @param motorIndex
 * @param dirPin
 * @param half_pulse
 * @return ret_t
 */
ret_t [[nodiscard]] _move(uint32_t steps, dir_t dir, uint8_t motorIndex, pin dirPin, uint32_t half_pulse)
{
    if (motors[motorIndex] == state_t::BLOCKED)
        return ret_t::LOCKED;
    digitalWrite(dirPin, (uint8_t)dir); // Set the direction of the motor movement

    for (uint32_t i = 0; i < steps; i++) {
        digitalWrite(MOTOR_PINS[motorIndex], HIGH);
        delayMicroseconds(half_pulse);
        digitalWrite(MOTOR_PINS[motorIndex], LOW);
        delayMicroseconds(half_pulse);
    }
    return ret_t::SUCCESS;
}