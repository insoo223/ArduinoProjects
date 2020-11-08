//**************************************************************//
//  Name    : SDC_Tandem328.ino                                
//  Author  : Insoo Kim
//  Create  : Oct 8, 2015 
//  Update  : Nov 8, 2020    
//  Notes   : Self-driving car on a single perforated board with w/ATmega328P in parallel with Arduino Pro Mini
//          : Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//****************************************************************
#include "defs.h"
#include "externs.h"

//-----------------------------
void setup() 
{
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("Serial setup success!");

  setGlobalVars(); // globals.ino

  //setup pin assignments and its mode of operations(Input or Output) or call required functions
  //Disable Speed intrp for its collision with tone function (Mar 26, 2019)
  //setupSpeedIntrPin(); // speed.ino
  
  setupCD4021BE(); // CD4021BE.ino
  setup74HC595(); // _7segLED.ino
  setupL293D(); // L293D.ino
  setupDHT11(); // DHT11.ino
  
  //setupRT(); //for RT module of 3231
  //setupDS1307();
  // CALL ONLY ONCE, WHEN SETUP Real-Time Clock. Refer RT_DS1307.ino
  //setTimeDS1307(); 
  // CALL ONLY ONCE, WHEN SETUP Real-Time Clock (End)
  
  setupCompass();
  
  //turn off motors, LEDs
  control74HC595(0, 0, 0);
  // again off motors
  OffBothMotors();
  
  // set time-dependent nightMode by calling DS1307
  // added by Insoo (Mar 27, 2019)
  getNightMode();
  
  /*
  // set time-dependent nightMode by calling DS3231
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
   // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  if (year > 100)
    nightMode=false;
  */
  /*
  if ( SingleReadCD4021BE(PGM_MODEpinCD4021) == HIGH)
  {
    playMode = UX_PRG;
    //disp4char_trippleX("UXPG", 50);
  }
  else
  */
  {
    //Case 1: when pressed tactile switch while power-on
    //if ( SingleReadCD4021BE(TactileButtonPinCD4021) == LOW) //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
    //Case 2: when the left-most slide switch is ON while power-on
    if ( SingleReadCD4021BE(PGM_MODEpinCD4021) == HIGH) //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
    {
      playMode = SELF_DRIVING_IR;
      //playMode = MENU_SYSTEM;
      //disp4char_trippleX("SDRC", 50);
    }
    //For Case 1, when NOT pressed tactile switch while power-on 
    //For Case 2, when the left-most slide switch is OFF while power-on
    else
    {
      //playMode = SELF_DRIVING_IR;
      playMode = MENU_SYSTEM;
      //disp4char_trippleX("MENU", 50);
    }
  }
  
  prevTime=millis();

  //*************************
  //*** To Test Motors while assembling
  //*************************
  //StandAloneTest_Setup();
  //StandAloneTest_L293D_RMotor_FWD();
  //*************************
  //*** (End) To Test Motors while assembling
  //*************************

}//setup

