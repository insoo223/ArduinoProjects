/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 12 
 Trig to middle tripple X pin 7
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */


#define DISPTIME 5  //display time [second]

byte maximumRange = 250; // Maximum range needed (default 200)
byte minimumRange = 0; // Minimum range needed
int duration, distance; // Duration used to calculate distance


boolean showTimeOut = false;
byte showCnt=0;
//EEPROM address 256 to 1023 will be used.
//  0 to 255 is used for IR sensitivity storage
//  so that, EP_Cmd_BeginBlock should not be zero, only from 1 to 3 are valid.

#define EP_Cmd_BaseAdrs    256 
//EEPROM is a byte-wide storage, so that bigger than 256 places are accessible by block & offset
#define EP_Cmd_BeginBlock  0 
#define EP_Cmd_BeginOffset 1
#define EP_Cmd_EndBlock    2  
#define EP_Cmd_EndOffset   3  
#define EP_Cmd_NumAdrs     4  
#define EP_Cmd_1stAdrs     5  
#define EEPROMmaxAdrs      1023

#define marginUX   2   // [cm]
#define maxCmdIdx  10  // # of elements in the command set
#define maxUsrCmd  30  // max # of user command entry

//distances in centimeter[cm]
byte UXArray[maxCmdIdx+1] =
{ 
  5,  10, 15, 20,
  25, 30, 35, 40,
  45, 50, 55 };
enum CMD {
  CMD_BEGIN, CMD_FWD, CMD_LEFT, CMD_RIGHT, 
  CMD_TNCLK, CMD_TNANT, CMD_STOP, CMD_REV, 
  CMD_LOOPBEGIN, CMD_LOOPEND, CMD_END};
char strCMD[maxCmdIdx+1][5] = 
{
  "BEGN", "FORD", "LEFT", "RITE", 
  "TNCL", "TNAC", "STOP", "REVS", 
  "LBGN", "LEND", "CEND"
};         
byte usrCmdArray[maxUsrCmd], usrCmdIdx=0;          

boolean foundCMD_BEGIN=false, foundCMD_END=false;
boolean DONE_writeCmd2EEPROM=false;
boolean DONE_readCMD=false;

void UX_PRGfunc()
{
  if ( SingleReadCD4021BE(TactileButtonPinCD4021) == LOW) //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
    chkCMD_BEGIN();
    //chkCMD_END();
    //chkCMD();
    //ProgByUX();
}//UX_PRGfunc
//-----------------------------
void selfDriving_UXbased_obstacleAvoidance()
{
  IRmode = IR_RUNmode; //IR_RUNmode IR_SETmode
  testUx_Motor();
}//selfDriving_IRbased_obstacleAvoidance

//-----------------------------
void testUx_Motor()
{
  byte uxDist;

  //IR info
  /*------
   readSensitivityFromEEPROM();
   getPureIRsignal(false, false);
   setUnitAndRefFront();
   --------*/

  //Ux based control
  frontUx_based_Control();

  //disp4digit7seg_trippleX(uxDist, 0, DISP_4digit7seg_ITERATION_NORMAL);
  /*
  if (uxDist < 15)
   {
   // *** Left IR based control
   if (LpureRcv > LIR_ABS_TURN)
   {
   turnRight(RECT_ITERATION);
   }
   
   // *** Right IR based control
   if (RpureRcv > RIR_ABS_TURN)
   {
   turnLeft(RECT_TL_ITERATION);
   }
   }    
   */
  // *** Left IR based control
  /*----------
   if (LpureRcv > LIR_ABS_TURN)
   {
   turnRight(SHORT_TURN_ITERATION);
   }
   
   // *** Right IR based control
   if (RpureRcv > RIR_ABS_TURN)
   {
   turnLeft(SHORT_TURN_ITERATION);
   }
   -------------*/
}//testUx_Motor


