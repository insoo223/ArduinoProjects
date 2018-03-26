//**************************************************************//
//  Name    : _74HC595_7SegFourDigit_StripWired_OutExp.ino                                
//  Author  : Insoo Kim
//  Create  : Oct 8, 2015 
//  Update  : Oct 8, 2015    
//  Notes   : Self-driving car on a single perforated board with w/ATmega328P in parallel with Arduino Pro Mini
//          : Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//          : 
//****************************************************************
#include "defs.h"
#include "externs.h"

//-----------------------------
void setup() 
{
  Serial.begin(9600);
  
  // ver 02 single piece of perf board
  latchPin = 6;
  clockPin = 7;
  dataPin = 8;
  
  latch4021 = 3;
  clock4021 = 4;
  data4021 = 5;
    
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(latch4021, OUTPUT);
  pinMode(clock4021, OUTPUT); 
  pinMode(data4021, INPUT);
  
  pinMode(MR_SPEEDpin, INPUT);
  //pinMode(UxECHOpin, INPUT);
  
  setupL293D();
  OffBothMotors();
  setupDHT11();
  setupRT();
  //setupCompass();
  
  //turn off motors, LEDs
  control74HC595(0, 0, 0);
  
  tirePerimeter = pi * tireD;
  curTime = millis();
  prevTime = curTime;  
  
  //attachInterrupt(digitalPinToInterrupt(MR_SPEEDpin), ISRspeed, CHANGE);
  attachInterrupt(0, ISRspeed, FALLING);

  // set time-dependent nightMode by calling DS3231
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
   // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  if (year > 100)
    nightMode=false;
  
  if ( SingleReadCD4021BE(PGM_MODEpinCD4021) == HIGH)
  {
    playMode = UX_PRG;
    //disp4char_trippleX("UXPG", 50);
  }
  else
  {
    if ( SingleReadCD4021BE(TactileButtonPinCD4021) == LOW) //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
    {
      playMode = SELF_DRIVING_IR;
      //disp4char_trippleX("SDRC", 50);
    }
    else
    {
      playMode = MENU_SYSTEM;
      //disp4char_trippleX("MENU", 50);
    }
  }
  
  prevTime=millis();
}//setup

void playMain()
{
  curTime=millis();
  lapse=curTime-prevTime;
  switch (playMode) 
  {
    case MENU_SYSTEM:
      menuSystem(); // Run-time function
      // test-time functions underneath
      //test293D(); 
      //getPureIRsignal_SingleIR(true, false); 
      //getXYZ(true);
      //printDirection();
      break;//case MENU_SYSTEM:
      
    case UX_PRG:
      UX_PRGfunc(); // Ux.ino
      break;//case UX_PRG:
      
    case SELF_DRIVING_IR:
      SELF_DRIVING_IRfunc(); // Menu.ino
      break;//case SELF_DRIVING_IR:
    case SELF_DRIVING_UX:
      //selfDriving_UXbased_obstacleAvoidance();
      break;//case SELF_DRIVING_UX:
  }//switch (playMode) 
}//playMain

void test4digit7segChar()
{
  disp4char_trippleX("FORD", 500);
  delay(1000);
  disp4char_trippleX("BACK", 500);
  delay(1000);
  disp4char_trippleX("LEFT", 500);
  delay(1000);
  disp4char_trippleX("RITE", 500);
  delay(1000);
}//test4digit7segChar

void leggcyTest()
{
  //testStrangeX();  
  /*--- test Bit Angle Modulation 
  //mainISR();
  --- test Bit Angle Modulation */
  
  //testTrippleX_Motor();
  //getPureIRsignal(true , false);  
  //delay(2000);
  //showDateTimeTemperatureHumidity();
  //setIR_RefVal_Turn_Auto();
  //intermittentGoForward();
  //testSensorBoardLED();
  //goForward();
  //delay(1000);
  //goBackward();
  //testTurnLeft();
  //OffGreenLED();
  //OffBothMotors();
  //delay(1000);
  //testTurnRight();
  //debugTrippleX_LED();
  /*---- test Ux
  loopUx();
  ------ (End) test Ux -----*/  
}//leggcyTest

void testIRpure()
{
  /*--- test IR sensors of front, left, and right
  --- test IR sensors of front, left, and right */
  getPureIRsignal(true, true);
  delay(2000);

  getPureIRsignal_SingleIR(true, true);
  delay(2000);
}//testIRpure

void testDHT11()
{
    /*--- test DHT11 
  --- (End) test DHT11 */
  loopDHT11(); 
  delay(1000);
  
}//testDHT11

void testRTC()
{
  /*--- test Real-Time module 
  --- (End) test Real-Time module */
  loopRT();
}//testRTC

