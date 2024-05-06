#ifndef GLOBALS
#define GLOBALS
#include <stdint.h>
#define __unused__ __attribute((unused))

typedef uint8_t pin;

// return types for driver functions
typedef enum STEPPER_RETURN {
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

#endif // GLOBALS
