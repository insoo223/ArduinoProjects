//**************************************************************//
//  Name    : IR.ino
//  Author  : Insoo Kim
//  Create  : Oct 8, 2015 
//  Update  : Mar 28, 2019    
//  Notes   : Infra-Red distance sensor control module for SDC_Tandem328.ino 
//            Self-driving car on a single perforated board with w/ATmega328P in parallel with Arduino Pro Mini
//            Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//****************************************************************
#include "defs.h"
#include "externs.h"
#include <EEPROM.h>

//-----------------------------
void selfDriving_IRbased_obstacleAvoidance()
{
  IRmode = IR_RUNmode; //IR_RUNmode IR_SETmode
  testIRs_Motor();
  //chkSpeedByInterrupt(false, true);
}//selfDriving_IRbased_obstacleAvoidance

//-----------------------------
// check voltage with oscilloscope
void sensitivityCheckIR_ForOSC()
{
  //--- Tunr ON ir sensor(s)
  //ThreeOn74HC595_Middle(FRIEND_MODE, IR_FRONT, IR_LEFT, IR_RIGHT);
  trippleXwrite(IR_FRONT, HIGH);
  trippleXwrite(IR_LEFT, HIGH);
  trippleXwrite(IR_RIGHT, HIGH);
  delay(500);

  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(IR_FRONT, LOW);
  trippleXwrite(IR_LEFT, LOW);
  trippleXwrite(IR_RIGHT, LOW);
  delay(500);
}//sensitivityCheckIR_ForOSC

//-----------------------------
bool done_readSensitivityFromEEPROM = false;
void readSensitivityFromEEPROM(bool SerialDebug)
{
  if (!done_readSensitivityFromEEPROM)
  {
    int startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR) - 3;
    
    FIR30 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_FRONT_OFFSET);
    //FIR10 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_FRONT_OFFSET);
    delay(EEPROM_READ_SAFEMARGIN);
    LIR30 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_LEFT_OFFSET);
    //LIR10 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_LEFT_OFFSET);
    delay(EEPROM_READ_SAFEMARGIN);
    RIR30 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET);
    //RIR10 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET);
    delay(EEPROM_READ_SAFEMARGIN);
    
    if (SerialDebug)
    {
       Serial.print("startAdrs = EEPROM: "); Serial.println(startAdrs);
       Serial.print("FIR30 = EEPROM: "); Serial.println(FIR30);
       Serial.print("LIR30 = EEPROM: "); Serial.println(LIR30);
       Serial.print("RIR30 = EEPROM: "); Serial.println(RIR30);
       /*
       Serial.print("FIR10 = EEPROM: "); Serial.println(FIR10);
       Serial.print("LIR10 = EEPROM: "); Serial.println(LIR10);
       Serial.print("RIR10 = EEPROM: "); Serial.println(RIR10);
       */
    }
    
    done_readSensitivityFromEEPROM = true;
  }
}//readSensitivityFromEEPROM

//-----------------------------
void testIRs_Motor()
{
  int uxDist;

  readSensitivityFromEEPROM(false); //true false
  //delay(1000);
  
  getPureIRsignal(false, false);
  //getPureIRsignal_Offseted(false, false);
  //delay(1000);

  if (REF_DIST == 30)
    setUnitAndRefFront30();
  if (REF_DIST == 10)
    setUnitAndRefFront10();
  
  frontIR_based_Control();

  
  // *** Left IR based control
  if (LpureRcv > LIR_ABS_TURN)
  {
    turnRight(SHORT_TURN_ITERATION);
    prevMode = TURN_RIGHT_MODE;
  }

  // *** Right IR based control
  if (RpureRcv > RIR_ABS_TURN)
  {
    turnLeft(SHORT_TURN_ITERATION);
    prevMode = TURN_LEFT_MODE;
  }
  
}//testIRs_Motor

//-----------------------------------
bool debug2Serial = true; // false true
bool debugUnitDist = true; // false true


