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

## Features

Stepper motor control: The library provides functions to control stepper motors. You can move the motors by a given number of steps at a set speed. The direction of movement can also be controlled. See `Stepper::move_steps` for more details.

Stepper motor initialization: The library provides a function to initialize the stepper motor pins. See `Stepper::set_pins` for more details.

Serial communication: The library provides a structure to handle packets of data for serial communication. See `packet` for more details.

## A Brief Intro to Cpp17+ (specifically the stuff used here)

As the idea of this library was to improve readability and maintainability of the communications protocol written by Dimitar and
wrap the stepper controls in nice functions to abstract away the actual calculations, it is probably a good idea to explain the
newer concepts used in this library for the uninitiated.

```cpp
constexpr // keyword telling the compiler that this is a value that if possible should be calculated at compile time instead of during runtime of the program

inline // in functions, this tells the compiler to simply copy paste the content of the function to where it is called
       // in Cpp23+, inline may be used in a variable declaration in a header file to indicate that this value should only exist once
       /*BRIEF EXPLANATION OF VARIABLE DECLARATIONS IN HEADER FILES
       In cpp, by default a variable declared in a header file is instantiated in every .cpp file that includes that header. This causes there to be multiple copies of the same variable with potentially different values. In the case of a constant or constexpr value, this means that there will be multiple copies of the constant value throughout the program. To prevent this, a variable can be declared inline, meaning it will only be created once and is the same over all files accessing it. This allows for reduction in memory usage and also sharing data over files.*/

      // For those who want to pedant me, yes, inline has been a thing since before cpp17. The use in variable definitions is new, however.

uint8_t // this is an unsigned int of 8 bits
// similarly for uint16_t, etc.
// the number indicates the number of bits
// the reason to use these is because regardless of platform, you are guaranteed to have the right number of bits instead of being reliant of knowing the size of each type yourself.

static_cast<type>(pointer_type) // safe type cast from one type to another. (Also constexpr since cpp20). A traditional C-style cast will not show whether a cast is possible or not and will simply convert. With static_cast, you can be confident the cast is possible and will be sucessful.

[[nodiscard]]  // tells the compiler to check if the return value of a function is used. If the return value is discarded, throw a compilation error or warning (depends on compiler settings).

noexcept // tells the compiler that it can more agressively optimise a function, because we guarantee that function will not throw any exceptions.
```

Helpful links:
1. [static cast](https://stackoverflow.com/questions/103512/why-use-static-casttx-instead-of-tx)

### Variable initialisation:
to avoid unwanted side effects that traditional initialisation can have (this is a very complicated topic and I will not go further into this), I have opted to initialise variables where possible the following way:

```cpp
type varable_name {value};
```

where the value in the `{}` is the value the variable is intitialised with.

### Other Cpp things that aren't new, but may be new to whoever wants to use this library

1. The use of checked return codes is prevalent throughout this library. `ret_t::
*` indicates any possible point of failure and makes debugging significantly nicer.

2. ```cpp
    auto get_next_token = [&token](char* const buffer) -> ret_t {...};
    ```

    from `recieve.cpp` is a lambda function. This is an anonymous function that can be declared anywhere. Things in the `[]` are "captured", i.e. they are taken as parameters from the surrounding context in which the function is called. This is a bit like taking a reference as an argument, but it is the actual object that is captured, not just a reference. Things in the `()` are taken as arguments justlike in a regular fuction. The function must be declared `auto` and the return type of the lambda is given by `-> return type` at the end of the definition. The function body is then in the following `{}`.

3. ```cpp
    typedef
    ```

    Declares a custom type. Can be used to rename types (e.g. `pin`) and make aliases for type names. A good example of this is the packet type:

    ```cpp
    typedef struct packet {...
    } packet_t; // packet_t is now a type specifier for the struct packet (*_t indicates "type")
    ```

4. A variable declared `static` is one of two things. Declared outside of a function, `static` means a variable is only accessible in that file. Declared in a function scope, the variable retains its value over function calls.

## Contributing

Contributions are welcome. Please open an issue or submit a pull request.

## License

This project is licensed under the terms of the MIT license.
