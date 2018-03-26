
// A to G & DP pin assignments for common cathode 7 segment LED     
byte _7segABC_Num[12] = {
              // dpGFEDCBA
		0b00111111, //0
		0b00000110, //1
		0b01011011, //2
		0b01001111, //3
		0b01100110, //4
		0b01101101, //5
		0b01111100, //6
		0b00000111, //7
		0b01111111, //8
		0b01100111, //9  
		0b00000000, //Blank (10)
		0b10000000 //DP (11)
        }; //_7segABC_Num[11]

byte _7segABC_Num_UpsideDown[12] = {
              // dpGFEDCBA
		0b00111111, //0
		0b00110000, //1
		0b01011011, //2
		0b01111001, //3
		0b01110100, //4
		0b01101101, //5
		0b01100111, //6
		0b00111000, //7
		0b01111111, //8
		0b01111100, //9  
		0b00000000, //Blank (10)
		0b10000000 //DP (11)
        }; //_7segABC_Num_UpsideDown[11]

// Fixed regardless of schematic
byte _7segABC_Alpha[26] = {
              // dpGFEDCBA
		0b01110111, //A
		0b01111100, //b //0b01111111, //B  
		0b01011000, //c //0b00111001, //C
		0b01011110, //d
		0b01111011, //e
		0b01110001, //F
		0b01101111, //g
		0b01110100, //h
		0b00000100, //i //0b00000110, //I
		0b00001110, //J  
		0b01110000, //k

		0b00111000, //L
		0b00100011, //m
		0b01010100, //n
		0b01011100, //o //0b00111111, //O
		0b01110011, //p
		0b01100111, //q
		0b01010000, //r //0b01110111, //R
		0b01101101, //S
		0b01111000, //t //0b00000111, //T
		0b00011100, //u  
		0b01110010, //v
		0b01001111, //W
		0b01110110, //X  
		0b01100110, //y
		0b01001001 //Z
        }; //_7segABC_Alpha[26]

// Variable depending on schematic
// Q-pin assignments for top X in trippleX,
//  beginning A to G & last one is for DP
//byte _74HC595pin[8] = {5, 6, 1, 7, 2, 4, 3, 0};  // ver01
byte _74HC595pin[8] = {0, 2, 6, 4, 3, 1, 7, 5};  // ver02

#define BLANK 10
#define DP 11

//-------------------------------------------------
byte getTopByteX_Num(byte num)
{
  byte targetByte, topByte=0;
  byte i;
  
  // get A to G & DP bit pattern for "num"
  targetByte = _7segABC_Num[num];
  
  // convert 7seg ABC byte into the byte for 74HC595 Q-pin 
  for (i=0; i<8; i++)
    if ( (targetByte >> i) & 0x01 )
      topByte |= 1 << _74HC595pin[i];

  return (topByte);
} //getTopByteX_Num

//-------------------------------------------------
byte getTopByteX_Num_UpsideDown(byte num)
{
  byte targetByte, topByte=0;
  byte i;
  
  // get A to G & DP bit pattern for "num"
  targetByte = _7segABC_Num_UpsideDown[num];
  
  // convert 7seg ABC byte into the byte for 74HC595 Q-pin 
  for (i=0; i<8; i++)
    if ( (targetByte >> i) & 0x01 )
      topByte |= 1 << _74HC595pin[i];

  return (topByte);
} //getTopByteX_Num_UpsideDown

//-------------------------------------------------
byte getTopByteX_ABC(char C)
{
  byte targetByte, topByte=0;
  byte i, arrIndex;
  
  arrIndex = C - 'A';
  // get A to G & DP bit pattern for "num"
  targetByte = _7segABC_Alpha[arrIndex];
  
  // convert 7seg ABC byte into the byte for 74HC595 Q-pin 
  for (i=0; i<8; i++)
    if ( (targetByte >> i) & 0x01 )
      topByte |= 1 << _74HC595pin[i];

  return (topByte);
} //getTopByteX_ABC
        
//-------------------------------------------------
void testSingleStage74HC595_7segLED()
{
  for(int k=0; k<10; k++)
    disp4digit7seg_trippleX(k, 0, 500);
  for(int k=10; k<100; k+=10)
    disp4digit7seg_trippleX(k, 0, 500);
  for(int k=100; k<1000; k+=100)
    disp4digit7seg_trippleX(k, 0, 500);
  for(int k=1000; k<10000; k+=1000)
    disp4digit7seg_trippleX(k, 0, 500);
}//testSingleStage74HC595_7segLED