void testCD4021inputExpansion()
{
  /*------ test read whole byte from CD4021BE 
  2^2: 2nd slide switch (alternative menu)
  2^1: button switch (tactile/toggle)
  2^0: left-most slide switch (UX_PGM mode)
  -------- (End) test read whole byte from CD4021BE----------*/
  byte inputStatus; //char inputStatus;

  readCD4021BE(&inputStatus);
  Serial.println(inputStatus, BIN);

  //white space
  Serial.println("-------------------");
  
  //delay so all these print satements can keep up. 
  delay(1000);
  
}//testCD4021inputExpansion

void testMRSpeed()
{
  /*------ test MR Speed
  -------- (End) test MR Speed ----------*/
  goForward();
  checkSpeed();
  if ( abs(prevSpeed - travelSpeed) > 20 )
    disp4digit7seg_trippleX(travelSpeed, 0, 1);  
  //  disp4digit7seg(travelSpeed, 0);
}//testMRSpeedJ

void testIRSensitivityWithOscilloscope()
{
  /*------ IR Sensitivity check with oscilloscope
  ------ (End) IR Sensitivity check with oscilloscope */
  sensitivityCheckIR_ForOSC();
  
  //intermittentGoForward();
  //testDoubleStage74HC595_7segLED();
  //delay(1000);
  testSingleStage74HC595_7segLED();
}//testIRSensitivityWithOscilloscope

//NOT yet tested
void togglePlayMode()
{
  /*--------
  -----------*/  
  if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
    playMode ^= 1;
  
  if (playMode == 0)
    showDateTimeTemperatureHumidity();
  else
  {
  }
}//togglePlayMode

void testMotor()
{
  //loopL293D();
}//testMotor
//-----------------------------
void loop() 
{

  int i; 
  //nightMode=true; // No RTC, so that manually set nightMode
  //main menu system to play
  playMain();
  //control74HC595(0,0,0);    

  //*** Test motor
  //loopL293D();
  //togglePlayMode();
  
  //test4digit7segChar();  
  //leggcyTest();
  
  //*** Function test
  //testRTC();  
  //testDHT11();
  //testIRpure();
  //testCD4021inputExpansion();
  //testMRSpeed();
  //testIRSensitivityWithOscilloscope(); 
  
  //Serial.println(SingleReadCD4021BE(SlideSwitch2ndPinCD4021)); //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
  //testMotor();
  
  /*--- initialize the internal EEPROM of ATmega328p
  if ( SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
    initEEPROM();
  ------- (End) initialize the internal EEPROM of ATmega328p  ---------*/  
  
}//loop

//-----------------------------------------------
void showDateTimeTemperatureHumidity()
{
  displayTime_4digit7seg();
  displayTemp_4digit7seg();
  delay(1000);
}//showDateTimeTemperatureHumidity

//-----------------------------------------------
// bit-oriented control for 74HC595
// Control three 74HC595 output eXpansion chips
// created: Thu Sep24,2015
// input args
//    ctrlID: refer "defs.h", 74HC595 pin assignments
//    state: HIGH to set, LOW to clear a corresponding pin of a 74HC595
void trippleXwrite(char ctrlID, char state)
{
  char bottomByte, middleByte, topByte;
  char sectorX, dataX;
  
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
        bottomByte = _BYTE(curBottomX) | _BV(dataX); 
      else
        bottomByte = _BYTE(curBottomX) & ~_BV(dataX);
      middleByte = _BYTE(curMiddleX);
      topByte = _BYTE(curTopX);
      //update the current bottom byte of 74HC595
      curBottomX = bottomByte;
      break;
      
    case 1: //middle 74HC595
      bottomByte = _BYTE(curBottomX);
      if  (state == HIGH)
        middleByte = _BYTE(curMiddleX) | _BV(dataX);
      else
        middleByte = _BYTE(curMiddleX) & ~_BV(dataX);
      topByte = _BYTE(curTopX);
      //update the current middle byte of 74HC595
      curMiddleX = middleByte;
      break;
      
    case 2: //top 74HC595
      bottomByte = _BYTE(curBottomX);
      middleByte = _BYTE(curMiddleX);
      if  (state == HIGH)
        topByte = _BYTE(curTopX) | _BV(dataX);
      else
        topByte = _BYTE(curTopX) & ~_BV(dataX);
      //update the current top byte of 74HC595
      curTopX = topByte;
      break;
  }//switch(sectorX)
            
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, bottomByte); //Bottom 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, middleByte); //Middle 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, topByte); //Top 74HC595
  digitalWrite(latchPin, HIGH);
}//trippleXwrite

//-----------------------------------------------
// byte-oriented control for 74HC595
void control74HC595(byte topBYTE, byte middleBYTE, byte bottomBYTE)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, bottomBYTE); //Bottom 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, middleBYTE); //Middle 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, topBYTE); //Top 74HC595
  digitalWrite(latchPin, HIGH);

}//control74HC595