void testIRs_ADC()
{
  //getPureIRsignal(false, false);
  getPureIRsignal(true, true);
  //getPureIRsignal_Offseted(true, true);
  
  /////setIR_RefVal_Turn();  
  setIR_RefVal_Turn_Auto(true, true); // write to EEPROM / line 389 in the printed code of IR.ino
  
  setUnitAndRefFront30();
  //setUnitAndRefFront10();
  
  frontIR_based_Control();
}//testIRs_ADC

//--------------------- Front IR based control  
// line 102 of the printed code IR.ino
void frontIR_based_Control()
{
  // Being in far away from a front obstacle
  if (FpureRcv < ref_front)
  {
    if (IRmode == IR_SETmode)
      //Show FWD_MODE "0000" without DP, meaning "goForward()"
      //disp4digit7seg(FWD_MODE, 0);
      ;
    else
    {    
      goForward();
      /*
      checkSpeed();
      if ( abs(prevSpeed - travelSpeed) > 20 )
        disp4digit7seg_trippleX(travelSpeed, 0, 1);  
      */
    }
      
  }
  // Passing between turning point of "ref_front" 
  // and "goBackward()" criterion of "FIR_BW"
  else if (FpureRcv < FIR_BW)
  {
    decideTurnDirection();
    //disp4digit7seg_trippleX(2000+FpureRcv, 0, 50);
  }
  // Crossing over "goBackward()" criterion of "FIR_BW"
  else
  {
    if (IRmode == IR_SETmode)
      // Show BWD_MODE "3000" without DP, meaning "goBackward()"
      //disp4digit7seg(BWD_MODE, 0);
      ;
    else
    {
      goBackward();
      decideTurnDirection();
      disp4digit7seg_trippleX(9000+FpureRcv, 0, 50);
    }
  }
}//frontIR_based_Control


//-----------------------------
void smartTurnLeft()
{
  if (prevMode == TURN_LEFT_MODE)
    cntTURN_LEFT_MODE++;
  if (cntTURN_LEFT_MODE == MAX_TURN_LEFT_IN_A_ROW)
  {
    OffBothMotors();
    delay(500);
    goBackward();
    turnLeft(RECT_TL_ITERATION);
    cntTURN_LEFT_MODE=0;
    prevMode= -1;
  }
  else
  {
    turnLeft(RECT_TL_ITERATION);
    prevMode = TURN_LEFT_MODE;
  }

}//smartTurnLeft

//-----------------------------
void smartTurnRight()
{
  if (prevMode == TURN_RIGHT_MODE)
    cntTURN_RIGHT_MODE++;
  if (cntTURN_RIGHT_MODE == MAX_TURN_RIGHT_IN_A_ROW)
  {
    OffBothMotors();
    delay(STOP_DURATION);
    goBackward();
    turnRight(RECT_ITERATION);
    cntTURN_RIGHT_MODE=0;
    prevMode= -1;
  }
  else
  {
    turnRight(RECT_ITERATION);
    prevMode = TURN_RIGHT_MODE;
  }

}//smartTurnRight

//-----------------------------
// Decide turning diection based on the weighted distance of left & right IR
void decideTurnDirection()
{
  // When the IR sensor is closer to Left side object than the right side one
  //disp4digit7seg_trippleX(4000+Ldist, 0, 50);
  //disp4digit7seg_trippleX(5000+Rdist, 0, 50);

  if (Ldist > Rdist) 
  {
    if (IRmode == IR_SETmode)
      // Show TURN_RIGHT_MODE "2000" without DP, meaning "turnRight(RECT_ITERATION)"
      //disp4digit7seg(TURN_RIGHT_MODE, 0);
      ;
    else
      smartTurnRight();
  }
  //when (Ldist =< Rdist), meaning the IR sensor is closer to Right side object than the left side one
  else 
  {
    if (IRmode == IR_SETmode)
      // Show TURN_LEFT_MODE "1000" without DP, meaning "turnRight(RECT_ITERATION)"
      //disp4digit7seg(TURN_LEFT_MODE, 0);
      ;
    else
      smartTurnLeft();
  }
  
  if (IRmode == IR_SETmode)
  {
    if (debug2Serial)
    {
      if (Ldist > Rdist)   
        Serial.println("TURN_RIGHT_MODE: Ldist & Rdist = ");
      else
        Serial.println("TURN_LEFT_MODE: Ldist & Rdist = ");
      Serial.print(Ldist);
      Serial.print(", ");
      Serial.println(Rdist);
    }
    
    if (debugUnitDist)
    {
      // FYI, Show weighted distance of Left & Right side IRs
      //disp4digit7seg(Ldist, 1);
      //disp4digit7seg(Rdist, 2);
    }
  }
}//decideTurnDirection

