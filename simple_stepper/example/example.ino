#include <SimpleStepper.h>

Stepper stepper;

void setup() {
  Serial.begin(115200);
  if (stepper.set_pins() == ret_t::SUCCESS) {
    Serial.println("Initialised Steppers!");
  }
}

void loop() {
  static packet_t packet;
  if (Serial.peek() == '~') {
    if (handle_transmission(&packet) == ret_t::SUCCESS) {
      Serial.println("Packet recieved");
    }
  }

  if (new_data) {
    if (stepper.set_speed(packet.motor, packet.speed) == ret_t::SUCCESS) {
      //__nop();
    }
  }
  new_data = false;
}