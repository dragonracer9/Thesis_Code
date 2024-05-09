# AutoFocus

AutoFocus is an Arduino library for simple serial communication over USB and with stepper control to interface with a focus detection script in python.

## Installation

1. Download the repository.
2. Move the downloaded folder to your Arduino libraries directory.

## Usage

Include the library in your Arduino sketch:

```cpp
#include "AutoFocus.h"
```

# Features

Stepper motor control: The library provides functions to control stepper motors. You can move the motors by a given number of steps at a set speed. The direction of movement can also be controlled. See `Stepper::move_steps` for more details.

Stepper motor initialization: The library provides a function to initialize the stepper motor pins. See `Stepper::set_pins` for more details.

Serial communication: The library provides a structure to handle packets of data for serial communication. See `packet` for more details.

# Contributing
Contributions are welcome. Please open an issue or submit a pull request.

# License
This project is licensed under the terms of the MIT license.
