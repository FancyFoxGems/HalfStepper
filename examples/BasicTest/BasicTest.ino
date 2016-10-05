#include "HalfStepper.h"

HalfStepper motor(200, 9, 11, 4, 5);

void setup() { Serial.begin(115200); }

void loop()
{
  motor.step(1);
  delay(500);
}

