#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#include "../include/simple_stepper.h"
/**
 * @brief initialises stepper pins
 *
 * @return ret_t
 */
[[nodiscard]] inline ret_t init_steppers() {
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
inline constexpr uint32_t calc_half_pulse_fact(const uint8_t u_step_fact) {
  return (uint32_t)(1000000L * 360L / 2L / steps_per_revolution / u_step_fact);
}

// half pulse factor given `microstepping_factor`
static constexpr uint32_t half_pulse_fact =
    calc_half_pulse_fact(microstepping_factor);

/**
 * @brief Set the speed of motor at motor index and sets it's half pulse
 * duration for pwm control
 *
 * @param motor_index (uint8_t)
 * @param speed  (double)
 * @return constexpr ret_t
 */
[[nodiscard]] inline constexpr ret_t
set_speed(const uint8_t motor_index, const double speed) //[[noexcept]]
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
[[nodiscard]] inline constexpr ret_t
set_steps(const uint8_t motor_index,
          const uint32_t steps) // [[noexcept]]
{
  stepsToMove[motor_index] = steps;
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
[[nodiscard]] ret_t __move(const uint32_t steps, const dir_t dir,
                           const uint8_t motorIndex,
                           const uint32_t half_pulse) {
  if (motors[motorIndex] == state_t::BLOCKED)
    return ret_t::LOCKED;

  digitalWrite(DIR_PINS[motorIndex],
               (uint8_t)dir); // Set the direction of the motor movement

  for (uint32_t i = 0; i < steps; i++) {
    digitalWrite(MOTOR_PINS[motorIndex], HIGH);
    delayMicroseconds(half_pulse);
    digitalWrite(MOTOR_PINS[motorIndex], LOW);
    delayMicroseconds(half_pulse);
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
[[nodiscard]] ret_t move_steps(const uint8_t motor_index, const uint32_t steps,
                               const dir_t dir) {
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
  return (
      uint32_t)((angle / ((double)2 * PI)) *
                (double)
                    steps_per_revolution); // i have no idea if this is correct
} // number of roations * steps per revolutions

/**
 * @brief moves motor at motor index by specified angle
 *
 * @param motor_index
 * @param angle
 * @param dir
 * @return ret_t
 */
[[nodiscard]] ret_t move_angle(const uint8_t motor_index, const double angle,
                               const dir_t dir) {
  uint32_t steps = angle_to_steps(angle);
  return __move(steps, dir, motor_index, half_pulse_duration_us[motor_index]);
}
