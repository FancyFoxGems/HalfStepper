/**********************************************************************************
* Name:		HalfStepper Library
* Created:	10/2/2016
* Author:	Tom Biuso <tjbtech@yahoo.com>
*
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE LGPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT.
***********************************************************************************/

#ifndef HALFSTEPPER_H
#define HALFSTEPPER_H "FTW"


/* INCLUDES */

// AVR LIBS
#include <avr/pgmspace.h>

// ARDUINO CORE
#include "Arduino.h"

// ARDUINO LIBS
#include "Stepper.h"


/* DEFINES */

// PROGRAM OPTIONS
#ifndef DEBUG_SERIAL
	#define DEBUG_SERIAL	0
#endif

#if !defined(DEBUG_SERIAL) || DEBUG_SERIAL == 0
#undef DEBUG_SERIAL
#endif

// MACROS
#define BOOL_TO_INDEX(bool_expr)	((bool_expr) ? 1 : 0)

// TYPE DEFINITION ALIASES
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;


/* HalfStepperOptions NAMESPACE: ENUMS & STEP SEQUENCE STATES */
namespace HalfStepperOptions
{
	/* ENUMS */

	enum SteppingMode : bool
	{
		FULL = false,
		HALF = true
	};

	enum PhasingMode : bool
	{
		SINGLE = false,
		DUAL = true
	};

	enum SequenceType : bool
	{
		SEQUENTIAL = false,
		ALTERNATING = true
	};

	enum Direction : bool
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



/* HalfStepper CLASS DECLARATION */

class HalfStepper : public Stepper
{
public:

	// CONSTRUCTORS
	HalfStepper(word, byte, byte, SteppingMode = SteppingMode::HALF);
	HalfStepper(word, byte, byte, byte, byte, SteppingMode = SteppingMode::HALF,
		PhasingMode = PhasingMode::DUAL, SequenceType = SequenceType::ALTERNATING);

	// DESTRUCTOR
	virtual ~HalfStepper();

	// ACCESSORS & MUTATORS
	word GetSpeedRPMs();
	void SetSteppingMode(SteppingMode);
	SteppingMode GetSteppingMode();
	void SetPhasingMode(PhasingMode);
	PhasingMode GetPhasingMode();
	void SetSequenceType(SequenceType);
	SequenceType GetSequenceType();

	void SetDirection(Direction);
	Direction GetDirection();

	void SetPosition(word);
	word GetPosition();

	void (HalfStepper::* const SetSpeedRPMs)(long) = &HalfStepper::setSpeed;	// Member function pointer alias

	// Stepper (HIDDEN) OVERRIDES
	void setSpeed(long);
	int version(void);
	void step(int);

	// PRIMARY USER METHODS
	void StepForward(word);
	void StepBackward(word);
	void StepTo(word);


protected:

	static const dword _DELAY_MS_DEFAULT_SPEED = 5;

	// MEMORY-RESIDENT STEPS ARRAY
	byte * _Steps = NULL;

	// DECORATED STEPPER OBJECT
	Stepper * _Stepper = NULL;

	// PINS FOR MOTOR CONTROL
	byte _PinCount = 2;
	byte * _Pins = NULL;

	// MOTOR PROPERTIES / STEPPING OPTIONS
	word _TotalSteps = 0;
	SteppingMode _SteppingMode = SteppingMode::HALF;
	PhasingMode _PhasingMode = PhasingMode::DUAL;
	SequenceType _SequenceType = SequenceType::ALTERNATING;

	// SPEED & DIRECTION
	Direction _Direction = Direction::FORWARD;
	word _SpeedRPMs = 0;
	dword _DelayMS = _DELAY_MS_DEFAULT_SPEED;

	// STATE TRACKING
	word _Position = 0;
	dword _LastStepMS = 0;

	// STEP SEQUENCE RETRIEVAL METHOD
	void UpdateSteps();

	// STEP EXECUTION METHOD
	virtual void DoStep(byte);
};

#endif
