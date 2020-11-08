/*----------------------------------------------------------
File Name: 
	Four7segX.cpp
Purpose: 
	Control 4 digits 7 segment LED on tripple 74HC595
Updated: 
	May 16, 2019 (Thu) - OOP Simplification Stage #03
		1) Moved "setSingleChipSinglePin595" function to trippleX.cpp & h. 
			For trippleX deals with chip operations of 74HC595, so that the function is reasonably cared by the tripplX class.
			HEX size of Arduino has been shrunken from 3464 to 3302 bytes. A good advantage!
	May 15, 2019 (Wed) - OOP Simplification Stage #02
		1) Modified "dispAllSegs" function to accomodate newly structured "_segPin" array elements which has chipID as well as pinID.
		2) Detached "dispSdigitChar" function from "dispMdigits", and looks more neat and tidy.
	May 14, 2019 (Tue) - OOP Simplification Stage #01
		1) Successfully created "dispMdigits" function to replace "disp4digits". 
			The former is better structured and scalable.
		2) Eliminate "disp4digits_UpsideDown" function by adding dispMode arg. to the "dispMdigits" function
		3) Created "dispMdigits" function to replace "disp4digits". 
			The former is better structured and scalable.
			Eliminate "disp4chars", "getTopX_Num", "getTopX_ABC" functions by adding dispMode arg. to the "dispMdigits" function

	May 08, 2019 (Wed) - 
		1) Class instance as call by referency 
		2) iterative logic to disp4digits, disp4chars, disp4digits_UpsideDown,
		3) Migrate global var. to corresponding class public var
			i.e. gNightMode --> nightMode in the class Four7segX
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
#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"

Four7segX :: Four7segX(byte* segPins) //May 9, 2019 
{
    _7segABC_Num[0] = 0b00111111; // DP_IDXGFEDCBA
    _7segABC_Num[1] = 0b00000110;
    _7segABC_Num[2] = 0b01011011;
    _7segABC_Num[3] = 0b01001111;
    _7segABC_Num[4] = 0b01100110;
    _7segABC_Num[5] = 0b01101101;
    _7segABC_Num[6] = 0b01111100;
    _7segABC_Num[7] = 0b00000111;
    _7segABC_Num[8] = 0b01111111;
    _7segABC_Num[9] = 0b01100111;
    _7segABC_Num[10] = 0b00000000; //BLANK_IDX (10)
    _7segABC_Num[11] = 0b10000000; //DP_IDX (11)
    
    _7segABC_Num_UpsideDown[0] = 0b00111111; //0
    _7segABC_Num_UpsideDown[1] = 0b00110000; //1
    _7segABC_Num_UpsideDown[2] = 0b01011011; //2
    _7segABC_Num_UpsideDown[3] = 0b01111001; //3
    _7segABC_Num_UpsideDown[4] = 0b01110100; //4
    _7segABC_Num_UpsideDown[5] = 0b01101101; //5
    _7segABC_Num_UpsideDown[6] = 0b01100111; //6
    _7segABC_Num_UpsideDown[7] = 0b00111000; //7
    _7segABC_Num_UpsideDown[8] = 0b01111111; //8
    _7segABC_Num_UpsideDown[9] = 0b01111100; //9
    _7segABC_Num_UpsideDown[10] = 0b00000000; //BLANK_IDX (10)
    _7segABC_Num_UpsideDown[11] = 0b10000000; //DP_IDX (11)
    
    _7segABC_Alpha[0] = 0b01110111; //A
    _7segABC_Alpha[1] = 0b01111100; //b //_7segABC_Alpha[1] = 0b01111111; //B
    _7segABC_Alpha[2] = 0b01011000; //c //_7segABC_Alpha[2] = 0b00111001; //C
    _7segABC_Alpha[3] = 0b01011110; //d
    _7segABC_Alpha[4] = 0b01111011; //e
    _7segABC_Alpha[5] = 0b01110001; //F
    _7segABC_Alpha[6] = 0b01101111; //g
    _7segABC_Alpha[7] = 0b01110100; //h
    _7segABC_Alpha[8] = 0b00000100; //i //_7segABC_Alpha[8] = 0b00000110; //I
    _7segABC_Alpha[9] = 0b00001110; //J
    _7segABC_Alpha[10] = 0b01110000; //k
    
    _7segABC_Alpha[11] = 0b00111000; //L
    _7segABC_Alpha[12] = 0b00100011; //m
    _7segABC_Alpha[13] = 0b01010100; //n
    _7segABC_Alpha[14] = 0b01011100; //o //_7segABC_Alpha[14] = 0b00111111; //O
    _7segABC_Alpha[15] = 0b01110011; //p
    _7segABC_Alpha[16] = 0b01100111; //q
    _7segABC_Alpha[17] = 0b01010000; //r //_7segABC_Alpha[17] = 0b01110111; //R
    _7segABC_Alpha[18] = 0b01101101; //S
    _7segABC_Alpha[19] = 0b01111000; //t //_7segABC_Alpha[19] = 0b00000111; //T
    _7segABC_Alpha[20] = 0b00011100; //u
    _7segABC_Alpha[21] = 0b01110010; //v
    _7segABC_Alpha[22] = 0b01001111; //W
    _7segABC_Alpha[23] = 0b01110110; //X
    _7segABC_Alpha[24] = 0b01100110; //y
    _7segABC_Alpha[25] = 0b01001001; //Z
    _7segABC_Alpha[26] = 0b00000000; //blank
    
	for(byte i=0; i<MAXSEG; i++)
		_segPins[i] = segPins[i];

	_nightMode = false;
}//Four7segX

void Four7segX :: setNumDigits(byte num)
{
	_numDigits = num;
}//setNumDigits

byte Four7segX :: getNumDigits()
{
	return(_numDigits);
}//getNumDigits

void Four7segX :: setDigitPins(byte* digitPins) // May 9, 2019 
{
	byte i;

	for(i=0; i<_numDigits; i++)
		_digitPins[i] = digitPins[i];
}//setNumDigits

void Four7segX :: setSingleDigitDelay(int num)
{
	_singleDigitDelay = num;
}//setSingleDigitDelay
int Four7segX :: getSingleDigitDelay()
{
	return(_singleDigitDelay);
}//getSingleDigitDelay

void Four7segX :: setNightMode(bool mode)
{
	_nightMode = mode;
}//setNightMode
bool Four7segX :: getNightMode()
{
	return(_nightMode);
}//getNightMode

/*----------------------------------------------------------
Function Name: 
	simplePow
Purpose: 
	Calculate N-based power and return as integer
How to:
	1. Get "num" of iteration to multiply by "base"
	2. Do for loop for iteration 
	3. Return the result as integer
Input Arguments: 
	byte num - Exponent of power
	byte base - Base of power 

Output Arguments: 
	Power of base by num
Updated: 
	May 6, 2019
Created: 
	May 6, 2019
Advantage:
	Much less flash footprint comparing with the Arduino standard "pow()"
	For example, in Arduino source code running the following,
		_7X.disp4digits(5678,0, 40); // HEX size: 3172 bytes using decimalPow, 5138 bytes using Arduino std pow
		_7X.disp4digits_legacy(5678,0, 40); // HEX size: 2868 bytes
Limitation:
	Still, the manual & fixed routine to decompose an integer,found in "disp4digits_legacy" is less footprint size
	Harder to get it than the manual routine.
Ref:
	Arduino power fuction https://www.arduino.cc/reference/en/language/functions/math/pow/
----------------------------------------------------------*/

