#include <AutoFocus.h>

Stepper stepper;


// DEBUG INPUT STRING USED : ~~1,2,3,4,5**


void setup() {
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

void loop() {
  packet_t packet;
  if (Serial.read() == '~') {
    Serial.println("Recieving");
    ret_t ret = handle_transmission(&packet);
    if (ret == ret_t::SUCCESS) {
      Serial.println("\nPacket recieved");
    } else if (ret == ret_t::WRONG_FMT0) {
      Serial.println("Wrong start");
    } else if (ret == ret_t::WRONG_FMT1) {
      Serial.println("Wrong end");
    } else if (ret == ret_t::ERROR) {
      Serial.println("ERROR");
    } else if (ret == ret_t::SPICY) {
      Serial.println("SPèICY");
    } else if (ret == ret_t::LITERALLY_IMPOSSIBLE) {
      Serial.println("IMPOSSIBLE");
    } else {
      Serial.println("No packet recieved");
    }
  }

  if (new_data) {
    if (stepper.set_state(packet.motor, int_to_state(packet.state)) != ret_t::SUCCESS
        && stepper.set_speed(packet.motor, packet.speed) != ret_t::SUCCESS) {
      // exit(1); // this is kinda very panicky lol
      Serial.println("Couldn't set state or speed");
    } else Serial.println("Sucessfully set state and speed");

    if (!(int_to_state(packet.state) == state_t::BLOCKED)) {
      Serial.println("not blocked, nice");
      packet.print();
    }
  }
  new_data = false;

  Serial.println("No Data");
  delay(2000);
}