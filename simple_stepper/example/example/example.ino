#include <AutoFocus.h>

Stepper stepper;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    if (stepper.set_pins() == ret_t::SUCCESS) {
        Serial.println("Initialised Steppers!");
    }
}

void loop()
{
    static packet_t packet;
    if (Serial.read() == '~') {
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
        ret_t did_move = stepper.move_steps(packet.motor, packet.steps, int_to_dir(packet.direction));
        send_end_flag(); //  to tell the python script that we completed the move
        if (!(did_move == ret_t::SUCCESS)) {
            Serial.println("Idk man, didnt do the thing... it's like locked or something");
        }
    }
    new_data = false;
}