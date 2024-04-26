#include "receive.h"
#include <typeinfo>
#include <vector>

ret_t handle_transmission(packet_t* const motor) // remember to use Serial.peek() to ckeck to run transmission before calling this ig ?
{
    uint8_t packet[5];
    ret_t ret = recieve_packet(packet);
    if (ret != ret_t::SUCCESS)
        return ret;
    ret = parse_packet(packet, motor);
    if (ret != ret_t::SUCCESS)
        return ret;
    new_data = true;
    return ret_t::SUCCESS;
}

ret_t recieve_packet(uint8_t* const packet)
{
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '~') {
            // we have a packet

            // read the packet
            for (uint8_t i { 0 }; i < 5; i++) {
                if (Serial.available()) {
                    packet[i] = (uint8_t)Serial.read();
                } else {
                    return ret_t::SPICY; // not enough data or something went wrong. the first is user error, the second is just weird
                }
            }
            // check the end byte
            if (Serial.available()) {
                char end = Serial.read();
                if (end == '*') {
                    return ret_t::SUCCESS;
                } else {
                    return ret_t::WRONG_FMT; // no end byte or wrong end byte
                }
            } else {
                return ret_t::SPICY; // cant read last byte???
            }

        } else { // wrong start byte
            return ret_t::WRONG_FMT;
        }
    } else {
        return ret_t::ERROR;
    }
    // this should never happen but just in case
    return ret_t::LITERALLY_IMPOSSIBLE; // actually, if it does reach this, I'm going to be very impressed
} // it turns out if you're consistent with the return value thing, it gets quite full lol

ret_t parse_packet(uint8_t* const arr, packet_t* const packet)
{
    packet->motor = arr[0];
    packet->state = arr[1];
    packet->direction = arr[2];
    packet->steps = arr[3];
    packet->speed = arr[4];
    /*
    TODO:if there is such a thing as introspection, check if these are actually integral types and stuff
    I suppose it just cant fail otherwise lol
    i think as far as safety goes, integrity of format is checked, simply the actual content  is not chackable for me here
    it's actually quite annoying the more i think about it.
    */
    return ret_t::SUCCESS;
}

//
//
/****************************************************
 * DIMITAR'S CODE ((START))
 * DO NOT TOUCH
 * **DEPRECATED**
 ***************************************************/
/* 
void receiveMarkers() // NOTE: this protocol relies on there being no random strings thrown at the board
{
    static bool receivingInProgress[20] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
    static uint8_t index = 0;
    char startMarkers[20] = { '<', '[', '(', '{', '@', '-', '$', 'A', 'C', 'E', 'G', 'I', 'K', 'M', 'O', 'Q', 'S', 'U', 'W', 'Y' };
    char endMarkers[20] = { '>', ']', ')', '}', '!', '%', '&', 'B', 'D', 'F', 'H', 'J', 'L', 'N', 'P', 'R', 'T', 'V', 'X', 'Z' };
    char data;

    while (Serial.available() > 0 && !newData) {
        data = Serial.read();

        for (int i = 0; i < 20; i++) {
            if (receivingInProgress[i]) {
                if (data != endMarkers[i]) {
                    receivedChars[index++] = data;
                    if (index >= NUM_CHARS)
                        index = NUM_CHARS - 1;
                } else {
                    receivedChars[index] = '\0'; // Terminate the string
                    receivingInProgress[i] = false;
                    index = 0;
                    newData = true;
                }
            } else if (data == startMarkers[i]) {
                memset(markerFlags, false, sizeof(markerFlags));
                markerFlags[i] = true;
                receivingInProgress[i] = true;
            }
        }
    }
}

void processReceivedData()
{
    if (!newData)
        return;
    int markerIndex = identifyReceivedMarker();
    int motorIndex = markerIndex / 4; // BECAUSE OF INTEGER DIVISION
    //    Serial.println("Identified motor index: ");
    //    Serial.print(motorIndex);
    //    Serial.println(" ");
    int markerType = markerIndex % 4;
    //    Serial.println("Identified marker type: ");
    //    Serial.print(markerType);
    //    Serial.println(" ");

    if (motorIndex >= 0 && motorIndex < NR_MOTORS) {
        switch (markerType) {
        case 0:
            // TODO
            // handleContinuousRotationMarker(motorIndex); // FIXME: get these implemented or something
            break;
        case 1:
            setRotationSpeed(motorIndex);
            break;
        case 2:
            setStepsNumber(motorIndex);
            break;
        case 3:
            // TODO
            // handleSteppedRotation(motorIndex);
            break;
        }
    } else {
        // Handle error or unexpected marker index
        Serial.println("Error: Unexpected marker index");
    }

    newData = false;
}

int identifyReceivedMarker()
{
    for (int i = 0; i < 20; i++) {
        if (markerFlags[i])
            return i;
    }
    return -1; // Marker not found
}

void setRotationSpeed(int motorIndex)
{
    dimitar_speedRotation[motorIndex] = atof(receivedChars);
    dimitar_half_pulse_duration_us[motorIndex] = HALF_PULSE_FACTOR / dimitar_speedRotation[motorIndex];
    dimitar_time_of_next_half_pulse[motorIndex] = micros();
    Serial.print("Motor ");
    Serial.print(motorIndex + 1);
    Serial.println(" rotational speed is [deg/sec]: ");
    Serial.println(dimitar_speedRotation[motorIndex]);
}

void setStepsNumber(int motorIndex)
{
    //    int motorIndex = identifyReceivedMarker() / 4; // Example to get motorIndex if needed.
    dimitar_stepsToMove[motorIndex] = atof(receivedChars); // convert the array to integer
    Serial.print("Motor ");
    Serial.print(motorIndex + 1);
    Serial.print(" steps to execute: ");
    Serial.println(dimitar_stepsToMove[motorIndex]);
} */