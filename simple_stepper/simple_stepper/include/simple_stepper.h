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
#include "__globals__.h"
#include "pins.h"
#include <stdint.h>

typedef enum STEPPER_RETURN {
    SUCCESS,
    WRONG_FMT,
    LOCKED,
    ERROR,
    SPICY
} ret_t;

typedef enum MOTOR_STATE {
    STEPPED,
    CONTINUUOUS,
    LOCKED,
} state_t;

typedef enum direction {
    FWD,
    BWD
} dir_t;

typedef enum AxisMoveType [[unused]] {
    NONE,
    SMALL_POS,
    SMALL_NEG,
    LARGE_POS,
    LARGE_NEG
} axis_t;

inline uint32_t stepsToMove[NR_MOTORS] { 0, 0, 0, 0, 0 };

inline constexpr float STEPS_PER_UM[NR_MOTORS] { 1, 1, 1, 1, 1 };
inline float rotation_speeds[NR_MOTORS] { 100, 100, 100, 100, 100 };

inline uint32_t half_pulse_duration_us[NR_MOTORS] { 562, 562, 562, 562, 562 };
inline uint32_t time_of_next_half_pulse[NR_MOTORS] { 0, 0, 0, 0, 0 };

inline state_t motors[NR_MOTORS] {
    state_t::LOCKED, state_t::LOCKED, state_t::LOCKED, state_t::LOCKED, state_t::LOCKED
};

inline ret_t init_steppers();
ret_t _move(uint32_t, dir_t, pin, pin);
#endif // !STP