//--- Front Ultrasound sensor based control  
void frontUx_based_Control()
{
  byte uxDist;

  uxDist=measureUx(false, false,false);
  //disp4digit7seg_trippleX(uxDist, 0, 30);
  // Being in far away from a front obstacle
  //if ((uxDist > ref_frontUx) || (uxDist == 5) || (uxDist == 6))
  if (uxDist > ref_frontUx)
  
  {
    goForward();
  }
  // Passing between turning point of "ref_front" 
  // and "goBackward()" criterion of "FIR_BW"
  else if (uxDist < Ux_BW)
  {
    decideTurnDirection();
  }
  // Crossing over "goBackward()" criterion of "FIR_BW"
  else
  {
    OffBothMotors();
    delay(500);
    goBackward();
    decideTurnDirection();
  }
}//frontUx_based_Control

//------------------------------------------------
void loopUx() 
{
  measureUx(false, true, false);
}//loop

// ************************
//---------------------------------------------        
//check BEGIN command
//---------------------------------------------        
bool chkCMD_BEGIN()
{
  byte cmdUX=0;

  //get distance as command  
  cmdUX = measureUx(false, false, false);

  if ( (cmdUX > (UXArray[CMD_BEGIN] - marginUX)) && (cmdUX < (UXArray[CMD_BEGIN] + marginUX)) )
   {
     //Serial.println("Found CMD_BEGIN"); //RAM shortage occured!
     //Serial.println(cmdUX);
     disp4digit7seg_trippleX(cmdUX,0,50);
     //disp4char_trippleX(strCMD[CMD_BEGIN], 50);
     return true;
   }
   else
   {
     //Serial.println("NOT yet found CMD_BEGIN");
     //Serial.println(cmdUX);
     //disp4char_trippleX("READ", 50);
     return false;   
   }
}//chkCMD_BEGIN()

//---------------------------------------------        
//check END command
//---------------------------------------------        
boolean chkCMD_END()
{
  byte cmdUX;
  
  //debouncing  
  delay(100);

  //get distance as command  
  cmdUX = measureUx(false, false, false);
  if ( (cmdUX < (UXArray[CMD_END] + marginUX)) &&
       (cmdUX > (UXArray[CMD_END] - marginUX)) )
  {
    //Serial.println(cmdUX);
    //Serial.println(UXArray[CMD_END] + marginUX);
    //Serial.println("Found CMD_END");
    //disp4char_trippleX(strCMD[CMD_END], 50);
    disp4digit7seg_trippleX(cmdUX,0,50);
    return true;
  }
  else
  {
    /*
    Serial.println(cmdUX);
    Serial.println(UXArray[CMD_END] + marginUX);
    Serial.println("NOT yet found CMD_END");
    */
    //digitalWrite(PGM_END_LEDpin, LOW);
    return false;   
  }
  //delay(500);
}//chkCMD_END()

//---------------------------------------------        
//check ordinary user command, like GO FORWARD, BACKWARD, TURN LEFT, etc.
//---------------------------------------------        
void chkCMD()
{
  byte cmdUX, cmdIdx;
  
  //to diminish noise influence
  delay(100);
  //get distance as command  
  cmdUX = measureUx(false, true, false);
  if (cmdUX < MAX_DISTANCE_UXArray)
  {
    //Serial.print("cmdUX");
    //Serial.println(cmdUX);
    delay(100);
    cmdIdx = getCmdIdx(cmdUX);
    if ((usrCmdIdx < maxUsrCmd) && (cmdIdx != 120))
    {
      //digitalWrite(PGM_CMD_LEDpin, HIGH);
      //disp4char_trippleX(strCMD[cmdIdx], 50);
      usrCmdArray[usrCmdIdx] = cmdIdx;
      disp4digit7seg_trippleX(cmdIdx,0,50);
      //Serial.println(cmdIdx);
      //Serial.println("Found CMD");
      //Serial.print("strCMD");
      //Serial.println(strCMD[cmdIdx]);
      usrCmdIdx++;
      delay(1000);
      //digitalWrite(PGM_CMD_LEDpin, LOW);
      //delay(500);
    }//if ((usrCmdIdx < maxUsrCmd) && (cmdIdx != -1))

    if (cmdIdx == CMD_END)
    {
      //digitalWrite(PGM_END_LEDpin, HIGH);
      //disp4char_trippleX(strCMD[cmdIdx], 50);
      //for (byte i=0; i<usrCmdIdx; i++)
        //disp4digit7seg_trippleX(usrCmdArray[i],0,50);
        //Serial.println(usrCmdArray[i]);
      disp4digit7seg_trippleX(100,0,50);
      foundCMD_END = true;
    }
    else
    {
      //digitalWrite(PGM_END_LEDpin, LOW);
      foundCMD_END = false;
    }
  }//if (cmdUX < MAX_DISTANCE_UXArray)
  
}//chkCMD()

