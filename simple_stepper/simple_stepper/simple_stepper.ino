#include "include/simple_stepper.h"

void setup()
{
  Serial.begin(9600);
  if (init_steppers())
  {
    Serial.println("failed to init steppers");
  }
}

void loop()
{
}