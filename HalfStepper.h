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

#include <Stepper\Stepper.h>

class HalfStepper
{
	public:

		HalfStepper(int number_of_steps, int motor_pin_1, int motor_pin_2);
		HalfStepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4);

		void useHalfSteps();
		void useFullSteps();

	private:

		bool _UseHalfSteps = true;
};

#endif
