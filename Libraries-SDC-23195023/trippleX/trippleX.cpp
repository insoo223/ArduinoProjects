/*----------------------------------------------------------
File Name: 
	trippleX.cpp
Purpose: 
	Control 4 digits 7 segment LED on tripple 74HC595
Updated: 
	May 06, 2019 (Mon) - parameterized 74HC595 pins for constructor  
		1) Four7segX(); --> Four7segX(byte A, byte B, byte C, byte D, byte E, byte F, byte G, byte DP,
				byte digitOnes, byte digitTens, byte digitHundreds, byte digitKilos); 
		2) #define DIGIT_K_PIN 3 --> byte _digitKilos; (as well as other digit pins)
	Oct 29, 2015 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref: Tightly coupled with "Four7segX.cpp & h"
----------------------------------------------------------*/

#include "Arduino.h"
#include "trippleX.h"


//-----------------------------------------------
//trippleX :: trippleX()
trippleX :: trippleX(byte latchPin, byte clockPin, byte dataPin)
{
    _l = latchPin;
    _c = clockPin;
    _d = dataPin;

    pinMode(_l,OUTPUT);
    pinMode(_c,OUTPUT);
    pinMode(_d,OUTPUT);

	for (byte i=0; i<MAXCHIP74HC595; i++)
		_curX[i] = 0;

}//trippleX

//-----------------------------------------------
// maintain the most current top, middle, and bottom bytes of tripple 74HC595s
//void trippleX :: updateX(uint8_t topX, uint8_t midX, uint8_t botX)
void trippleX :: updateX(byte* group595)
{
	for (byte i=0; i<MAXCHIP74HC595; i++)
		_curX[i] = group595[i];
}//updateX

//-----------------------------------------------
// get the most current top, middle, and bottom bytes of tripple 74HC595s
//void trippleX :: getCurrentX(uint8_t *topX, uint8_t *midX, uint8_t *botX)
void trippleX ::getCurrentX(byte *group595)
{
	for (byte i=0; i<MAXCHIP74HC595; i++)
		group595[i] = _curX[i];
}//getCurrentX

//-----------------------------------------------
// get the most current top, middle, and bottom bytes of tripple 74HC595s
/*
void trippleX ::getCurrentX_legacy(uint8_t *topX, uint8_t *midX, uint8_t *botX)
{
    *topX = gCurTopX;
    *midX = gCurMidX;
    *botX = gCurBotX;
}//getCurrentX_legacy
*/
//-----------------------------------------------
// byte-oriented control for 74HC595
//void trippleX :: ctrlAll(uint8_t topX, uint8_t midX, uint8_t botX)
//void trippleX :: ctrlAll(byte *pos74HC595)
void trippleX :: ctrlAll()
{
	byte i;
    
	digitalWrite(_l, LOW);
	for(i=0; i<MAXCHIP74HC595; i++)
		shiftOut(_d, _c, MSBFIRST, _curX[i]);
    digitalWrite(_l, HIGH);
    
    //updateX(topX, midX, botX); //if execute, remnant display on K-unit
}//ctrlAll

//-----------------------------------------------
// byte-oriented control for 74HC595
void trippleX :: ctrlAll_legacy(uint8_t topX, uint8_t midX, uint8_t botX)
{
    digitalWrite(_l, LOW);
    shiftOut(_d, _c, MSBFIRST, botX);
    shiftOut(_d, _c, MSBFIRST, midX);
    shiftOut(_d, _c, MSBFIRST, topX);
    digitalWrite(_l, HIGH);
    
    //updateX(topX, midX, botX); //if execute, remnant display on K-unit
}//ctrlAll_legacy
//-----------------------------------------------
// bit-oriented control for 74HC595
// Control three 74HC595 output eXpansion chips
// created: Thu Sep24,2015
// input args
//    ctrlID: refer "defs.h", 74HC595 pin assignments
//    state: HIGH to set, LOW to clear a corresponding pin of a 74HC595


/*----------------------------------------------------------
Function Name: 
	setSingleChipSinglePin595
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. 
	2. 
	3. 
Arguments:
	Input
		byte chipID - to which chip of mulitple 74HC595s 
		byte pinID - to which pin of a specific "chipID" 74HC595
		byte val - 1 or 0, HIGH or LOW
Updated: 
	May 15, 2019 (Wed)
Created: 
	May 13, 2019 (Mon)
Limitation:
Ref:
----------------------------------------------------------*/
void trippleX :: setSingleChipSinglePin595 (byte chipID, byte pinID, byte val)
{
	if (val == 1)
		_curX[MAXCHIP74HC595-chipID] |= _BV(pinID);
	else
		_curX[MAXCHIP74HC595-chipID] &= ~_BV(pinID);
}//setSingleChipSinglePin595

