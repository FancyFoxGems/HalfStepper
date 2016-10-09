#include "HalfStepper.h"

HalfStepper motor(200, 7, 6, 5, 4);

void setup() { }

void loop()
{
  motor.step(10);
  delay(500);
}


