#include "recieve.h"


/**
 * @brief Recieve a message from the serial port
 * USES DIMITAR'S CODE
 * 
 */
void receiveMarkers() // NOTE: this protocol relies on there being no random strings thrown at the board
{
    static bool receivingInProgress[20] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    static byte index = 0;
    char startMarkers[20] = {'<', '[', '(', '{', '@', '-', '$', 'A', 'C', 'E', 'G', 'I', 'K', 'M', 'O', 'Q', 'S', 'U', 'W', 'Y'};
    char endMarkers[20] = {'>', ']', ')', '}', '!', '%', '&', 'B', 'D', 'F', 'H', 'J', 'L', 'N', 'P', 'R', 'T', 'V', 'X', 'Z'};
    char data;

    while (Serial.available() > 0 && !newData)
    {
        data = Serial.read();

        for (int i = 0; i < 20; i++)
        {
            if (receivingInProgress[i])
            {
                if (data != endMarkers[i])
                {
                    receivedChars[index++] = data;
                    if (index >= NUM_CHARS)
                        index = NUM_CHARS - 1;
                }
                else
                {
                    receivedChars[index] = '\0'; // Terminate the string
                    receivingInProgress[i] = false;
                    index = 0;
                    newData = true;
                }
            }
            else if (data == startMarkers[i])
            {
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
    int motorIndex = markerIndex / 4;
    //    Serial.println("Identified motor index: ");
    //    Serial.print(motorIndex);
    //    Serial.println(" ");
    int markerType = markerIndex % 4;
    //    Serial.println("Identified marker type: ");
    //    Serial.print(markerType);
    //    Serial.println(" ");

    if (motorIndex >= 0 && motorIndex < NUM_MOTORS)
    {
        switch (markerType)
        {
        case 0:
            handleContinuousRotationMarker(motorIndex);
            break;
        case 1:
            setRotationSpeed(motorIndex);
            break;
        case 2:
            setStepsNumber(motorIndex);
            break;
        case 3:
            handleSteppedRotation(motorIndex);
            break;
        }
    }
    else
    {
        // Handle error or unexpected marker index
        Serial.println("Error: Unexpected marker index");
    }

    newData = false;
}

int identifyReceivedMarker()
{
    for (int i = 0; i < 20; i++)
    {
        if (markerFlags[i])
            return i;
    }
    return -1; // Marker not found
}