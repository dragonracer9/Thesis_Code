#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include "SimpleStepper.h"


/**
 * @brief initialises stepper pins
 *
 * @return ret_t
 */
ret_t Stepper::set_pins(void) const //[[noexcept]]
{
    for (uint8_t i = 0; i < NR_MOTORS; i++) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        pinMode(DIR_PINS[i], OUTPUT);
        digitalWrite(MOTOR_PINS[i], LOW); // hopefully this prevents jittering
        /* digitalWrite(dirPins[i], LOW); */
        // TODO: determine if this is necessary
    } // Set the motor pins as outputs
    // since all motors are locked, we can set the enable pin to low
    return ret_t::SUCCESS;
}

ret_t Stepper::set_state(const uint8_t motor_index, const state_t state)
{
    motors[motor_index] = state;
    return ret_t::SUCCESS;
}

// half pulse factor given `microstepping_factor`
static constexpr uint32_t half_pulse_fact = calc_half_pulse_fact(microstepping_factor);

/**
 * @brief Set the speed of motor at motor index and sets it's half pulse
 * duration for pwm control
 *
 * @param motor_index (uint8_t)
 * @param speed  (double)
 * @return ret_t
 */
ret_t Stepper::set_speed(const uint8_t motor_index, const uint8_t speed) noexcept //[[noexcept]]
{
    rotation_speeds[motor_index] = speed;
    half_pulse_duration_us[motor_index] = (uint32_t)(half_pulse_fact / (float)speed);
    // TODO
    return ret_t::SUCCESS;
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
[[nodiscard]] ret_t Stepper::__move(const uint32_t steps, const dir_t dir,
    const uint8_t motorIndex,
    const uint32_t half_pulse)
{
    if (motors[motorIndex] == state_t::BLOCKED)
        return ret_t::LOCKED;

    digitalWrite(DIR_PINS[motorIndex],
        (uint8_t)dir); // Set the direction of the motor movement

    for (uint32_t i = 0; i < steps; i++) {
        digitalWrite(MOTOR_PINS[motorIndex], HIGH);
        delayMicroseconds(half_pulse);
        digitalWrite(MOTOR_PINS[motorIndex], LOW);
        delayMicroseconds(half_pulse); // this runs 
    }
    return ret_t::SUCCESS;
}

/**
 * @brief Moves motor at motor index by given number of steps. Speed can be set
 * in `set_speed`
 *
 * @param motor_index
 * @param steps
 * @param dir
 * @return ret_t
 */
[[nodiscard]] ret_t Stepper::move_steps(const uint8_t motor_index,
    const uint32_t steps, const dir_t dir)
{
    return __move(steps, dir, motor_index, half_pulse_duration_us[motor_index]);
}

/**
 * @brief
 *
 * @param angle - (double) the angle to move, in [rad]
 * @return constexpr uint32_t
 */
[[nodiscard]] inline constexpr uint32_t
angle_to_steps(double angle) // FIXME: tune to correct values
{
    return (uint32_t)((angle / ((double)2 * PI)) * (double)steps_per_revolution); // i have no idea if this is correct
} // number of roations * steps per revolutions

/**
 * @brief moves motor at motor index by specified angle
 *
 * @param motor_index
 * @param angle
 * @param dir
 * @return ret_t
 */
[[nodiscard]] ret_t Stepper::move_angle(const uint8_t motor_index,
    const double angle, const dir_t dir)
{
    uint32_t steps = angle_to_steps(angle);
    return __move(steps, dir, motor_index, half_pulse_duration_us[motor_index]);
}