int Four7segX :: simplePow(byte base, byte num)			
{
	byte i;
	int val=1;
	for(i=0; i<num; i++)
		val *= base;
	return(val);
}//simplePow


/*----------------------------------------------------------
Function Name: 
	decomposeNum
Purpose: 
	Decompose an integer number and save each digit to a corresponding array.
How to:
	1. Get "num" of iteration to multiply by "base"
	2. Do for loop for iteration 
	3. Return the result as integer
Input Arguments: 
	int num - An integer number to be shown at multiple-digit 7 segment LED
	byte* eachDigit - An array to hold the value of each digit of a multiple-digit 7 segment LED

Output Arguments: 
	None
Updated: 
	May 9, 2019
Created: 
	May 8, 2019
Advantage:
	Regardless of how many digits of 7 segment LED, you can easily adapt its function 
		by changing the class var. "_numDigits" by calling setNumDigits (as of May 9, 2019)
		by changing the constant "MAXDIG7SEG" (as of May 8, 2019)
Limitation:
	Still, the manual & fixed routine to decompose an integer,found in "disp4digits_legacy" of a previou version is less footprint size
	Harder to get it than the manual routine.
Ref:
----------------------------------------------------------*/
void  Four7segX :: decomposeNum(int num, byte* eachDigit)
{
	byte i;
	byte arrEle;
	int val;
	byte k;
	byte p;

	/* Following sequence to decompose an integer number has been transformed into a loop
    thousands=num/1000;
    hundreds=(num - thousands*1000)/100;
    tens=(num - thousands*1000 - hundreds*100)/10;
    ones=num % 10;
	*/
	// Loop to determine each digit (May 6, 2019)
	for (i=0; i<_numDigits; i++)
	{
		arrEle = _numDigits-1-i;
		if (i == 0) // MSB position
			val = num;
		else if (i == _numDigits-1) // LSB position
			val = num % 10;
		else
		{
			k = i;
			p = 0;
			while (k > 0)
			{
				//val -= eachDigit[arrEle+1+p] * pow(10, arrEle+1+p);
				val -= eachDigit[arrEle+1+p] * simplePow(10, arrEle+1+p);
				
				k--;
				p++;
			}
			
		}
		//eachDigit[arrEle] = val / pow(10,arrEle);
		eachDigit[arrEle] = val / simplePow(10, arrEle);
		val = num;
	}// for i
}//decomposeNum