//-------------------------------------------------
void dispABC_trippleX(char C, int duration)
{
  for (int k=0; k<duration; k++)
    control74HC595 (getTopByteX_ABC(C), _BV(DIGIT_U_PIN) | curMiddleX, curBottomX);
}

//-------------------------------------------------
void disp4char_trippleX(char* str, int duration)
{
  for (int k=0; k<duration; k++)
  {
    curTopX = getTopByteX_ABC(str[3]);
    curMiddleX = _BV(DIGIT_U_PIN);
    control74HC595 (curTopX, curMiddleX, curBottomX);
    if (nightMode)
    {
      curMiddleX &= ~_BV(DIGIT_U_PIN);
      control74HC595 (curTopX, curMiddleX, curBottomX);
      delay(3);
    }
    else
    {
      delay(singleDigitDelay);
    }

    curTopX = getTopByteX_ABC(str[2]);
    curMiddleX = _BV(DIGIT_T_PIN);
    control74HC595 (curTopX, curMiddleX, curBottomX);
    //control74HC595 (getTopByteX_ABC(str[2]), _BV(DIGIT_T_PIN) | curMiddleX, curBottomX);
    if (nightMode)
    {
      curMiddleX &= ~_BV(DIGIT_T_PIN);
      control74HC595 (curTopX, curMiddleX, curBottomX);
      //control74HC595 (getTopByteX_ABC(str[2]), ~_BV(DIGIT_T_PIN) & curMiddleX, curBottomX);
      delay(3);
    }
    else
    {
      delay(singleDigitDelay);
    }
    
    curTopX = getTopByteX_ABC(str[1]);
    curMiddleX = _BV(DIGIT_H_PIN);
    control74HC595 (curTopX, curMiddleX, curBottomX);
    //control74HC595 (getTopByteX_ABC(str[1]), _BV(DIGIT_H_PIN) | curMiddleX, curBottomX);
    if (nightMode)
    {
      curMiddleX &= ~_BV(DIGIT_H_PIN);
      control74HC595 (curTopX, curMiddleX, curBottomX);
      //control74HC595 (getTopByteX_ABC(str[1]), ~_BV(DIGIT_H_PIN) & curMiddleX, curBottomX);
      delay(3);
    }
    else
    {
      delay(singleDigitDelay);
    }
    
    curTopX = getTopByteX_ABC(str[0]);
    curMiddleX = _BV(DIGIT_K_PIN);
    control74HC595 (curTopX, curMiddleX, curBottomX);
    //control74HC595 (getTopByteX_ABC(str[0]), _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
    if (nightMode)
    {
      curMiddleX &= ~_BV(DIGIT_K_PIN);
      control74HC595 (curTopX, curMiddleX, curBottomX);
      //control74HC595 (getTopByteX_ABC(str[0]), ~_BV(DIGIT_K_PIN) & curMiddleX, curBottomX);
      delay(3);
    }
    else
    {
      delay(singleDigitDelay);
    }
  }
  //eliminate a remnant display at the 1000s digit
  
  curTopX |= 0x00;
  curMiddleX |= _BV(DIGIT_K_PIN);
  control74HC595 (curTopX, curMiddleX, curBottomX);
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
}//disp4char_trippleX

