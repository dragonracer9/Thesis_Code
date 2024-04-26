/**
 * @file AutoFocus.h
 * @author Vikram Damani (vdamani@ethz.ch)
 * @brief Arduino lib for simple autofocus sketch including serial communication and stepper control
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */
#if !defined(AUTOFOCUS)
#define AUTOFOCUS

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include "include/__globals__.h"
#include "include/pins.h"
#include "core/SimpleStepper.h"
#include "utility/receive.h"
#include <stdint.h>

#ifndef STP
#warning "STP not defined"
#endif // !STP
#ifndef _RECIEVE_H
#warning "_RECIEVE_H not defined"
#endif // !_RECIEVE_H
#ifndef GLOBALS
#warning "GLOBALS not defined"
#endif // !GLOBALS
#ifndef PINS
#warning "PINS not defined"
#endif // !PINS


#endif