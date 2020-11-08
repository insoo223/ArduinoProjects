/*----------------------------------------------------------
File Name: 
	Four7segX.h
Purpose: 
	Control 4 digits 7 segment LED on tripple 74HC595
Updated: 
	May 15, 2019 (Wed) - OOP Simplification Stage #02
	May 14, 2019 (Tue) - OOP Simplification Stage #01
	May 06, 2019 (Mon) - parameterized 74HC595 pins for constructor  
	Oct 29, 2015 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref:
----------------------------------------------------------*/

#ifndef Four7segX_h
#define Four7segX_h

#include "Arduino.h"
#include "trippleX.h"

#define MAXCNT_DISP_NUM 12
#define BLANK_IDX		10 //array index for _7segABC_Num to represent All Blank of 7 Seg
#define BLANK_CHAR_IDX	26 //array index for _7segABC_Alpha to represent All Blank of 7 Seg
#define DP_IDX			11 //array index for _7segABC_Num to represent Decimal Point of 7 Seg

#define MAXSEG 8 // number of segments of a 7 segment LED 
#define SEG_DISP_DELAY 200 // [ms]
#define MAXDIG7SEG 4 //number of digits in your multiple digit 7 segment LED
#define DISP_NUM_NORMAL 10
#define DISP_NUM_UPSIDEDOWN 20
#define DISP_CHAR_NORMAL 30

//LED birightness is dimed in night time, by off the digit for this duration[ms]
#define NIGHT_BRIGHTNESS_DELAY 0


class Four7segX
{
public:
	Four7segX(byte* segPins);
	void setNumDigits(byte num);
	byte getNumDigits();
	void setDigitPins(byte* digitPins);

	void dispAllSegs(trippleX* X, byte numUnit);
	
	void setSingleNum595pattern (trippleX* X, byte* segVal, byte num, byte digit, byte dispMode);

	void dispSdigitChar (trippleX* X, byte singleDigitChar, byte unit, byte pos, byte dispMode);

	void dispMdigits (trippleX* X,int num, byte pos, byte duration, byte dispMode);

    void dispMchars (trippleX* X, char* str, byte pos, byte duration, byte dispMode);

	void setSingleDigitDelay(int num);
	int getSingleDigitDelay();

	void setNightMode(bool mode);
	bool getNightMode();
	
    
private:
	int simplePow(byte base, byte num);			
	void decomposeNum(int num, byte* eachDigit);
    //byte getTopX_Num(byte* numArray, byte num);
    //byte getTopX_ABC(char C);

	byte _numDigits; //number of disits in multiple-digit 7 segment LED

	byte _segPins[MAXSEG];
	
	/* Using 4 digit 7 segment LED,
	MAXDIG7SEG-1 for Kilo(1000s) digit control pin
	MAXDIG7SEG-2 for Hundres(100s) digit control pin
	MAXDIG7SEG-3 for Tens(10s) digit control pin
	MAXDIG7SEG-4 Units(1s) digit control pin
	*/
	byte _digitPins[MAXDIG7SEG];

	byte _singleDigitDelay;
	bool _nightMode;// true to dim the brightness of 7 segment LED   
    
    //----------------------------------------------
    //--- bit assignments for 7 segment LED
    //----------------------------------------------
    // A to G & DP pin assignments for common cathode 7 segment LED
    byte _7segABC_Num[MAXCNT_DISP_NUM];
    
    byte _7segABC_Num_UpsideDown[MAXCNT_DISP_NUM];
    
    // Fixed regardless of schematic
    byte _7segABC_Alpha[27];
    
};

#endif /* Four7segX_h */
