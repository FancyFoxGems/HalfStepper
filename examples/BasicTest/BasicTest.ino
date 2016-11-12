/***********************************************************************************************
* This file is part of the HalfStepper Arduino library.                                        *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.             *
* RELEASED UNDER THE LGPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
************************************************************************************************/

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
