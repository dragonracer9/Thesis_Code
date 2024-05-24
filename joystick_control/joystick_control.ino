#define VRX_PIN A0
#define VRY_PIN A1

int xValue = 0;
int yValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    xValue = analogRead(VRX_PIN);
    yValue = analogRead(VRY_PIN);

    Serial.print("x = ");
    Serial.print(xValue);
    Serial.print(", y = ");
    Serial.println(yValue);

    /// if the joystic is in the middle == = > stop the motor
    if ((val > 500) && (val < 523)) { // FIXME: adapt to motor shield
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    } else {
        // move the motor in the first direction
        while (val >= 523) {
            // map the speed between 5 and 500 rpm
            int speed_ = map(val, 523, 1023, 5, 500); 
            // set motor speed
            stepper.setSpeed(speed_); //TODO: check if set speed does anything really???

            // move the motor (1 step)
            stepper.step(1);

            val = analogRead(joystick);
        }

        // move the motor in the other direction
        while (val <= 500) {
            // map the speed between 5 and 500 rpm
            int speed_ = map(val, 500, 0, 5, 500);
            // set motor speed
            stepper.setSpeed(speed_);

            // move the motor (1 step)
            stepper.step(-1);

            val = analogRead(joystick);
        }
    }
}
