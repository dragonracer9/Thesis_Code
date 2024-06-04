// this project is made by BEASTIDREES62 https://id.arduino.cc/?code=MTxqeHweG6vL2cur&state=amxEcTB2bnNiYjluTUxmRExWaWZnOXIzUk1BLWRqZTZYUGtJNGtxODB%2Bdg%3D%3D

// include Arduino stepper motor library
#include <AutoFocus.h>

// initialize stepper library
Stepper stepper;

#define VRX_PIN A0 // Arduino pin connected to VRX pin
#define VRY_PIN A1 // Arduino pin connected to VRY pin

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("-----------------------------------------------------------\nBOOTED\n-----------------------------------------------------------\n\n\n");
    delay(500);
    if (stepper.set_pins() == ret_t::SUCCESS) {
        Serial.println("Initialised Steppers!");
        Serial.flush();
        delay(3000);
    } else {
        Serial.println("Steppers failed to init :(");
        Serial.flush();
        delay(2000);
        Serial.println("Ideally i'd try again, but like, i'm lazy");
        Serial.flush();
    }
}

void loop()
{
    //  read analog value from the potentiometer
    xValue = analogRead(VRX_PIN);
    yValue = analogRead(VRY_PIN);

    // print data to Serial Monitor on Arduino IDE
    Serial.print("x = ");
    Serial.print(xValue);
    Serial.print(", y = ");
    Serial.println(yValue);
    delay(10);
    // TODO chech following logic
    // if the joystic is in the middle ===> stop the motor
    if ((xValue > 490) && (xValue < 505)) {
        stepper.set_speed(0, 0);
        stepper.set_state(0, state_t::BLOCKED);
    }

    else {
      stepper.set_state(0, state_t::UNBLOCKED);
        // move the motor in the first direction
        while (xValue >= 505) {
            // map the speed between 5 and 500 rpm
            int speed_ = map(xValue, 523, 1023, 5, 500);
            // set motor speed
            stepper.set_speed(0, speed_);
            // move the motor (1 step)
            stepper.move_steps(0, 1, dir_t::FWD);

            xValue = analogRead(VRX_PIN);
        }

        // move the motor in the other direction
        while (xValue <= 490) {
            // map the speed between 5 and 500 rpm
            int speed_ = map(xValue, 500, 0, 5, 500);
            // set motor speed
            stepper.set_speed(0, speed_);
            // move the motor (1 step)
            stepper.move_steps(0, 1, dir_t::BWD);

            xValue = analogRead(VRX_PIN);
        }
    }
}