/*----------------------------------------------------------
Function Name: 
	ctrlSingle
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. 
	2. 
	3. 
Arguments:
	Input
		byte chipID - to which chip & pin of mulitple 74HC595s 
		byte state - 1 or 0, HIGH or LOW
Updated: 
	
Created: 
	Oct 24, 2015
Limitation:
	This is the function similar to "setSingleChipSinglePin595"
Ref:
----------------------------------------------------------*/
void trippleX :: ctrlSingle(uint8_t ctrlID, uint8_t state)
{
    //uint8_t bottomByte, middleByte, topByte;
	byte group595[MAXCHIP74HC595];

    uint8_t sectorX, dataX;
    
    // The first bit  in the upper nibble, bit 4, of ctrlID represents
    // which of three 74HC595s, called "sectorX", holds the corresponding pin
    // sectorX 0 for bottom, 1 for middle, and 2 for top sector of three 74HC595s
    sectorX = ctrlID >> SECTOR_BIT_LOCATION_74HC595;
    
    // The bits in the lower nibble, bit3 to bit0, of ctrlID represent
    // the corresponding pin of the 74HC595 chip of ctrlID
    dataX = ctrlID & DATA_BITS_MASK_74HC595;

	for (byte i=0; i<MAXCHIP74HC595; i++)
	{
		if (i == sectorX)
			if  (state == HIGH)
				group595[i] = _curX[i] | _BV(dataX);
			else
				group595[i] = _curX[i] & ~_BV(dataX);
	}

    /*
    switch (sectorX)
    {
        case 0: //bottom 74HC595
            if  (state == HIGH)
                bottomByte = gCurBotX | _BV(dataX);
            else
                bottomByte = gCurBotX & ~_BV(dataX);
            middleByte = gCurMidX;
            topByte = gCurTopX;
            break;
            
        case 1: //middle 74HC595
            bottomByte = gCurBotX;
            if  (state == HIGH)
                middleByte = gCurMidX | _BV(dataX);
            else
                middleByte = gCurMidX & ~_BV(dataX);
            topByte = gCurTopX;
            break;
            
        case 2: //top 74HC595
            bottomByte = gCurBotX;
            middleByte = gCurMidX;
            if  (state == HIGH)
                topByte = gCurTopX | _BV(dataX);
            else
                topByte = gCurTopX & ~_BV(dataX);
            break;
    }//switch(sectorX)
    */


    //update the current bytes of tripple 74HC595s
    //updateX(topByte, middleByte, bottomByte);
	updateX(group595);
            
    //ctrlAll_legacy(gCurTopX, gCurMidX, gCurBotX);
	ctrlAll();

}//ctrlSingle

/*
//-------------------------------------------------
void trippleX :: ctrlSingle_legacy(uint8_t ctrlID, uint8_t state)
{
    uint8_t bottomByte, middleByte, topByte;
    uint8_t sectorX, dataX;
    
    // The first bit  in the upper nibble, bit 4, of ctrlID represents
    // which of three 74HC595s, called "sectorX", holds the corresponding pin
    // sectorX 0 for bottom, 1 for middle, and 2 for top sector of three 74HC595s
    sectorX = ctrlID >> SECTOR_BIT_LOCATION_74HC595;
    
    // The bits in the lower nibble, bit3 to bit0, of ctrlID represent
    // the corresponding pin of the 74HC595 chip of ctrlID
    dataX = ctrlID & DATA_BITS_MASK_74HC595;
    
    switch (sectorX)
    {
        case 0: //bottom 74HC595
            if  (state == HIGH)
                bottomByte = gCurBotX | _BV(dataX);
            else
                bottomByte = gCurBotX & ~_BV(dataX);
            middleByte = gCurMidX;
            topByte = gCurTopX;
            break;
            
        case 1: //middle 74HC595
            bottomByte = gCurBotX;
            if  (state == HIGH)
                middleByte = gCurMidX | _BV(dataX);
            else
                middleByte = gCurMidX & ~_BV(dataX);
            topByte = gCurTopX;
            break;
            
        case 2: //top 74HC595
            bottomByte = gCurBotX;
            middleByte = gCurMidX;
            if  (state == HIGH)
                topByte = gCurTopX | _BV(dataX);
            else
                topByte = gCurTopX & ~_BV(dataX);
            break;
    }//switch(sectorX)
    
    //update the current bytes of tripple 74HC595s
    //updateX(topByte, middleByte, bottomByte);
            
    ctrlAll_legacy(gCurTopX, gCurMidX, gCurBotX);

}//ctrlSingle_legacy
*/
