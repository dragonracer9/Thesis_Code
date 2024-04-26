#include <AutoFocus.h>

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
    if (stepper.set_state(packet.motor, int_to_state(packet.state)) != ret_t::SUCCESS
        && stepper.set_speed(packet.motor, packet.speed) != ret_t::SUCCESS) {
      // exit(1); // this is kinda very panicky lol
      Serial.println("Couldn't set state or speed");
    }
  }
  if (!(int_to_state(packet.state) == state_t::BLOCKED)) {
    char buffer[32];
    Serial.println(packet.to_string(buffer));
  }
  new_data = false;
}