//----------------------------------------------------------
void disp4digit7seg_trippleX(int num, int pos, int duration) 
{
  int ones, tens, hundreds, thousands;
  byte dpAdded, dispBlank;
  
  thousands=num/1000;
  hundreds=(num - thousands*1000)/100;
  tens=(num - thousands*1000 - hundreds*100)/10;
  ones=(num - thousands*1000 - hundreds*100 - tens*10);
  
    for(int k=0; k<duration; k++)
    {
      //mid74HC595: 0x01 - control 1s common cathode transistor
      curTopX = getTopByteX_Num(ones);
      if (pos & 0x01 == 1)
        curTopX |= getTopByteX_Num(DP);
      control74HC595 (curTopX, _BV(DIGIT_U_PIN) | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (curTopX, ~_BV(DIGIT_U_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }

      delay(singleDigitDelay);

      //mid74HC595: 0x02 - control 10s common cathode transistor
      // remove leading zeros
      if ( (thousands == 0) && (hundreds == 0) && (tens == 0))
        curTopX = getTopByteX_Num(BLANK);
      else
        curTopX = getTopByteX_Num(tens);
  
      if ((pos & 0x02)>>1 == 1)
        curTopX |= getTopByteX_Num(DP);
  
      control74HC595 (curTopX, _BV(DIGIT_T_PIN)  | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (dispBlank, ~_BV(DIGIT_T_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }

      delay(singleDigitDelay);

      //mid74HC595: 0x20 - control 100s common cathode transistor
      //remove leading zeros
      if ( (thousands == 0) && (hundreds == 0) )
        curTopX = getTopByteX_Num(BLANK);
      else
        curTopX = getTopByteX_Num(hundreds);
        
      if ((pos & 0x04)>>2 == 1)
        curTopX |= getTopByteX_Num(DP);
        
      control74HC595 (curTopX, _BV(DIGIT_H_PIN)  | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (curTopX, ~_BV(DIGIT_H_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }
      
      delay(singleDigitDelay);
      
      //mid74HC595: 0x10 - control 1000s common cathode transistor
      //remove leading zeros
      
      if (thousands == 0)
        curTopX = getTopByteX_Num(BLANK);
      else
        curTopX = getTopByteX_Num(thousands);

      if ( (pos & 0x08)>>3 == 1)
        curTopX |= getTopByteX_Num(DP);

      control74HC595 (curTopX, _BV(DIGIT_K_PIN)  | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (dispBlank, ~_BV(DIGIT_K_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }

      delay(singleDigitDelay);
      
    }
    //eliminate a remnant display at the 1000s digit
    control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);

}//disp4digit7seg_trippleX

//----------------------------------------------------------
void disp4digit7seg_UpsideDown_trippleX(int num, int pos, int duration) 
{
  int ones, tens, hundreds, thousands;
  byte dpAdded, dispBlank;
  
  thousands=num/1000;
  hundreds=(num - thousands*1000)/100;
  tens=(num - thousands*1000 - hundreds*100)/10;
  ones=(num - thousands*1000 - hundreds*100 - tens*10);
  
    for(int k=0; k<duration; k++)
    {
      //mid74HC595: 0x01 - control 1s common cathode transistor
      curTopX = getTopByteX_Num_UpsideDown(ones);
      if (pos & 0x01 == 1)
        curTopX |= getTopByteX_Num_UpsideDown(DP);
      control74HC595 (curTopX, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (curTopX, ~_BV(DIGIT_K_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }

      delay(singleDigitDelay);

      //mid74HC595: 0x02 - control 10s common cathode transistor
      // remove leading zeros
      if ( (thousands == 0) && (hundreds == 0) && (tens == 0))
        curTopX = getTopByteX_Num_UpsideDown(BLANK);
      else
        curTopX = getTopByteX_Num_UpsideDown(tens);
  
      if ((pos & 0x02)>>1 == 1)
        curTopX |= getTopByteX_Num_UpsideDown(DP);
  
      control74HC595 (curTopX, _BV(DIGIT_H_PIN)  | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (dispBlank, ~_BV(DIGIT_H_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }

      delay(singleDigitDelay);

      //mid74HC595: 0x20 - control 100s common cathode transistor
      //remove leading zeros
      if ( (thousands == 0) && (hundreds == 0) )
        curTopX = getTopByteX_Num_UpsideDown(BLANK);
      else
        curTopX = getTopByteX_Num_UpsideDown(hundreds);
        
      if ((pos & 0x04)>>2 == 1)
        curTopX |= getTopByteX_Num_UpsideDown(DP);
        
      control74HC595 (curTopX, _BV(DIGIT_T_PIN)  | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (curTopX, ~_BV(DIGIT_T_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }
      
      delay(singleDigitDelay);
      
      //mid74HC595: 0x10 - control 1000s common cathode transistor
      //remove leading zeros
      
      if (thousands == 0)
        curTopX = getTopByteX_Num_UpsideDown(BLANK);
      else
        curTopX = getTopByteX_Num_UpsideDown(thousands);

      if ( (pos & 0x08)>>3 == 1)
        curTopX |= getTopByteX_Num_UpsideDown(DP);

      control74HC595 (curTopX, _BV(DIGIT_U_PIN)  | curMiddleX, curBottomX);
      if (nightMode)
      {
        control74HC595 (dispBlank, ~_BV(DIGIT_U_PIN) & curMiddleX, curBottomX);
        delay(NIGHT_BRIGHTNESS_DELAY);
      }

      delay(singleDigitDelay);
      
    }
    //eliminate a remnant display at the 1000s digit
    control74HC595 (0x00, _BV(DIGIT_U_PIN) | curMiddleX, curBottomX);

}//disp4digit7seg_UpsideDown_trippleX

//----------------------------------------------------------