//-----------------------------
void setUnitAndRefFront10()
{
  //refer "globals.h" for Lunit, Runit, Ldist, Rdist, and ref_front
  if (REF_DIST == 10) // refer defs.h
  {
    // ADC for unit distance within the range of 0cm to 10cm away from an obstacle.
    // It assumes linearity of IR sensitivity in the corresponding range. 
    // You'd got more precise assumption after checking the signal sensitivity curvarture by experiment
    Lunit = 3000/(MAX_ADC - LIR10); // [0.1mm/ADC]
    Runit = 3000/(MAX_ADC - RIR10); // [0.1mm/ADC]

    //Ldist & Rdist will be used to decide turning direction at the distance of 10cm between the car & an obstacle
    // Ldist & Rdist will be increased if the car is closer to an obstacle
    // Weighted distance measured by the Left IR
    Ldist = (LpureRcv - LIR10) * Lunit;
    // Weighted distance measured by the Right IR
    Rdist = (RpureRcv - RIR10) * Runit;
  }//if (REF_DIST == 10)
  

  if (REF_DIST == 30)
    ref_front = FIR30;

  if (REF_DIST == 10)
    ref_front = FIR10;
    
  //Serial.println(ref_front);  

}//setUnitAndRefFront10

//-----------------------------
void setUnitAndRefFront30()
{
  //refer "globals.h" for Lunit, Runit, Ldist, Rdist, and ref_front
  if (REF_DIST == 30)
  {
    // ADC for unit distance within the range of 0cm to 30cm away from an obstacle.
    // It assumes linearity of IR sensitivity in the corresponding range. 
    // You'd got more precise assumption after checking the signal sensitivity curvarture by experiment
    Lunit = 3000/(MAX_ADC - LIR30); // [0.1mm/ADC]
    Runit = 3000/(MAX_ADC - RIR30); // [0.1mm/ADC]

    //Ldist & Rdist will be used to decide turning direction at the distance of 30cm between the car & an obstacle
    // Ldist & Rdist will be increased if the car is closer to an obstacle
    // Weighted distance measured by the Left IR
    Ldist = abs((LpureRcv - LIR30) * Lunit);
    // Weighted distance measured by the Right IR
    Rdist = abs((RpureRcv - RIR30) * Runit);
  }//if (REF_DIST == 30)
  

  if (REF_DIST == 30)
    ref_front = FIR30;
    
  //Serial.println(ref_front);  

}//setUnitAndRefFront30

//-----------------------------
bool doneInitEEPROM = false;
void initEEPROM()
{
    while (!doneInitEEPROM)
    {
      disp4char_trippleX("INIT", 100);
      disp4char_trippleX("EEPR", 100);
      
      for (int i=0; i<1023; i++)
      {
        disp4digit7seg_trippleX(i,0,1);
        if (i==0)
          EEPROM.write(i,1);
        else
          EEPROM.write(i,0);
        delay(1);
        
      }

      doneInitEEPROM = true;

      disp4char_trippleX("DONE", 100);
      disp4char_trippleX("INIT", 100);
    }// while (!doneInitEEPROM)
    
}//initEEPROM

