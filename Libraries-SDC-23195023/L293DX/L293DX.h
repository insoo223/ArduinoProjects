/*----------------------------------------------------------
File Name: 
	L293DX.h
Purpose: 
	Control two DC geared-motors or TT motors connected on L293D H-bridge chip.
Updated: 
	May 21, 2019 (Tue) - L293D OOP Simplification Stage #01
	Oct 29, 2015 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref:
----------------------------------------------------------*/

#ifndef L293DX_h
#define L293DX_h

#include "Arduino.h"
#include "trippleX.h"

#define NUM_MOTOR 2
#define NUM_MOTOR_POLE (NUM_MOTOR*2)

class L293DX
{
public:
    L293DX(byte* enable, byte* motorPole);

	void motorOnForDuration(trippleX* X,int duration);
	void LFwd(trippleX* X);
	void LBwd(trippleX* X);
	void LStop(int duration);

	void RFwd(trippleX* X);
	void RBwd(trippleX* X);
	void RStop(int duration);

	void fwd(trippleX* X, int duration);
	void bwd(trippleX* X, int duration);
	void bothStop(int duration);

	void turnLeft(trippleX* X, int duration);
	void turnRight(trippleX* X, int duration);
    
private:
	void _setMotEnable(byte mot, byte val);
	void _setMotPole(trippleX* X, byte pole, byte val);

    byte _enable[NUM_MOTOR];
    byte _motorPole[NUM_MOTOR_POLE]; //  byte MLPpin, byte MLNpin, byte MRPpin, byte MRNpin 
	enum _me {MLEpin, MREpin};
	enum _mp {MLPpin, MLNpin, MRNpin, MRPpin};
};
#endif /* L293DX_h */
