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
#include "receive.h"
#include <stdint.h>

#define sign(x) (((x) > 0) - ((x) < 0)) // nice to have, return values {-1, 0, 1}
#undef abs
#define abs(x) ((x) * sign((x))) // better (branchless)

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

inline constexpr uint16_t steps_per_revolution { 200 };
inline constexpr uint8_t microstepping_factor { 16 };
inline constexpr float STEPS_PER_UM[NR_MOTORS] { 1, 1, 1, 1, 1 };

inline constexpr uint32_t angle_to_steps(double);

class Stepper {
public:
    constexpr Stepper(void) noexcept
        : motors { state_t::BLOCKED, state_t::BLOCKED, state_t::BLOCKED,
            state_t::BLOCKED, state_t::BLOCKED }
        , rotation_speeds { 100, 100, 100, 100, 100 }
        , stepsToMove { 0, 0, 0, 0, 0 }
        , half_pulse_duration_us { 562, 562, 562, 562, 562 }
    {
    }
    /* constexpr Stepper(Stepper&&) = delete; // dont really need move semantics
    constexpr Stepper(const Stepper&) = delete;
    constexpr Stepper& operator=(Stepper&&) = delete;
    constexpr Stepper& operator=(const Stepper&) = delete; */
    ~Stepper() = default;

    ret_t init_steppers(void) const noexcept;
    inline constexpr ret_t set_speed(const uint8_t, const double) noexcept;
    inline constexpr ret_t set_steps(const uint8_t, const uint32_t) noexcept;
    ret_t __move(const uint32_t, const dir_t, const uint8_t, const uint32_t) noexcept;

    ret_t move_steps(uint8_t, uint32_t, dir_t) noexcept;
    ret_t move_angle(uint8_t, double, dir_t) noexcept;
    // ret_t continuous_rotation(int8_t, dir_t);
private:
    state_t motors[NR_MOTORS];

    float rotation_speeds[NR_MOTORS];
    uint32_t stepsToMove[NR_MOTORS];
    inline constexpr uint32_t calc_half_pulse_fact(const uint8_t);

    uint32_t half_pulse_duration_us[NR_MOTORS];
};

typedef Stepper stp_t;
#endif // !STP
