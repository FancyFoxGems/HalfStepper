/************************************************************************************
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.  *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT. *
************************************************************************************/

#include "HalfStepper.h"

using namespace HalfStepperOptions;


HalfStepper::HalfStepper(word numSteps, byte pin1, byte pin2, 
	SteppingMode steppingMode = SteppingMode::HALF, PhasingMode phasingMode = PhasingMode::DUAL, SequenceType sequenceType = SequenceType::ALTERNATING) 
	: Stepper(numSteps, pin1, pin2), _PinCount(2), _NumSteps(numSteps), _SteppingMode(steppingMode), _PhasingMode(phasingMode), _SequenceType(sequenceType)
{
	_Pins = new byte[_PinCount];	
	_Pins[0] = pin1;
	_Pins[1] = pin2;


	_Stepper = new Stepper(numSteps, pin1, pin2);
}

HalfStepper::HalfStepper(word numSteps, byte pin1, byte pin2, byte pin3, byte pin4, 
	SteppingMode steppingMode = SteppingMode::HALF, PhasingMode phasingMode = PhasingMode::DUAL, SequenceType sequenceType = SequenceType::ALTERNATING) 
	: Stepper(numSteps, pin1, pin2, pin3, pin4), _PinCount(4), _NumSteps(numSteps), _SteppingMode(steppingMode), _PhasingMode(phasingMode), _SequenceType(sequenceType)
{
	_Pins = new byte[_PinCount];
	_Pins[0] = pin1;
	_Pins[1] = pin2;
	_Pins[2] = pin3;
	_Pins[3] = pin4;

	_Stepper = new Stepper(numSteps, pin1, pin2, pin3, pin4);
}

HalfStepper::~HalfStepper()
{
	delete _Stepper;
	delete[] _Pins;
}


void HalfStepper::setSpeed(long rpm)
{
	if (rpm < 0)
		_Direction = Direction::REVERSE;

	_SpeedRPM = abs(rpm);
	_DelayMS = 60L * 1000L / _NumSteps / _SpeedRPM;

	_Stepper->setSpeed(_SpeedRPM);
}

int HalfStepper::version()
{
	return 69;	// heh...
}


void HalfStepper::SetDirection(Direction direction) { _Direction = direction;  }

Direction HalfStepper::GetDirection() { return _Direction; }

word HalfStepper::GetSpeedRPMs() { return _Direction; }

void HalfStepper::SetSteppingMode(SteppingMode steppingMode) { _SteppingMode = steppingMode; }

SteppingMode HalfStepper::GetSteppingMode() { return _SteppingMode; }

void HalfStepper::SetPhasingMode(PhasingMode phasingMode) { _PhasingMode = phasingMode; }

PhasingMode HalfStepper::GetPhasingMode() { return _PhasingMode; }

void HalfStepper::SetSequenceType(SequenceType sequenceType) { _SequenceType = sequenceType; }

SequenceType HalfStepper::GetDriverType() { return _SequenceType; }


void HalfStepper::StepForward(word numSteps) { this->step(numSteps); }

void HalfStepper::StepBackward(word numSteps) { this->step(numSteps); }

void HalfStepper::step(int numSteps)
{
	if (numSteps == 0)
		return;

	if (numSteps > 0)
	{
		_Direction = Direction::FORWARD;
	}
	else
	{
		_Direction = Direction::REVERSE;
		numSteps = abs(numSteps);
	}

	while (numSteps-- > 0)
	{
		if (millis() - _LastStepMS >= _DelayMS)
		{
			_LastStepMS = millis();

			if (_Direction == Direction::FORWARD)
			{
				if (++_CurrStepNum == numSteps)
					_CurrStepNum = 0;
			}
			else
			{
				if (_CurrStepNum == 0)
					_CurrStepNum = numSteps;
				_CurrStepNum--;
			}

			this->DoStep(_CurrStepNum % 4);
		}
	}
}

void HalfStepper::DoStep(byte stepIdx)
{
	/*
	if (	pin_count == 2)
	{
	switch (stepIndex) {
	case 0:
	digitalWrite(motor_pin_1, LOW);
	digitalWrite(motor_pin_2, HIGH);
	break;
	case 1:
	digitalWrite(motor_pin_1, HIGH);
	digitalWrite(motor_pin_2, HIGH);
	break;
	case 2:
	digitalWrite(motor_pin_1, HIGH);
	digitalWrite(motor_pin_2, LOW);
	break;
	case 3:
	digitalWrite(motor_pin_1, LOW);
	digitalWrite(motor_pin_2, LOW);
	break;
	}
	else
	{
	switch (stepIndex) {
	case 0:    // 1010
	digitalWrite(motor_pin_1, HIGH);
	digitalWrite(motor_pin_2, LOW);
	digitalWrite(motor_pin_3, HIGH);
	digitalWrite(motor_pin_4, LOW);
	break;
	case 1:    // 0110
	digitalWrite(motor_pin_1, LOW);
	digitalWrite(motor_pin_2, HIGH);
	digitalWrite(motor_pin_3, HIGH);
	digitalWrite(motor_pin_4, LOW);
	break;
	case 2:    //0101
	digitalWrite(motor_pin_1, LOW);
	digitalWrite(motor_pin_2, HIGH);
	digitalWrite(motor_pin_3, LOW);
	digitalWrite(motor_pin_4, HIGH);
	break;
	case 3:    //1001
	digitalWrite(motor_pin_1, HIGH);
	digitalWrite(motor_pin_2, LOW);
	digitalWrite(motor_pin_3, LOW);
	digitalWrite(motor_pin_4, HIGH);
	break;
	}
	}*/
}
