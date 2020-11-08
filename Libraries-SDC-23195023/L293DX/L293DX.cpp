/*----------------------------------------------------------
File Name: 
	L293DX.cpp
Purpose: 
	Control two DC geared-motors or TT motors connected on L293D H-bridge chip.
Updated: 
	May 21, 2019 (Tue) - L293D OOP Simplification Stage #01
		1) Function trimmed

	Oct 29, 2015 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref:
----------------------------------------------------------*/

#include "Arduino.h"
#include "L293DX.h"

/*----------------------------------------------------------
Function Name: 
	L293DX
Purpose: 
	The constructor of the class L293DX.
	Assign pins for Enable & Positive/Negative poles of motors
How to:
	1. Get "enable" & "motorPole" pins array as input arg. 
	2. Assign them to the corresponding privat vars.
Input Arguments: 
	byte* enable - Enable pins of L293D chip
	byte* motorPole - Positive/Negative or "A" pins of L293D chip

Output Arguments: 
	NA
Updated: 
	May 16, 2019 (Thu)
Created: 
	May 6, 2019 (Mon)
Advantage:
Limitation:
Ref:
	L293D datasheet https://app.box.com/s/50le9cx36soiljm42cxe2ibp9elxzv23
----------------------------------------------------------*/
L293DX :: L293DX(byte* enable, byte* motorPole)
{
	for (byte i=0; i<NUM_MOTOR_POLE; i++)
	{
		_motorPole[i] = motorPole[i];
		if (i < NUM_MOTOR)
		{
			_enable[i] = enable[i];
			pinMode(_enable[i],OUTPUT);
		}
	}

}//L293DX

void  L293DX :: _setMotEnable(byte mot, byte val)
{
    analogWrite(_enable[mot] & 0x0F, val);
}//_setMotEnable

void  L293DX :: _setMotPole(trippleX* X, byte pole, byte val)
{
	byte chip74HC595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 
	
	X->getCurrentX(chip74HC595);
	if (val == HIGH)
		chip74HC595[MAXCHIP74HC595-3] |= _BV(pole & 0x0F); 
	else
		chip74HC595[MAXCHIP74HC595-3] &= ~_BV(pole & 0x0F); 
	X->updateX(chip74HC595);
}//_setMotPole

void L293DX :: motorOnForDuration(trippleX* X,int duration)
{
	X->ctrlAll();
    delay(duration);
}
//--------------------------------------------
//--- LEFT MOTOR
//--------------------------------------------

void L293DX :: LFwd(trippleX* X)
{
	_setMotEnable(MLEpin, 255);

	_setMotPole(X, MLPpin, HIGH);
	_setMotPole(X, MLNpin, LOW);
}//LFwd

//--------------------------------------------------
void L293DX :: LBwd(trippleX* X)
{
	_setMotEnable(MLEpin, 255);

	_setMotPole(X, MLPpin, LOW);
	_setMotPole(X, MLNpin, HIGH);
}//LBwd

//--------------------------------------------------
void L293DX :: LStop(int duration)
{
	_setMotEnable(MLEpin, 0);
    delay(duration);
}//LStop

//--------------------------------------------
//--- RIGHT MOTOR
//--------------------------------------------

void L293DX :: RFwd(trippleX* X)
{
	_setMotEnable(MREpin, 255);

	_setMotPole(X, MRPpin, HIGH);
	_setMotPole(X, MRNpin, LOW);
}//RFwd

//--------------------------------------------------
void L293DX :: RBwd(trippleX* X)
{
	_setMotEnable(MREpin, 255);

	_setMotPole(X, MRPpin, LOW);
	_setMotPole(X, MRNpin, HIGH);
}//RBwd

//--------------------------------------------------
void L293DX :: RStop(int duration)
{
	_setMotEnable(MREpin, 0);
    delay(duration);
}//RStop


//--------------------------------------------
//--- Go Forward & Backward, All stop
//--------------------------------------------

void L293DX :: fwd(trippleX* X, int duration)
{
	LFwd(X);
	RFwd(X);

	motorOnForDuration(X, duration);

}//fwd

//--------------------------------------------------
void L293DX :: bothStop(int duration)
{
	_setMotEnable(MLEpin, 0);
	_setMotEnable(MREpin, 0);
    delay(duration);
}//stop

//--------------------------------------------------
void L293DX :: bwd(trippleX* X, int duration)
{
	LBwd(X);
	RBwd(X);

	motorOnForDuration(X, duration);

}//bwd


//--------------------------------------------
//--- Turn Left & Right
//--------------------------------------------
void L293DX :: turnLeft(trippleX* X, int duration)
{
	LBwd(X);
	RFwd(X);
	motorOnForDuration(X, duration);
}//turnLeft

void L293DX :: turnRight(trippleX* X, int duration)
{
	LFwd(X);
	RBwd(X);
	motorOnForDuration(X, duration);
}//turnRight

