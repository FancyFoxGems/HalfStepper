#include "HalfStepper.h"

HalfStepper motor(200, 9, 11, 4, 5);

void setup() { }

void loop()
{
  motor.step(1);
  delay(500);
}