/*----------------------------------------------------------
Function Name: 
	dispAllSegs
Purpose: 
	Display every segment on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. An input "num" will be decomposed as thousands, hundres, tens and ones.
	2. The decomposed numbers will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments: 
	trippleX* X - an instance of the class for handling 3 (or multiple) 74HC595 chips
	byte numUnit - digit position of 4 (or multiple) digits 7segment LED
Updated: 
Created: 
	May 9, 2019
Limitation:
Ref:
----------------------------------------------------------*/
void Four7segX :: dispAllSegs(trippleX* X, byte numUnit)
{
  byte m, aByte; 
  //byte arrBytes[MAXCHIP74HC595];

  for(m=0; m<MAXSEG; m++)
  {
	aByte = simplePow(2,(_segPins[m] & 0x0F));

	/*
	arrBytes[MAXCHIP74HC595-1] = aByte;
	arrBytes[MAXCHIP74HC595-2] = _BV(numUnit);
	arrBytes[MAXCHIP74HC595-3] = 0;

	X->updateX(arrBytes);
	X->ctrlAll();
	*/
	X->ctrlAll_legacy (aByte, _BV(numUnit), 0);
    delay(SEG_DISP_DELAY);

	/*
	arrBytes[MAXCHIP74HC595-1] = 0;
	arrBytes[MAXCHIP74HC595-2] = _BV(numUnit);
	arrBytes[MAXCHIP74HC595-3] = 0;
	X->updateX(arrBytes);
	X->ctrlAll();
	*/
	X->ctrlAll_legacy (0, _BV(numUnit), 0);
    delay(SEG_DISP_DELAY);
  }
}// dispAllSegs  

/*----------------------------------------------------------
Function Name: 
	setSingleNum595pattern
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. 
	2. 
	3. 
Arguments:
	Input
		trippleX* X - Class instance for multiple 74HC595
		byte* segVal - Array of segment values for numbers
		byte num - Number to be displayed on a sigle digit 
		byte digit - digit position of 4 (or multiple) digits 7segment LED 
		byte dispMode - display direction or mode(number or char.)
Updated: 
	May 16, 2019 (Thu)
Created: 
	May 13, 2019 (Mon)
Limitation:
Ref:
----------------------------------------------------------*/
void Four7segX :: setSingleNum595pattern (trippleX* X, byte* segVal, byte num, byte digit, byte dispMode)
{
	byte i;
	byte chip595;
	byte pin595;

	//set segments
	for(i=0; i<MAXSEG; i++)
	{
		chip595 = (_segPins[i] >> SECTOR_BIT_LOCATION_74HC595) & 0x0F;
		pin595 = _segPins[i] & 0x0F;
		if ( ( (segVal[num] >> i) & 0x01) == 1)
			X->setSingleChipSinglePin595(chip595, pin595, 1);
			//setSingleChipSinglePin595(X, chip595, pin595, 1);
		else
			X->setSingleChipSinglePin595(chip595, pin595, 0);
			//setSingleChipSinglePin595(X, chip595, pin595, 0);

	}
	
	//set digit
	if ( (dispMode == DISP_NUM_UPSIDEDOWN) || (dispMode == DISP_CHAR_NORMAL) )
		digit = MAXDIG7SEG - 1 - digit;
	chip595 = (_digitPins[digit] >> SECTOR_BIT_LOCATION_74HC595) & 0x0F;
	pin595 = _digitPins[digit] & 0x0F;
	//setSingleChipSinglePin595(X, chip595, pin595, 1);
	X->setSingleChipSinglePin595(chip595, pin595, 1);
}//setSingleNum595pattern

