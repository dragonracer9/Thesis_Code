#ifndef GLOBALS
#define GLOBALS
#include <stdint.h>
#define __unused__ __attribute((unused))

typedef uint8_t pin;

// return types for driver functions
typedef enum STEPPER_RETURN : int {
    SUCCESS,
    WRONG_FMT,
    WRONG_FMT0,
    WRONG_FMT1,
    WRONG_FMT2,
    LOCKED,
    ERROR,
    LITERALLY_IMPOSSIBLE,
    SPICY,
} ret_t;

typedef enum class MOTOR_STATE : int {
    // STEPPED,
    // CONTINUUOUS,

    UNBLOCKED,
    BLOCKED,
} state_t;

typedef enum class direction : int {
    FWD,
    BWD
} dir_t;

/**
 * @brief Takes uint8_t to state_t, where 0 is unblocked and 1 is blocked (other states kinda dont exist atm lol)
 *
 * @return constexpr state_t
 */
inline constexpr state_t int_to_state(uint8_t state)
{
    return static_cast<state_t>(state);
}

inline constexpr dir_t int_to_dir(uint8_t dir)
{
    return static_cast<dir_t>(dir);
}

inline constexpr char* dir_to_str(dir_t dir)
{
    if (dir == dir_t::BWD)
        return "BWD";
    else
        return "FWD";
}

#endif // GLOBALS
