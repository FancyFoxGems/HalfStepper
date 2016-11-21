/************************************************************************************************
* Name:		HalfStepper (Arduino Library)
* Created:	10/2/2016
* Author:	Tom Biuso <tjbtech@yahoo.com>
*
* This file is the root, primary source code file for the HalfStepper Arduino library.
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE LGPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
*
* HalfStepper is free software: you can redistribute it and/or modify it under the terms of the
* GNU Lesser General Public License as published by the Free Software Foundation, either version
* 3 of the License, or (at your option) any later version.
*
* HalfStepper is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* See the included GNU Lesser General Public License text for more details.
************************************************************************************************/

#ifndef _HALFSTEPPER_
#define _HALFSTEPPER_ "FTW"


#pragma region INCLUDES

// AVR LIBS
#include <avr/pgmspace.h>

// ARDUINO CORE
#include "Arduino.h"

// ARDUINO LIBS
#include "Stepper.h"

#pragma endregion INCLUDES



#pragma region DEFINES

// PROGRAM OPTIONS

#ifndef DEBUG_SERIAL
	#define DEBUG_SERIAL	0
#endif

#if !defined(DEBUG_SERIAL) || DEBUG_SERIAL == 0
#undef DEBUG_SERIAL
#endif


// MACROS

#define BOOL_TO_INDEX(bool_expr)	((bool_expr) ? 1 : 0)

#pragma endregion DEFINES



#pragma region TYPEDEFS

// TYPE DEFINITION ALIASES
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

#pragma endregion TYPEDEFS



#pragma region HalfStepperOptions

/* [HalfStepperOptions] NAMESPACE: ENUMS & STEP SEQUENCE STATES */

namespace HalfStepperOptions
{
	// ENUMS

	enum class SteppingMode : bool
	{
		FULL = false,
		HALF = true
	};

	enum class PhasingMode : bool
	{
		SINGLE = false,
		DUAL = true
	};

	enum class SequenceType : bool
	{
		SEQUENTIAL = false,
		ALTERNATING = true
	};

	enum class Direction : bool
	{
		FORWARD = false,
		REVERSE = true
	};


	// PIN STATE SEQUENCES FOR STEPS

	// Two pin step motor sequences
	// NOTE: Only two variations exist; dual phasing is the same as half-stepping.
	PROGMEM static const byte _STEP_SEQUENCES_TWO_PIN[2][4] =
	{
		{ B00000001, B00000011, B00000010, B00000000 },
		{ B00000001, B00000010, B00000001, B00000010 }
	};

	// Four pin step motor sequences
	// Structured by Stepping mode --> Phasing mode --> Sequence type.
	PROGMEM static const byte _STEP_SEQUENCES_FOUR_PIN[2][2][2][8] =
	{
		{
			{
				{ B00001000, B00000010, B00000100, B00000001, B00001000, B00000010, B00000100, B00000001 },
				{ B00001000, B00000100, B00000010, B00000001, B00001000, B00000100, B00000010, B00000001 }
			},
			{
				{ B00001100, B00000110, B00000110, B00000011, B00001100, B00000110, B00000110, B00000011 },
				{ B00001010, B00000110, B00000101, B00001001, B00001010, B00000110, B00000101, B00001001 }
			}
		},
		{
			{
				{ B00001000, B00001100, B00000100, B00000110, B00000010, B00000011, B00000001, B00001001 },
				{ B00001000, B00001010, B00000010, B00000110, B00000100, B00000101, B00000001, B00001001 }
			},
			{
				{ B00001000, B00001100, B00000100, B00000110, B00000010, B00000011, B00000001, B00001001 },
				{ B00001000, B00001010, B00000010, B00000110, B00000100, B00000101, B00000001, B00001001 }
			}
		}
	};
}

using namespace HalfStepperOptions;

#pragma endregion HalfStepperOptions


#pragma region HalfStepper CLASS DECLARATION

/* [HalfStepper]: THE CLASS YOU WANT TO USE, SILLY. */

class HalfStepper : public Stepper
{
public:

	// CONSTRUCTORS

	HalfStepper(dword, byte, byte, SteppingMode = SteppingMode::HALF);
	HalfStepper(dword, byte, byte, byte, byte, SteppingMode = SteppingMode::HALF,
		PhasingMode = PhasingMode::DUAL, SequenceType = SequenceType::ALTERNATING);


	// DESTRUCTOR

	virtual ~HalfStepper();


	// ACCESSORS & MUTATORS

	word GetSpeedRPMs() const;
	void SetSteppingMode(SteppingMode);
	SteppingMode GetSteppingMode() const;
	void SetPhasingMode(PhasingMode);
	PhasingMode GetPhasingMode() const;
	void SetSequenceType(SequenceType);
	SequenceType GetSequenceType() const;

	void SetDirection(Direction);
	Direction GetDirection() const;

	void SetPosition(dword);
	dword GetPosition() const;

	void (HalfStepper::* const SetSpeedRPMs)(long) = &HalfStepper::setSpeed;	// Member function pointer alias


	// [Stepper] (HIDDEN) OVERRIDES

	void setSpeed(long);
	int version(void);
	void step(int);


	// PRIMARY USER METHODS

	void Step(long = 1);
	void StepForward(dword = 1);
	void StepBackward(dword = 1);
	void StepTo(dword);


protected:

	// CLASS CONSTANTS

	static const dword _DELAY_MS_DEFAULT_SPEED = 5;


	// INSTANCE VARIABLES

	// MEMORY-RESIDENT STEPS ARRAY
	byte * _Steps = NULL;

	// DECORATED STEPPER OBJECT
	Stepper * _Stepper = NULL;

	// PINS FOR MOTOR CONTROL
	byte _PinCount = 2;
	byte * _Pins = NULL;

	// MOTOR PROPERTIES / STEPPING OPTIONS
	dword _TotalSteps = 0;
	SteppingMode _SteppingMode = SteppingMode::HALF;
	PhasingMode _PhasingMode = PhasingMode::DUAL;
	SequenceType _SequenceType = SequenceType::ALTERNATING;

	// SPEED & DIRECTION
	Direction _Direction = Direction::FORWARD;
	word _SpeedRPMs = 0;
	dword _DelayMS = _DELAY_MS_DEFAULT_SPEED;

	// STATE TRACKING
	dword _Position = 0;
	dword _LastStepMS = 0;


	// HELPER METHODS

	// STEP SEQUENCE RETRIEVAL METHOD
	void UpdateSteps();

	// STEP EXECUTION METHOD
	virtual void DoStep(byte);
};

#pragma endregion HalfStepper CLASS DECLARATION

#endif