//-----------------------------
//bool setIR_RefVal_Turn_Auto_SerialDebug = true; //false true
//bool setIR_RefVal_Turn_Auto_disp4digit7seg = true; //false true

//-----------------------------
// the value of ref30 should be same or within 255.
void setOneIR_Sensitivity(byte loc, bool SerialDebug, bool disp4digit7seg)
{
  int startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR);
  char strDirection[4];
  int *ref30, *ref15, *ref10, expRcv, EEPROMoffset;
  int dispPrefix;
  char target[50];
  
  switch (loc)
  {
    case IR_FRONT:
      strcpy(strDirection, STR_FRNT);
      ref30 = &FIR30;
      ref15 = &FIR15;
      ref10 = &FIR10;
      expRcv = FpureRcv;
      EEPROMoffset = EEPROM_IR_SENSITIVITY_FRONT_OFFSET;
      dispPrefix = PREFIX_DISP_IR_FRONT;
      strcpy(target, "Front IR sensitivity");
      break;

    case IR_LEFT:
      strcpy(strDirection, STR_LEFT);
      ref30 = &LIR30;
      ref15 = &LIR15;
      ref10 = &LIR10;
      expRcv = LpureRcv;
      EEPROMoffset = EEPROM_IR_SENSITIVITY_LEFT_OFFSET;
      dispPrefix = PREFIX_DISP_IR_LEFT;
      strcpy(target, "Left IR sensitivity");
      break;

    case IR_RIGHT:
      strcpy(strDirection, STR_RITE);
      ref30 = &RIR30;
      ref15 = &RIR15;
      ref10 = &RIR10;
      expRcv = RpureRcv;
      EEPROMoffset = EEPROM_IR_SENSITIVITY_RIGHT_OFFSET;
      dispPrefix = PREFIX_DISP_IR_RIGHT;
      strcpy(target, "Right IR sensitivity");
      break;
  }//switch (loc)
      
  // set the Front IR sensitivity
  ////disp4digit7seg(FpureRcv, 0);
  //disp4char_trippleX(STR_FRNT, 50);
  disp4char_trippleX(strDirection, 50);
  delay(SET_IR_SENSITIVITY_READY_ON_DURATION);
  disp4char_trippleX(BLANK4CHAR, 50);
  delay(SET_IR_SENSITIVITY_READY_OFF_DURATION);
  
  //if reference distance is 30cm (default), 
  //  measurement distance should be at 30cm away from an object
  //Currently, no other REF_DIST except 30.
  if (REF_DIST == 30)
  {
    //FpureRcv is a global variable attained thru "getPureIRsignal()"
    // implying the function "getPureIRsignal()" should be called beforhand.
    *ref30 = expRcv;
    
    //ADCdiff_30_15cm is a rule of thumb compensation, 
    //  need to experiment & measure further
    *ref15 = *ref30 + ADCdiff_30_15cm;
  }

  if (REF_DIST == 10)
  {
    //FpureRcv is a global variable attained thru "getPureIRsignal()"
    // implying the function "getPureIRsignal()" should be called beforhand.
    *ref10 = expRcv;
    
    //ADCdiff_30_15cm is a rule of thumb compensation, 
    //  need to experiment & measure further
    *ref15 = *ref10;
  }
  disp4char_trippleX(STR_DONE, 50);
  disp4char_trippleX(strDirection, 50);
  delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
  disp4char_trippleX(BLANK4CHAR, 50);    
  delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);

  //--- write to EEPROM, the calibrated sensitivity of the corresponding IR
  if (REF_DIST == 10)
    EEPROM.write(startAdrs + EEPROMoffset, *ref10); 
  if (REF_DIST == 30)
    EEPROM.write(startAdrs + EEPROMoffset, *ref30); 

  delay(50);
  if (SerialDebug)
  {
    Serial.println("-----"); 
    Serial.println(target); 
    Serial.print("startAdrs + EEPROMoffset: "); Serial.println(startAdrs + EEPROMoffset);
    Serial.print("IR sensitivity in EEPROM: "); Serial.println(EEPROM.read(startAdrs + EEPROMoffset));
    if (REF_DIST == 30)
      Serial.print("Updated IR sensitivity: "); Serial.println(*ref30);
    if (REF_DIST == 10)
      Serial.print("Updated IR sensitivity: "); Serial.println(*ref10);
  }

  if (disp4digit7seg)
    disp4digit7seg_trippleX(dispPrefix + EEPROM.read(startAdrs + EEPROMoffset), 0, DISP_4digit7seg_ITERATION_NORMAL);

}//setOneIR_Sensitivity

