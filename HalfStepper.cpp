/************************************************************************************
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.  *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT. *
************************************************************************************/

#include "HalfStepper.h"

using namespace HalfStepperOptions;


HalfStepper::HalfStepper(word numSteps, byte pin1, byte pin2, SteppingMode steppingMode) 
	: Stepper(numSteps, pin1, pin2), _PinCount(2), _NumSteps(numSteps), 
		_SteppingMode(steppingMode), _PhasingMode(PhasingMode::DUAL), _SequenceType(SequenceType::ALTERNATING)
{
	_Pins = new byte[_PinCount];	

	_Pins[0] = pin1;
	_Pins[1] = pin2;


	_Stepper = new Stepper(numSteps, pin1, pin2);

	this->UpdateSteps();
}

HalfStepper::HalfStepper(word numSteps, byte pin1, byte pin2, byte pin3, byte pin4, 
	SteppingMode steppingMode, PhasingMode phasingMode, SequenceType sequenceType) 
	: Stepper(numSteps, pin1, pin2, pin3, pin4), _PinCount(4), _NumSteps(numSteps), 
		_SteppingMode(steppingMode), _PhasingMode(phasingMode), _SequenceType(sequenceType)
{
	_Pins = new byte[_PinCount];

	_Pins[0] = pin1;
	_Pins[1] = pin2;
	_Pins[2] = pin3;
	_Pins[3] = pin4;

	_Stepper = new Stepper(numSteps, pin1, pin2, pin3, pin4);
	this->UpdateSteps();
}

HalfStepper::~HalfStepper()
{
	delete[] _Steps;
	delete _Stepper;
	delete[] _Pins;
}


void HalfStepper::SetSteppingMode(SteppingMode steppingMode)
{
	_SteppingMode = steppingMode;
	this->UpdateSteps();
}

SteppingMode HalfStepper::GetSteppingMode() { return _SteppingMode; }

void HalfStepper::SetPhasingMode(PhasingMode phasingMode)
{
	_PhasingMode = phasingMode;
	this->UpdateSteps();
}

PhasingMode HalfStepper::GetPhasingMode() { return _PhasingMode; }

void HalfStepper::SetSequenceType(SequenceType sequenceType)
{
	_SequenceType = sequenceType;
	this->UpdateSteps();
}

SequenceType HalfStepper::GetSequenceType() { return _SequenceType; }

void HalfStepper::SetDirection(Direction direction) { _Direction = direction; }

Direction HalfStepper::GetDirection() { return _Direction; }

word HalfStepper::GetSpeedRPMs() { return _Direction; }


void HalfStepper::setSpeed(long rpms)
{
	if (rpms < 0)
		_Direction = Direction::REVERSE;

	_SpeedRPMs = abs(rpms);
	_DelayMS = 60L * 1000L / _NumSteps / _SpeedRPMs;

	_Stepper->setSpeed(_SpeedRPMs);
}

int HalfStepper::version()
{
	return 69;	// heh...
}

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

			this->DoStep(_CurrStepNum % (_PinCount * 2));
		}
	}
}


void HalfStepper::StepForward(word numSteps) { this->step(numSteps); }

void HalfStepper::StepBackward(word numSteps) { this->step(-1 * numSteps); }


void HalfStepper::UpdateSteps()
{

	if (_PinCount == 2)
	{
		_Steps = new byte[4];

		for (int i = 0; i < 4; i++)
			_Steps[i] = pgm_read_byte_near(&_STEP_SEQUENCES_TWO_PIN[BOOL_TO_INDEX(_SteppingMode)][i]);
	}
	else
	{
		_Steps = new byte[8];

		for (int i = 0; i < 4; i++)
			_Steps[i] = pgm_read_byte_near(&_STEP_SEQUENCES_FOUR_PIN[BOOL_TO_INDEX(_SteppingMode)][BOOL_TO_INDEX(_PhasingMode)][BOOL_TO_INDEX(_SequenceType)][i]);
	}


}

void HalfStepper::DoStep(byte stepIdx)
{
	if (_PinCount == 4)
	{
		digitalWrite(_Pins[0], ((_Steps[stepIdx] & B1000) >> 0x3) ? HIGH : LOW);
		digitalWrite(_Pins[1], ((_Steps[stepIdx] & B0000) >> 0x2) ? HIGH : LOW);
		digitalWrite(_Pins[2], ((_Steps[stepIdx] & B0010) >> 0x1) ? HIGH : LOW);
		digitalWrite(_Pins[3], (_Steps[stepIdx] & B0001) ? HIGH : LOW);
	}
	else
	{
		digitalWrite(_Pins[0], ((_Steps[stepIdx] & B0010) >> 0x1) ? HIGH : LOW);
		digitalWrite(_Pins[1], (_Steps[stepIdx] & B0001) ? HIGH : LOW);
	}
}
