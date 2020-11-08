/*----------------------------------------------------------
File Name: 
	trippleX.h
Purpose: 
	Control tripple 74HC595 output expansion chips
Updated: 
	May 14, 2019 (Tue) - OOP Simplification Stage #1
	May 06, 2019 (Mon) - parameterized 74HC595 pins for constructor  
		1) Four7segX(); --> Four7segX(byte A, byte B, byte C, byte D, byte E, byte F, byte G, byte DP,
				byte digitOnes, byte digitTens, byte digitHundreds, byte digitKilos); 
		2) #define DIGIT_K_PIN 3 --> byte _digitKilos; (as well as other digit pins)
	Oct 29, 2015 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref:
----------------------------------------------------------*/

#ifndef trippleX_h
#define trippleX_h

#include "Arduino.h"
//#include "Four7segX.h"

#define MAXCHIP74HC595 3 //number of 74HC595 chips used for SDC

// Bit location of 74HC595 sector
// A "sector" tells which of three 74HC595 chips holds a corresponding signal: top, middle, or bottom
#define SECTOR_BIT_LOCATION_74HC595   4

// bit mask for a corresponding signal ID
#define DATA_BITS_MASK_74HC595   0x07

// global variable declared at Arduino sketch "globals" file 
//extern uint8_t gCurTopX, gCurMidX, gCurBotX;

class trippleX
{
public:
	trippleX(byte latchPin, byte clockPin, byte dataPin);
    
	//void updateX(uint8_t topX, uint8_t midX, uint8_t botX);
	void updateX(byte* group595);

	void getCurrentX(byte *group595);
    //void getCurrentX_legacy(uint8_t *topX, uint8_t *midX, uint8_t *botX);
	//void ctrlAll(byte *pos74HC595);
	void ctrlAll();
    void ctrlAll_legacy(uint8_t topX, uint8_t midX, uint8_t botX);

	void setSingleChipSinglePin595 (byte chipID, byte pinID, byte val);
	void ctrlSingle(uint8_t ctrlID, uint8_t state);
	//void ctrlSingle_legacy(uint8_t ctrlID, uint8_t state);
	
private:
    uint8_t _l, _c, _d;
	byte _curX[MAXCHIP74HC595]; // May 8, 2019 (@ed) - Migrate from External var. to Private Class Members
};


#endif /* trippleX_h */
