/**
 * @file simple_stepper.h
 * @author Vikram Damani (vdamani@ethz.ch)
 * @brief Simple stepper motor driver (definitely not generic any more though)
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

typedef enum MOTOR_STATE {
    STEPPED,
    CONTINUUOUS,
    BLOCKED,
} state_t;

typedef enum direction {
    FWD,
    BWD
} dir_t;

// [[unused]]
/* typedef enum AxisMoveType {
    NONE,
    SMALL_POS,
    SMALL_NEG,
    LARGE_POS,
    LARGE_NEG
} axis_t; */

inline constexpr uint16_t steps_per_revolution {200};
inline constexpr uint8_t microstepping_factor {16};
inline constexpr uint32_t calc_half_pulse_fact(uint8_t);


inline constexpr float STEPS_PER_UM[NR_MOTORS] { 1, 1, 1, 1, 1 };
inline float rotation_speeds[NR_MOTORS] { 100, 100, 100, 100, 100 };

inline uint32_t half_pulse_duration_us[NR_MOTORS] { 562, 562, 562, 562, 562 };
inline uint32_t time_of_next_half_pulse[NR_MOTORS] { 0, 0, 0, 0, 0 };

inline uint32_t stepsToMove[NR_MOTORS] { 0, 0, 0, 0, 0 };

inline state_t motors[NR_MOTORS] {
    state_t::BLOCKED, state_t::BLOCKED, state_t::BLOCKED, state_t::BLOCKED, state_t::BLOCKED
};

inline ret_t [[nodiscard]] init_steppers();
inline constexpr ret_t [[nodiscard]] set_speed(uint8_t, double);
inline constexpr ret_t [[nodiscard]] set_steps(uint8_t, uint32_t);
ret_t [[nodiscard]] _move(uint32_t, dir_t, pin, pin);

ret_t move_steps() ;
ret_t continuous_rotation();
ret_t move_angle();
#endif // !STP