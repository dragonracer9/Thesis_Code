#if !defined(PINS)
#define PINS

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#include <stdint.h>

#define STEP_X 2
#define DIR_X 5

#define STEP_Y 3
#define DIR_Y 6

#define STEP_Z 4
#define DIR_Z 7

#define STEP_PIN_4 12
#define DIR_PIN_4 13

#define STEP_PIN_5 34 // ???
#define DIR_PIN_5 36  // ???

#define ENABLE 8;

inline constexpr uint8_t NR_MOTORS{5}; // number of motors

inline constexpr uint8_t STEP_PINS[NR_MOTORS]{STEP_X, STEP_Y, STEP_Z, STEP_PIN_4, STEP_PIN_5};
inline constexpr uint8_t DIR_PINS[NR_MOTORS]{DIR_X, DIR_Y, DIR_Z, DIR_PIN_4, DIR_PIN_5};

#endif