//-----------------------------
// Set IR sensitivity of front, left, and right IRs in a row.
// line 389 of the printed code of IR.ino
void setIR_RefVal_Turn_Auto(bool SerialDebug, bool disp4digit7seg)
{
  int startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR);

  if (SerialDebug)
  {
    Serial.print("startAdrs: ");
    Serial.println(startAdrs);
  }

  if (disp4digit7seg)
    disp4digit7seg_trippleX(startAdrs, 0, DISP_4digit7seg_ITERATION_NORMAL);
    
  // set IR sensitivity
  setOneIR_Sensitivity(IR_FRONT, SerialDebug, disp4digit7seg);
  setOneIR_Sensitivity(IR_LEFT, SerialDebug, disp4digit7seg);
  setOneIR_Sensitivity(IR_RIGHT, SerialDebug, disp4digit7seg);

  //--- write to EEPROM, the updated startAdrs
  if ( (startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET+1) <= EEPROM_IR_SENSITIVITY_MAX_ADDR )
    EEPROM.write(EEPROM_IR_SENSITIVITY_ADDR_PTR, startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET+1);
  else
    EEPROM.write(EEPROM_IR_SENSITIVITY_ADDR_PTR, 1);

  if (SerialDebug)
  {
    Serial.print("Done: ");
    Serial.println("Tuning IR sensitivity Completed1");
  }

  //finished tuning sensitivity  
  disp4char_trippleX("DONE", 100);
  
  delay(SET_IR_SENSITIVITY_NEXT_DURATION);
}//setIR_RefVal_Turn_Auto