//--------------------------------------------- 
byte getCmdIdx(byte valUX)
{
  boolean foundCMD=false;
  byte i=0;
  
  while ((!foundCMD) && (i < maxCmdIdx+1))
  {
    if ( (valUX > (UXArray[i] - marginUX)) && (valUX < (UXArray[i] + marginUX)) )
    {
      foundCMD = true;
      return i;
      //break;
    }
    else
      i++;
  }//while ((!foundCMD) && (i < maxCmdIdx))
  
  return 120;
}//getCmdIdx

//---------------------------------------------        
//---------------------------------------------        
void ProgByUX()
{
  if (!foundCMD_BEGIN)
  {
    foundCMD_BEGIN = chkCMD_BEGIN();
    if (foundCMD_BEGIN) 
      disp4digit7seg_trippleX(100, 0, 200);
    delay(200);
  }//if (!foundCMD_BEGIN)
  else
  {
    if (!foundCMD_END)
      foundCMD_END = chkCMD_END();
      
    //if ((!foundCMD_END) && (digitalRead(modeOPswitch) == LOW))
    if (!foundCMD_END)
    {
      chkCMD();
    }//if (!foundCMD_END)
    
    if (foundCMD_END)
    {
      disp4digit7seg_trippleX(300, 0, 200);
      //doUsrLoop();
    }//if (foundCMD_END == true)
  }//else (foundCMD_BEGIN == true)
}//ProgByUX
//------------------------------------------------
// ************************

//------------------------------------------------
//bool measureUx_SerialDebug = false; //true false

byte measureUx(bool DebugSerial, bool Debug7seg, bool dispUpsideDown)
{
  if (DebugSerial)
    Serial.println("measureUx: ");
  /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */
  //digitalWrite(trigPin, LOW); 
  //Off74HC595_Middle(FRIEND_MODE, Ux_TRIG);
  trippleXwrite(Ux_TRIG, LOW);
  delayMicroseconds(2); 

  //digitalWrite(trigPin, HIGH);
  //On74HC595_Middle(FRIEND_MODE, Ux_TRIG);
  trippleXwrite(Ux_TRIG, HIGH);
  delayMicroseconds(10); 

  //digitalWrite(trigPin, LOW); 
  //Off74HC595_Middle(FRIEND_MODE, Ux_TRIG);
  trippleXwrite(Ux_TRIG, LOW);

  //duration = pulseIn_CD4021(UxECHOpinCD4021, HIGH);
  duration = pulseIn(UxECHOpin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  if (Debug7seg)
  {
    if (dispUpsideDown)
      disp4digit7seg_UpsideDown_trippleX(distance, 0, 50);
    else
      disp4digit7seg_trippleX(distance, 0, 50);
  }
  if (DebugSerial)
  {
    Serial.print("distance: ");
    Serial.println(distance);
  }

  if (distance >= maximumRange || distance <= minimumRange)
  {
    distance = 40;
    // Send a negative number to computer and Turn LED ON 
    //to indicate "out of range" 
    if (DebugSerial)
      Serial.println("-1");
    //digitalWrite(LEDPin, HIGH); 
  }

  return (distance);
}//measureUx  
