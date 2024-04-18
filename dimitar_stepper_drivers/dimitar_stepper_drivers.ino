#define STEPS_PER_REVOLUTION 200
#define MICROSTEPPING_FACTOR 16

#define CALC_HALF_PULSE_FACTOR(MICROSTEPPING_FACT) (1000000L * 360L / 2L / STEPS_PER_REVOLUTION / MICROSTEPPING_FACT)
#define HALF_PULSE_FACTOR CALC_HALF_PULSE_FACTOR(MICROSTEPPING_FACTOR)

const float SMALL_DISTANCE = 5;  // 5um
const float LARGE_DISTANCE = 10; // 10um

enum AxisMoveType
{
    AXIS_MOVE_NONE,
    SMALL_POS,
    SMALL_NEG,
    LARGE_POS,
    LARGE_NEG
};

const byte NUM_CHARS = 32;
char receivedChars[NUM_CHARS];
boolean markerFlags[20] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
boolean newData = false;

// STEP_PIN , DIR_PIN for small CNC Shield
// X - 2,5 ; Y - 3,6 ; Z - 4,7 ; R - 12,13

// Motor pins
#define STEP_PIN_1 2
#define DIR_PIN_1 5

#define STEP_PIN_2 3
#define DIR_PIN_2 6

#define STEP_PIN_3 4
#define DIR_PIN_3 7

#define STEP_PIN_4 12
#define DIR_PIN_4 13

#define STEP_PIN_5 34
#define DIR_PIN_5 36

const int NUM_MOTORS = 5;
const int STEP_PINS[NUM_MOTORS] = {STEP_PIN_1, STEP_PIN_2, STEP_PIN_3, STEP_PIN_4, STEP_PIN_5};
const int DIR_PINS[NUM_MOTORS] = {DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5};
float speedRotation[NUM_MOTORS] = {100, 100, 100, 100, 100};
long half_pulse_duration_us[NUM_MOTORS] = {562, 562, 562, 562, 562};
unsigned long time_of_next_half_pulse[NUM_MOTORS] = {0, 0, 0, 0, 0};
bool rotationEnabled[NUM_MOTORS] = {false, false, false, false, false};
bool steppedEnabled[NUM_MOTORS] = {false, false, false, false, false};
const float STEPS_PER_UM[NUM_MOTORS] = {1, 1, 1, 1, 1};
int stepsToMove[NUM_MOTORS] = {0, 0, 0, 0, 0}; // Add this global variable to store steps for each motor.

AxisMoveType AxisMove[NUM_MOTORS] = {AXIS_MOVE_NONE, AXIS_MOVE_NONE, AXIS_MOVE_NONE, AXIS_MOVE_NONE, AXIS_MOVE_NONE};

void setup()
{
    Serial.begin(115200);
    pinMode(STEP_PIN_1, OUTPUT);
    pinMode(DIR_PIN_1, OUTPUT);

    pinMode(STEP_PIN_2, OUTPUT);
    pinMode(DIR_PIN_2, OUTPUT);

    pinMode(STEP_PIN_3, OUTPUT);
    pinMode(DIR_PIN_3, OUTPUT);

    pinMode(STEP_PIN_4, OUTPUT);
    pinMode(DIR_PIN_4, OUTPUT);

    pinMode(STEP_PIN_5, OUTPUT);
    pinMode(DIR_PIN_5, OUTPUT);
}

void loop()
{
    receiveMarkers();
    processReceivedData();
    for (int i = 0; i < NUM_MOTORS; i++)
    {
        handleContinuousRotation(i);
    }
}

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

void setRotationSpeed(int motorIndex)
{
    speedRotation[motorIndex] = atof(receivedChars);
    half_pulse_duration_us[motorIndex] = HALF_PULSE_FACTOR / speedRotation[motorIndex];
    time_of_next_half_pulse[motorIndex] = micros();
    Serial.print("Motor ");
    Serial.print(motorIndex + 1);
    Serial.println(" rotational speed is [deg/sec]: ");
    Serial.println(speedRotation[motorIndex]);
}

void setStepsNumber(int motorIndex)
{
    //    int motorIndex = identifyReceivedMarker() / 4; // Example to get motorIndex if needed.
    stepsToMove[motorIndex] = atof(receivedChars); // convert the array to integer
    Serial.print("Motor ");
    Serial.print(motorIndex + 1);
    Serial.print(" steps to execute: ");
    Serial.println(stepsToMove[motorIndex]);
}

void handleSteppedRotation(int motorIndex)
{
    switch (receivedChars[0])
    {
    case '0': // Assume '0' means stop or do nothing
        // Implement stopping the motor or do nothing, as per your application
        Serial.print("Motor ");
        Serial.print(motorIndex + 1);
        Serial.println(" not moving.");
        steppedEnabled[motorIndex] = false;
        break;
    case '1': // Move motor forward
        steppedEnabled[motorIndex] = true;
        moveMotor(stepsToMove[motorIndex], STEP_PINS[motorIndex], DIR_PINS[motorIndex], LOW, motorIndex, half_pulse_duration_us[motorIndex]);
        break;
    case '2': // Move motor backward
        steppedEnabled[motorIndex] = true;
        moveMotor(stepsToMove[motorIndex], STEP_PINS[motorIndex], DIR_PINS[motorIndex], HIGH, motorIndex, half_pulse_duration_us[motorIndex]);
        break;
    default: // Handle unexpected values
        Serial.print("Error: Unexpected move direction received for motor ");
        Serial.println(motorIndex + 1);
        break;
    }
}

void handleContinuousRotationMarker(int motorIndex)
{
    switch (receivedChars[0])
    {
    case '0':
        rotationEnabled[motorIndex] = false;
        break;
    case '1':
        rotationEnabled[motorIndex] = true;
        digitalWrite(DIR_PINS[motorIndex], LOW);
        break;
    case '2':
        rotationEnabled[motorIndex] = true;
        digitalWrite(DIR_PINS[motorIndex], HIGH);
        break;
    default:
        Serial.println("Error: Unexpected value for rot");
        break;
    }
}

void handleContinuousRotation(int motorIndex)
{
    if (!rotationEnabled[motorIndex])
        return;
    if (micros() >= time_of_next_half_pulse[motorIndex])
    {
        digitalWrite(STEP_PINS[motorIndex], !digitalRead(STEP_PINS[motorIndex]));
        time_of_next_half_pulse[motorIndex] = micros() + half_pulse_duration_us[motorIndex];
    }
}

void moveMotor(int steps, int stepPin, int dirPin, int direction, int motorIndex, unsigned long halfPulseDuration)
{
    digitalWrite(dirPin, direction); // Set the direction of the motor movement
    for (int i = 0; i < steps; i++)
    {
        while (steppedEnabled[motorIndex])
        {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(halfPulseDuration);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(halfPulseDuration);
        }
    }
}
