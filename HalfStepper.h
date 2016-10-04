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

#include "Arduino.h"
#include "Stepper\Stepper.h"


#define BOOL_TO_INDEX(bool_expr)	((bool_expr) ? 1 : 0)


typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;


namespace HalfStepperOptions
{
	enum PinCount : bool
	{
		TWO = false,
		FOUR = true
	};

	enum Direction : bool
	{
		FORWARD = false,
		REVERSE = true
	};

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
}

using namespace HalfStepperOptions;



class HalfStepper : public Stepper
{
	public:

		HalfStepper(word, byte, byte, SteppingMode, PhasingMode, SequenceType);
		HalfStepper(word, byte, byte, byte, byte, SteppingMode, PhasingMode, SequenceType);

		virtual ~HalfStepper();

		// (Hidden) overrides
		void setSpeed(long);
		void step(int);
		int version(void);

		// Accessors & mutators
		void SetDirection(Direction);
		Direction GetDirection();
		void (HalfStepper::* const SetSpeedRPMs)(long) = &HalfStepper::setSpeed;	// Member function pointer alias
		word GetSpeedRPMs();
		void SetSteppingMode(SteppingMode);
		SteppingMode GetSteppingMode();
		void SetPhasingMode(PhasingMode);
		PhasingMode GetPhasingMode();
		void SetSequenceType(SequenceType);
		SequenceType GetDriverType();

		// Primary user methods
		void StepForward(word);
		void StepBackward(word);
		virtual void Step(int);

	protected:

		// Pin state sequences for steps

		// Two pin step motor sequences
		// NOTE: Only two variations exist; dual phasing is the same as half-stepping.
		const byte _STEP_SEQUENCES_TWO_PIN[2][4] = 
		{
			{ B00000001, B00000011, B00000010, B00000000 },
			{ B00000001, B00000010, B00000001, B00000010 }
		};

		// Four pin step motor sequences
		// Structured by Stepping mode --> Phasing mode --> Sequence type.
		const byte _STEP_SEQUENCES_FOUR_PIN[2][2][2][8] =
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
				
		// Decorated Stepper object
		Stepper * _Stepper = NULL;

		// Pins for motor control
		byte _PinCount = 2;
		byte * _Pins = NULL;

		// Motor properties / stepping options
		word _NumSteps = 0;
		SteppingMode _SteppingMode = SteppingMode::HALF;
		PhasingMode _PhasingMode = PhasingMode::DUAL;
		SequenceType _SequenceType = SequenceType::ALTERNATING;

		// Speed & direction
		Direction _Direction = Direction::FORWARD;
		word _SpeedRPM = 0;
		dword _DelayMS = 0;

		// State tracking
		byte _CurrStepNum = 0;
		dword _LastStepMS = 0;

		// Step execution method
		virtual void DoStep(byte);
};

#endif