//------------------------------
// Added by Insoo Kim 
// On Friday Jan 04, 2019
void getPureIRsignal_Offseted(bool DebugSerial, bool Debug4digit7seg)
{
  
  //--- Tunr ON ir sensor(s)
  //ThreeOn74HC595_Middle(FRIEND_MODE, IR_FRONT, IR_LEFT, IR_RIGHT);
  trippleXwrite(IR_FRONT, HIGH);
  trippleXwrite(IR_LEFT, HIGH);
  trippleXwrite(IR_RIGHT, HIGH);

  // Give some break to raise its full power of IR beam, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);

  // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
  // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
  FcombinedRcv=analogRead(IR_Rcv_FRONTpin);
  LcombinedRcv=analogRead(IR_Rcv_LEFTpin);
  RcombinedRcv=analogRead(IR_Rcv_RIGHTpin);

  //--- Turn OFF IR sensors
  // SOLE_MODE turns off itself and all the other bits, so that effectively off all IRs.
  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(IR_FRONT, LOW);
  trippleXwrite(IR_LEFT, LOW);
  trippleXwrite(IR_RIGHT, LOW);
  
  // Give some break to cool down their IR beam emission, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);
  
  // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
  // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
  FambientRcv=analogRead(IR_Rcv_FRONTpin);  
  LambientRcv=analogRead(IR_Rcv_LEFTpin);
  RambientRcv=analogRead(IR_Rcv_RIGHTpin);

  // Now, we can get the pure signal strengh of our IR LEDs!
  FpureRcv=FcombinedRcv-FambientRcv;
  LpureRcv=LcombinedRcv-LambientRcv;
  RpureRcv=RcombinedRcv-RambientRcv;

  FpureRcv -= IR_SIG_DEDUCTION; // IR_SIG_DEDUCTION is determined by experiment done on Jan 4, 2019
  //ADC value normalization
  if (FpureRcv > 255)
    FpureRcv = 255;
  else if (FpureRcv > 0) 
    ;
  else
    FpureRcv = 0;

  LpureRcv -= IR_SIG_DEDUCTION; // IR_SIG_DEDUCTION is determined by experiment done on Jan 4, 2019
  if (LpureRcv > 255)
    LpureRcv = 255;
  else if (LpureRcv > 0)
    ;
  else
    LpureRcv = 0;

  RpureRcv -= IR_SIG_DEDUCTION; // IR_SIG_DEDUCTION is determined by experiment done on Jan 4, 2019
  if (RpureRcv > 255)
    RpureRcv = 255;
  else if (RpureRcv > 0)
    ;
  else
    RpureRcv = 0;

    
  if (DebugSerial)
  {
    Serial.print("F Cmb Amb Pure: "); Serial.print(FcombinedRcv); Serial.print(", "); Serial.print(FambientRcv); Serial.print(", "); Serial.println(FpureRcv);
    Serial.print("L Cmb Amb Pure: "); Serial.print(LcombinedRcv); Serial.print(", "); Serial.print(LambientRcv); Serial.print(", "); Serial.println(LpureRcv);
    Serial.print("R Cmb Amb Pure: "); Serial.print(RcombinedRcv); Serial.print(", "); Serial.print(RambientRcv); Serial.print(", "); Serial.println(RpureRcv);
    Serial.println("----------------------------------");
  }
  
  if (Debug4digit7seg)
  {
    disp4digit7seg_trippleX(1000+FpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    disp4digit7seg_trippleX(2000+LpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    disp4digit7seg_trippleX(3000+RpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
  }    
}//getPureIRsignal_Offseted


//------------------------------
// Get pure IR rx signals of Front, Left, Right bounced back from IR tx signals of each 
//    deducted by ambient IR noise
// The ADC values of pure IR rx signals are assigned to their corresponding global variables.
// The global variables of pure IR rx signals are used to determine self-driving operation.
// Updated by Insoo: Wed Apr 3, 2019 
void getPureIRsignal(bool DebugSerial, bool Debug4digit7seg)
{
  //--- Tunr ON ir sensor(s)
  //ThreeOn74HC595_Middle(FRIEND_MODE, IR_FRONT, IR_LEFT, IR_RIGHT);
  trippleXwrite(IR_FRONT, HIGH);
  trippleXwrite(IR_LEFT, HIGH);
  trippleXwrite(IR_RIGHT, HIGH);

  // Give some break to raise its full power of IR beam, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);

  // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
  // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
  FcombinedRcv=analogRead(IR_Rcv_FRONTpin);
  LcombinedRcv=analogRead(IR_Rcv_LEFTpin);
  RcombinedRcv=analogRead(IR_Rcv_RIGHTpin);

  //--- Turn OFF IR sensors
  // SOLE_MODE turns off itself and all the other bits, so that effectively off all IRs.
  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(IR_FRONT, LOW);
  trippleXwrite(IR_LEFT, LOW);
  trippleXwrite(IR_RIGHT, LOW);
  
  // Give some break to cool down their IR beam emission, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);
  
  // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
  // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
  FambientRcv=analogRead(IR_Rcv_FRONTpin);  
  LambientRcv=analogRead(IR_Rcv_LEFTpin);
  RambientRcv=analogRead(IR_Rcv_RIGHTpin);

  // Now, we can get the pure signal strengh of our IR LEDs!
  FpureRcv=abs(FcombinedRcv-FambientRcv);
  LpureRcv=abs(LcombinedRcv-LambientRcv);
  RpureRcv=abs(RcombinedRcv-RambientRcv);

  if (DebugSerial)
  {
    Serial.print("F Cmb Amb Pure: "); Serial.print(FcombinedRcv); Serial.print(", "); Serial.print(FambientRcv); Serial.print(", "); Serial.println(FpureRcv);
    Serial.print("L Cmb Amb Pure: "); Serial.print(LcombinedRcv); Serial.print(", "); Serial.print(LambientRcv); Serial.print(", "); Serial.println(LpureRcv);
    Serial.print("R Cmb Amb Pure: "); Serial.print(RcombinedRcv); Serial.print(", "); Serial.print(RambientRcv); Serial.print(", "); Serial.println(RpureRcv);
    Serial.println("----------------------------------");
  }
  
  if (Debug4digit7seg)
  {
    disp4digit7seg_trippleX(1000+FpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    disp4digit7seg_trippleX(2000+LpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    disp4digit7seg_trippleX(3000+RpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
  }    
}//getPureIRsignal


//------------------------------
//added by Insoo (Mar27, 2019)
void testSingleIR(byte loc)
{
  getPureIRsignal_Single(loc, true, true);
  delay(200);
}//testSingleIR

//------------------------------
//added by Insoo (Mar27, 2019)
void getPureIRsignal_Single(byte loc, bool DebugSerial, bool Debug4digit7seg)
{
  
  //--- Tunr ON ir sensor(s)
  trippleXwrite(loc, HIGH);
      
  // Give some break to raise its full power of IR beam, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);

  // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
  // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
  switch(loc)
  { 
    case IR_FRONT:
      FcombinedRcv=analogRead(IR_Rcv_FRONTpin);
      break;
    case IR_LEFT:
      LcombinedRcv=analogRead(IR_Rcv_LEFTpin);
      break;
    case IR_RIGHT:
      RcombinedRcv=analogRead(IR_Rcv_RIGHTpin);
      break;
    case IR_FRONT_CLIFF:
      CliffFcombinedRcv=analogRead(IR_Rcv_CLIFF_FRONTpin);
      break;
  }

  //--- Turn OFF IR sensors
  // SOLE_MODE turns off itself and all the other bits, so that effectively off all IRs.
  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(loc, LOW);
  
  // Give some break to cool down their IR beam emission, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);
  
  // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
  // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
  switch(loc)
  { 
    case IR_FRONT:
      FambientRcv=analogRead(IR_Rcv_FRONTpin);  
      break;
    case IR_LEFT:
      LambientRcv=analogRead(IR_Rcv_LEFTpin);
      break;
    case IR_RIGHT:
      RambientRcv=analogRead(IR_Rcv_RIGHTpin);
      break;
    case IR_FRONT_CLIFF:
      CliffFambientRcv=analogRead(IR_Rcv_CLIFF_FRONTpin);
      break;
  }

  // Now, we can get the pure signal strengh of our IR LEDs!
  switch(loc)
  { 
    case IR_FRONT:
      FpureRcv=abs(FambientRcv-FcombinedRcv); 
      break;
    case IR_LEFT:
      LpureRcv=abs(LambientRcv-LcombinedRcv);
      break;
    case IR_RIGHT:
      RpureRcv=abs(RambientRcv-RcombinedRcv);
      break;
    case IR_FRONT_CLIFF:
      CliffFpureRcv=abs(CliffFambientRcv-CliffFcombinedRcv);
      //if cliff is detected, then make noise "bibibip"
      /*
      if (CliffFpureRcv < IR_FRONT_CLIFF_THRESHOLD) 
        bibibip();
      break;
      */
  }
  
  if (DebugSerial)
  {
    switch(loc)
    { 
      case IR_FRONT:
        Serial.print(FcombinedRcv); Serial.print(", "); Serial.print(FambientRcv); Serial.print(", "); Serial.println(FpureRcv);
        break;
      case IR_LEFT:
        Serial.print(LcombinedRcv); Serial.print(", "); Serial.print(LambientRcv); Serial.print(", "); Serial.println(LpureRcv);
        break;
      case IR_RIGHT:
        Serial.print(RcombinedRcv); Serial.print(", "); Serial.print(RambientRcv); Serial.print(", "); Serial.println(RpureRcv);
        break;
      case IR_FRONT_CLIFF:
        Serial.print(CliffFcombinedRcv); Serial.print(", "); Serial.print(CliffFambientRcv); Serial.print(", "); Serial.println(CliffFpureRcv);
        break;
    }
  }
  
  if (Debug4digit7seg)
  {
    switch(loc)
    { 
      case IR_FRONT:
        disp4digit7seg_trippleX(1000+FpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
        break;
      case IR_LEFT:
        disp4digit7seg_trippleX(2000+LpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
        break;
      case IR_RIGHT:
        disp4digit7seg_trippleX(3000+RpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
        break;
      case IR_FRONT_CLIFF:
        disp4digit7seg_trippleX(4000+CliffFpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
        break;
    }
  }    
}//getPureIRsignal_Single

//-----------------------------------------------------------------------
void getPureIRsignal_SingleIR(bool DebugSerial, bool Debug4digit7seg)
{
  //--- Tunr ON ir sensor(s)
  //ThreeOn74HC595_Middle(FRIEND_MODE, IR_FRONT, IR_LEFT, IR_RIGHT);
    trippleXwrite(IR_FRONT, HIGH);
    trippleXwrite(IR_LEFT, HIGH);
    trippleXwrite(IR_RIGHT, HIGH);

  // Give some break to raise its full power of IR beam, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);

  // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
  // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
  FcombinedRcv=analogRead(IR_Rcv_FRONTpin);
  LcombinedRcv=analogRead(IR_Rcv_LEFTpin);
  RcombinedRcv=analogRead(IR_Rcv_RIGHTpin);

  //--- Turn OFF IR sensors
  // SOLE_MODE turns off itself and all the other bits, so that effectively off all IRs.
  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(IR_FRONT, LOW);
  trippleXwrite(IR_LEFT, LOW);
  trippleXwrite(IR_RIGHT, LOW);
  
  // Give some break to cool down their IR beam emission, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);
  
  // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
  // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
  FambientRcv=analogRead(IR_Rcv_FRONTpin);  
  LambientRcv=analogRead(IR_Rcv_LEFTpin);
  RambientRcv=analogRead(IR_Rcv_RIGHTpin);

  // Now, we can get the pure signal strengh of our IR LEDs!
  FpureRcv=FcombinedRcv-FambientRcv;
  LpureRcv=LcombinedRcv-LambientRcv;
  RpureRcv=RcombinedRcv-RambientRcv;

  if (DebugSerial)
  {
    if (SingleReadCD4021BE(PGM_MODEpinCD4021) == LOW)
    {
      //Serial.print("F Cmb Amb Pure: "); Serial.print(FcombinedRcv); Serial.print(", "); Serial.print(FambientRcv); Serial.print(", "); 
      Serial.println(FpureRcv);
    }
    if (SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == HIGH)
    {
      Serial.print("L Cmb Amb Pure: "); Serial.print(LcombinedRcv); Serial.print(", "); Serial.print(LambientRcv); Serial.print(", "); Serial.println(LpureRcv);
    }
    if (SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == HIGH)
    {
      Serial.print("R Cmb Amb Pure: "); Serial.print(RcombinedRcv); Serial.print(", "); Serial.print(RambientRcv); Serial.print(", "); Serial.println(RpureRcv);
    }
    //Serial.println("----------------------------------");
  }
  
  if (Debug4digit7seg)
  {
    if ((SingleReadCD4021BE(PGM_MODEpinCD4021) == LOW) && (SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == LOW))
      disp4digit7seg_trippleX(1000+FpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    if ((SingleReadCD4021BE(PGM_MODEpinCD4021) == HIGH) && (SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == LOW))
      disp4digit7seg_trippleX(2000+LpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    if (SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == HIGH)
      disp4digit7seg_trippleX(3000+RpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
  }    
}//getPureIRsignal_Front