/*----------------------------------------------------------
Function Name: 
	dispSdigitChar
Purpose: 
	Display a single integer on a specific digit of 7segment LED of common cathode type.
How to:
	1. According to the input "dispMode", referece array values are set.
	2. "setSingleNum595pattern" function will compose the byte patterns of all 74HC595 chips.
	3. "X->ctrlAll()" will shift-out the byte patterns to the corresponding 74HC595 chips.
	4. if "_nightMode" is set, then make the 7 seg LED dimmer or darker.
	5. After displaying a single digit for "_singleDigitDelay", the digit will be blank off.
Arguments:
	Input & Output
		trippleX* X - Class instance for multiple 74HC595
	Input
	byte singleDigit - a single integer digit to display
	byte unit - a digit position or unit of a multiple digit 7 seg LED. 
				Depends on the schematic. For SDC v1.x, the lower unit , the less significant number.
	byte pos - Position to display Decimal Point(DP)
	byte dispMode - display mode or orientation.
					header file(Four7segX.h) has definitions 
Updated: 
	May 15, 2019 (Wed)
Created: 
	May 15, 2019 (Wed)
Limitation:
Advantage:
	Detached from the longer "dispMdigits". 
Ref:
----------------------------------------------------------*/
void Four7segX :: dispSdigitChar (trippleX* X, byte arrEleSingleDigitChar, byte unit, byte pos, byte dispMode)
{
	byte n;
	byte chip595;
	byte pin595;

	if (dispMode == DISP_NUM_NORMAL)
		setSingleNum595pattern(X, _7segABC_Num, arrEleSingleDigitChar, unit, dispMode);
	else if (dispMode == DISP_NUM_UPSIDEDOWN)
		setSingleNum595pattern(X, _7segABC_Num_UpsideDown, arrEleSingleDigitChar, unit, dispMode);
	else if (dispMode == DISP_CHAR_NORMAL)
		setSingleNum595pattern(X, _7segABC_Alpha, arrEleSingleDigitChar, unit, dispMode);
	X->ctrlAll();

	if (_nightMode)
	{
		if (dispMode == DISP_NUM_NORMAL)
			setSingleNum595pattern(X, _7segABC_Num, BLANK_IDX, unit, dispMode);
		else if (dispMode == DISP_NUM_UPSIDEDOWN)
			setSingleNum595pattern(X, _7segABC_Num_UpsideDown, BLANK_IDX, unit, dispMode);
		else if (dispMode == DISP_CHAR_NORMAL)
			setSingleNum595pattern(X, _7segABC_Alpha, BLANK_CHAR_IDX, unit, dispMode);
		X->ctrlAll();
		delay(NIGHT_BRIGHTNESS_DELAY);
	}

	// should be short enough to give a static view of multiple digits on 7 seg LED
	//"_singleDigitDelay" determines how long a single digit of a number are to be shown. 
	delay(_singleDigitDelay);

	//Clear digit for each digit should be turned on and off sequentially.
	if (dispMode == DISP_NUM_NORMAL)
		n = unit;
	else 
		n = MAXDIG7SEG - 1 - unit ;
	chip595 = (_digitPins[n] >> SECTOR_BIT_LOCATION_74HC595) & 0x0F;
	pin595 = _digitPins[n] & 0x0F;
	//setSingleChipSinglePin595(X, chip595, pin595, 0);
	X->setSingleChipSinglePin595(chip595, pin595, 0);
}//dispSdigitChar

