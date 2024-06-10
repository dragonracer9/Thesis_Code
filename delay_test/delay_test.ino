#include<AutoFocus.h>

Stepper stepper;

void setup() {
    Serial.begin(9600);
    stepper.set_pins();
    stepper.set_speed(0,10);
    stepper.set_state(0, state_t::UNBLOCKED);
}

void loop() {
    stepper.move_steps(0, 1000, dir_t::FWD);
    stepper.move_steps(0, 1000, dir_t::BWD);
    delay(500);
}