void playMain()
{
  curTime=millis();
  lapse=curTime-prevTime;
  
  //*************************
  //*** To Test Self-Driving Mode
  //*************************
  //playMode = SELF_DRIVING_IR;
  //*************************
  //*** (End) To Test Self-Driving Mode
  //*************************

  switch (playMode) 
  {
    case MENU_SYSTEM:
      //*************************
      //*** Run-time function ***
      //*************************
      menuSystem(); 
      
      //*************************
      //*** Test-time functions underneath
      //*************************
      //getXYZ(true);
      //printDirection();
      //loopDS1307 ();
      //StandAloneTest_L293D(); 
      //StandAloneTest_L293D_LMotor_FWD();
      //chkSpeedByInterrupt(true, false);
      //selfDriving_IRbased_obstacleAvoidance();
      //getPureIRsignal_Offseted(true, true);
      //getPureIRsignal(true, true);
      //testCD4021inputExpansion();
      //initEEPROM();
      //readSensitivityFromEEPROM(true);
      //allSeg_trippleX();
      //testSingleStage74HC595_7segLED();
      //testLeftMotor_293D();
      //test293D(); 
      //getPureIRsignal_SingleIR(true, false); 
      break;//case MENU_SYSTEM:
      
    case UX_PRG:
      UX_PRGfunc(); // Ux.ino
      break;//case UX_PRG:
      
    case SELF_DRIVING_IR:
      //*************************
      //*** Run-time function of Self-Driving***
      //*************************
      SELF_DRIVING_IRfunc(); // Menu.ino
      
      //*************************
      //*** Test-time functions underneath of Self-Driving
      //*************************
      //selfDriving_IRbased_obstacleAvoidance(); // IR.ino
      //testSingleIR_Motor(IR_FRONT);
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


/*
void testMRSpeed()
{
  goForward();
  checkSpeed();
  if ( abs(prevSpeed - travelSpeed) > 20 )
    disp4digit7seg_trippleX(travelSpeed, 0, 1);  
  //  disp4digit7seg(travelSpeed, 0);
}//testMRSpeedJ
*/

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
  //---- No RTC, so that manually set nightMode
  //nightMode=true; 
  //nightMode=false;
  //---- No RTC, so that manually set nightMode (End)
  
  //*** Production system
  //main menu system to play
  playMain();
  //*** Production system (End)
  
  //control74HC595(0,0,0);    

  //*** Test SDC
  //getPureIRsignal(true, true);
   //*** Test SDC (End)
  //togglePlayMode();
  
  //leggcyTest();
  
  //*** Function test for SeeedStudio PCB made by Insoo (version JAN 2019)
  //-_- NOT success on SeeedStudio PCB made by Insoo (version JAN 2019)
  //-_- NOT success on SeeedStudio PCB made by Insoo (version JAN 2019) (End)

  //testSingleIR_Motor(IR_FRONT);
  //frontUx_based_Control();
  
  //^^SUCESS on SeeedStudio PCB made by Insoo (version JAN 2019)^^
  
  //----- IR self-drive 1 (success as of Wed. Apr 3, 2019)
  //getPureIRsignal(true, true);
  //delay(500);
  //----- IR self-drive 1 (success as of Wed. Apr 3, 2019) (End)
  
  //------ CD4021 Input Expansion (success as of Tue. Apr 2, 2019)
  //------ 4021_DAT should connect to Q8 (pin3 of CD4021BE chip), NOT to Serial-In(pin11 of CD4021BE)
  // ref) https://www.arduino.cc/en/Tutorial/ShiftIn
  //byte swByte;
  //readCD4021BE(&swByte);
  //Serial.println(swByte , BIN);
  //delay(500);
  
  //testSingleReadCD4021BE();
  //testCD4021inputExpansion();
  //------ CD4021 Input Expansion (success as of Tue. Apr 2, 2019)(End)

  //------ IR Front, Left, RIght, and Cliff Front (as of Thu. Mar 28, 2019)
  //testSingleIR(IR_FRONT_CLIFF); //(success as of Thu. Mar 28, 2019)
  //testSingleIR(IR_FRONT); //(success as of Thu. Mar 28, 2019)
  //testSingleIR(IR_LEFT); //(success as of Wed. Mar 27, 2019)
  //testSingleIR(IR_RIGHT); //(success as of Wed. Mar 27, 2019)
  //------ IR Front, Left, RIght (as of Thu. Mar 28, 2019) (End)

  //------ L293D Right Motor (success as of Tue. Mar 26, 2019)
  //test293D();
  //testRightMotor_293D();
  //testX595_RFwd();
  //------ L293D Right Motor (success as of Tue. Mar 26, 2019) (End)

  //------ L293D Left Motor (success as of Tue. Mar 26, 2019)
  //testLeftMotor_293D();
  //testX595_LFwd();
  //------ L293D Left Motor (success as of Tue. Mar 26, 2019) (End)

  //------ Buzzer (success as of Tue. Mar 26, 2019)
  //bibibip();
  //beepWarning();
  //testBuzzer();
  //------ Buzzer (success as of Tue. Mar 26, 2019)(End)

  //------ RTC DS1307 (success as of Tue. Mar 26, 2019)
  /*
  dispTimeYYYYMMDD_DS1307_4digit7Seg();
  dispTimeHHMM_DS1307_4digit7Seg();
  displayTemp_4digit7seg();
  */
  //loopDS1307_4digit7Seg();
  //loopDS1307Serial();
  //------ RTC DS1307 (success as of Tue. Mar 26, 2019)(End)

  //------ Ux (success as of Tue. Mar 26, 2019)
  //measureUx(true, true, false);
  //delay(1000);
  //------ Ux (success as of Tue. Mar 26, 2019)

  //------ DHT11 Temperature & Humidity sensor (success as of Sun. Mar 24, 2019)
  //Show temperature(celcius) and humidity(relative %)
  //displayTemp_4digit7seg();
  
  //------ 4 digit 7 segment LED (success as of Sat. Mar 23, 2019)
  //show numbers from 0 to 9999
  //test0to9999();
  
  //show number'1234'
  //disp4digit7seg_trippleX(1234,0,50);
  
  //illuminate each segment of 4 digits 
  //allSeg_trippleX();
  
  //show numbers from 0 to 9 of 4 digits 
  //testSingleStage74HC595_7segLED();
  
  //show characters "Ford" for forward, "Back", "Left", and "Rite"
  //test4digit7segChar();  
  //------ 4 digit 7 segment LED (success as of Mar 23, 2019) (End)
  
  //------ 4 digit 7 segment LED (end)
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
