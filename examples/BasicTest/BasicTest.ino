#include "HalfStepper.h"

HalfStepper _Motor(200, 7, 6, 5, 4);

static const byte PIN_LED = 13;

bool _LedState = LOW;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
}

void loop()
{
  _LedState = !_LedState; 
  digitalWrite(PIN_LED, _LedState);
  
  _Motor.step(10);
  
  delay(500);
}


