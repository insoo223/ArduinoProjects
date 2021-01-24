//**************************************************************//
//  Name    : CD4021BE.ino
//  Author  : Insoo Kim
//  Create  : Oct 8, 2015 
//  Update  : Jan 5, 2019    
//  Notes   : CD4021BE Digital input expansion chip for SDC_Tandem328.ino 
//            Self-driving car on a single perforated board with w/ATmega328P in parallel with Arduino Pro Mini
//            Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//****************************************************************

//-------------------------------------------------
void setupCD4021BE()
{
  pinMode(latch4021, OUTPUT);
  pinMode(clock4021, OUTPUT); 
  pinMode(data4021, INPUT);
}//setupCD4021BE
      

void testSingleReadCD4021BE() 
{
  int a; 
  a= SingleReadCD4021BE(1);
  //a= SingleReadCD4021BE(PGM_MODEpinCD4021);
  //Serial.println(s);
  //a=234;
  disp4digit7seg_trippleX(a,0,100);
  //delay(2000);
}//testSingleReadCD4021BE
//--------------------------------------------
int SingleReadCD4021BE(byte pinPosition) 
{
  byte inputStatus=0, bitVal=0;

  //Pulse the latch pin:
  //set it to 1 to collect parallel data
  digitalWrite(latch4021,1);
  //set it to 1 to collect parallel data, wait
  delayMicroseconds(20);
  //set it to 0 to transmit data serially  
  digitalWrite(latch4021,0);

  //while the shift register is in serial mode
  //collect each shift register into a byte
  //the register attached to the chip comes in first 
  inputStatus = shiftIn(data4021, clock4021);
  bitVal = ((inputStatus & (1 << pinPosition)) >> pinPosition) & 0x01;
  return (bitVal);
      
}//SingleReadCD4021BE

//--------------------------------------------
void readCD4021BE(byte* inputStatus) 
{
  //Define variables to hold the data 
  //for shift register.
  //starting with a non-zero numbers can help
  //troubleshoot
  //byte switchVar1 = 0x88; // 10001000  //0x48;  //01001000
  //byte *inputStatus = 0x88; // 10001000;

  //Pulse the latch pin:
  //set it to 1 to collect parallel data
  digitalWrite(latch4021,1);
  //set it to 1 to collect parallel data, wait
  delayMicroseconds(20);
  //set it to 0 to transmit data serially  
  digitalWrite(latch4021,0);

  //while the shift register is in serial mode
  //collect each shift register into a byte
  //the register attached to the chip comes in first 
  *inputStatus = shiftIn(data4021, clock4021);
      
}//readCD4021BE

//---------------------------------------------------------
////// ----------------------------------------shiftIn function
///// just needs the location of the data pin and the clock pin
///// it returns a byte with each bit in the byte corresponding
///// to a pin on the shift register. leftBit 7 = Pin 7 / Bit 0= Pin 0
byte shiftIn(int myDataPin, int myClockPin) 
{ 
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);
//we will be holding the clock pin high 8 times (0,..,7) at the
//end of each time through the for loop

//at the begining of each loop when we set the clock low, it will
//be doing the necessary low to high drop to cause the shift
//register's DataPin to change state based on the value
//of the next bit in its serial information flow.
//The register transmits the information about the pins from pin 7 to pin 0
//so that is why our function counts down
  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1;
      //set the bit to 0 no matter what
      myDataIn = myDataIn | (1 << i);
    }
    else {
      //turn it off -- only necessary for debuging
     //print statement since myDataIn starts as 0
      pinState = 0;
    }

    //Debuging print statements
    //Serial.print(pinState);
    //Serial.print("     ");
    //Serial.println (dataIn, BIN);

    digitalWrite(myClockPin, 1);

  }
  //debuging print statements whitespace
  //Serial.println();
  //Serial.println(myDataIn, BIN);
  return myDataIn;
}//shiftIn