/*----------------------------------------------------------
Function Name: 
	dispMdigits
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. An input "num" will be decomposed as thousands, hundres, tens and ones.
	2. The decomposed numbers will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments:
	Input & Output
		trippleX* X - Class instance for multiple 74HC595
	Input
	int num - Number to display
	byte pos - Position to display Decimal Point(DP)
	byte duration - Number of iteration for sigle digit duration
	byte dispMode - display mode or orientation.
					header file(Four7segX.h) has definitions 
Updated: 
	May 14, 2019 (Tue)
Created: 
	May 13, 2019 (Mon)
Limitation:
Advantage:
	Better structured and scalable than the legacy "disp4digits". 
Ref:
----------------------------------------------------------*/
void Four7segX :: dispMdigits(trippleX* X,int num, byte pos, byte duration, byte dispMode)
{
    
	byte eachDigit[MAXDIG7SEG]; //Decomposition of input arg."num"
								//array element 0 for 10^0, 1 for 10^1, 2 for 10^2, 3 for 10^3
	byte k;
	byte d;

	//Get the number for each digit position
	decomposeNum(num, eachDigit);
    
	//display 1 to _numDigits digit on 7 segment LED
	//"duration" determines how long the whole digits of a number are to be shown. 
	for(k=0; k<duration; k++)
    {
		//display each digit of 7 segment LED
		for(d=0; d < _numDigits; d++)
		{
			dispSdigitChar (X, eachDigit[d], d, pos, dispMode);
		}//for d
	}//for k
	
}//dispMdigits

/*----------------------------------------------------------
Function Name: 
	dispMchars
Purpose: 
	Display a character string on 4 (or multiple) digits 7segment  LED of common cathode type.
How to:
	1. An input "str" will be parced.
	2. Each char will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments: 
	char* str - Character string to display
	byte duration - Number of iteration for sigle digit duration
	byte dispMode - display mode or orientation.
					header file(Four7segX.h) has definitions 
Updated: 
	May 15, 2019 (Wed)
	May 8, 2019 (Wed)
		1) Give the Class instance as call-by reference argument, rather than creat another instance in the function.
		--> Save ATmega328P flash footprint by 400 bytes (HEX size from 3212 to 2812).
			By calling test_trippleX_7segChar(); and running _7X.disp4chars(&X, "ABCD", 200); than _7X.disp4chars_legacy("ABCD", 200); in the Arduino source.
		--> Change the calling pattern to the class because of using its instance as call-by reference argument.
			From X.getCurrentX(group595); to X->getCurrentX(group595);
		2)  Use iteration in "Four7segX :: disp4digits" function
		--> Save HEX size by ?? bytes: (HEX size from ?? to ??).

	May 6, 2019 (Mon)
Created: 
	Oct 29, 2015
Limitation:
	"disp4digits" function is using decomposition loop which is applicable regardless of number of digits of 7 segment LED,
	But, has bigger footprint in flash size. i.e.
		_7X.disp4digits(5678,0, 40); // HEX size: 3172 bytes using decimalPow, 5138 bytes using Arduino std pow
		_7X.disp4digits_legacy(5678,0, 40); // HEX size: 2868 bytes
Ref:
	Arduino power fuction https://www.arduino.cc/reference/en/language/functions/math/pow/
----------------------------------------------------------*/
void Four7segX :: dispMchars (trippleX* X, char* str, byte pos, byte duration, byte dispMode)
{
	byte k;
	byte d;
	byte arrEle;

    //Byte pattern of parallel pins of each 74HC595 chip
	byte group595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 

	//display 1 to _numDigits digit on 7 segment LED
	//"duration" determines how long the whole digits of a number are to be shown. 
	for(k=0; k<duration; k++)
    {
		//display each digit of 7 segment LED
		for(d=0; d < _numDigits; d++)
		{
			if (str[d] == ' ')
				arrEle = BLANK_CHAR_IDX;
			else
				arrEle = str[d]-'A';

			dispSdigitChar (X, arrEle, d, pos, dispMode);
		}//for d

	}//for k
	
}//dispMchars
