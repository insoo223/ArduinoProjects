/**************************************************************
  Target MCU: Arduino UNO (ATmega328P)Í
  Clock type: External as in Arduino UNO
  Clock speed: 16Mhz as of Arduino UNO 16Mhz 5V version
  Name    : CodingByThing.ino
  Author  : Insoo Kim (insoo@hotmail.com)
  Date    : Thu Aug 06, 2015
  Update  : Sun Aug 09, 2015
  Objective: Program self-driving car by Walnut or any thing. Code by Thing!
  Desc    : This is an accompanying code module for WalnutPgmCar.ino
            Basically the system identifies user command by voltage divider
              implemented at the analog port of the Arduino.
    
  Ref:
    Source of inspiration: KIBO
*****************************************************************/ 

#include <EEPROM.h>

#define inputPin   A5
#define CMDswitch  10
#define BEGINpin  11
#define ENDpin  12
#define CMDpin  13
#define programmingPin 4

#define EEPROM_UsrCmd_BeginBlock  0
#define EEPROM_UsrCmd_BeginAdrs   1
#define EEPROM_UsrCmd_EndBlock    2  
#define EEPROM_UsrCmd_EndAdrs     3  
#define EEPROM_UsrCmd_NumAdrs     4  
#define EEPROM_UsrCmd_1stAdrs     5  
#define EEPROMmaxAdrs             1023

#define marginADC   20
#define maxCmdIdx  10
#define maxUsrCmd  30

int ADCArray[maxCmdIdx+1] =
      { 10, 50, 110, 171,
        236,287,346,392,
        438,512,974 };
enum CMD {CMD_BEGIN, CMD_FWD, CMD_LEFT, CMD_RIGHT, 
          CMD_TNCLK, CMD_TNANT, CMD_STOP, CMD_REV, 
          CMD_LOOPBEGIN, CMD_LOOPEND, CMD_END};
char strCMD[maxCmdIdx+1][20] = 
        {"CMD_BEGIN", "CMD_FWD", "CMD_LEFT", "CMD_RIGHT", 
         "CMD_TNCLK", "CMD_TNANT", "CMD_STOP","CMD_REV", 
         "CMD_LOOPBEGIN", "CMD_LOOPEND", "CMD_END"
        };         
int usrCmdArray[maxUsrCmd], usrCmdIdx=0;          

boolean foundCMD_BEGIN=false, foundCMD_END=false;
boolean DONE_writeCmd2EEPROM=false;
boolean DONE_readCMD=false;
//---------------------------------------------        
void setupCBT()
{
  Serial.begin(9600);
  pinMode(BEGINpin, OUTPUT);
  pinMode(ENDpin, OUTPUT);
  pinMode(CMDpin, OUTPUT);
  pinMode(CMDswitch, INPUT);
  pinMode(programmingPin, INPUT);
}//setupCBT

//---------------------------------------------        
void loopCBT()
{
  boolean testMode=false; //false true
  int operationMode;

  if (testMode)
    testMaster();
  else
  {
    operationMode = digitalRead(programmingPin);  
    //programming & write to EEPROM mode
    //the programmingPin swith ON and not yet write program to EEPROM
    if (operationMode == HIGH) 
    {  
      if (!foundCMD_BEGIN)
      {
        foundCMD_BEGIN = chkCMD_BEGIN();
        delay(500);
      }//if (!foundCMD_BEGIN)
      else
      {
        if (!foundCMD_END)
          foundCMD_END = chkCMD_END();
          
        //if ((!foundCMD_END) && (digitalRead(CMDswitch) == LOW))
        if (!foundCMD_END)
        {
          chkCMD();
        }//if (!foundCMD_END)
        
        if (foundCMD_END)
        {
          doUsrLoop();
        }//if (foundCMD_END == true)
      }//else (foundCMD_BEGIN == true)
    }//if (operationMode == HIGH) 
    else
      readEEPROMnPlay();
  }//else (testMODE == false)
  
  //Serial.println(cmdADC);
  //delay(200);
}//loopCBT

//---------------------------------------------        
void testMaster()
{
  int cmdADC;
  
  cmdADC = analogRead(inputPin);
  Serial.print("ADC");
  Serial.println(cmdADC);
  delay(1000);
  
  /*
  usrCmdIdx = 6;
  usrCmdArray[0] = CMD_FWD;
  usrCmdArray[1] = CMD_LEFT;
  usrCmdArray[2] = CMD_FWD;
  usrCmdArray[3] = CMD_LEFT;
  usrCmdArray[4] = CMD_FWD;
  usrCmdArray[5] = CMD_LEFT;
  doUsrLoop();
  */
  /*
  if (digitalRead(CMDswitch) == LOW)
  {
    chkCMD();
  }
  */
  /*
  Serial.print("CMD_BEGIN: ");
  Serial.println(CMD_BEGIN);
  Serial.print("CMD_END: ");
  Serial.println(CMD_END);
  */
}//testMaster

//---------------------------------------------        
boolean chkCMD_BEGIN()
{
  int cmdADC;
  
  cmdADC = analogRead(inputPin);
  if (cmdADC < (ADCArray[CMD_BEGIN] + marginADC))
  {
    Serial.println(cmdADC);
    Serial.println("Found CMD_BEGIN");
    digitalWrite(BEGINpin, HIGH);
    return true;
  }
  else
  {
    /*
    Serial.println(cmdADC);
    Serial.println("NOT yet found CMD_BEGIN");
    */
    digitalWrite(BEGINpin, LOW);
    return false;   
  }
}//chkCMD_BEGIN()

//---------------------------------------------        
boolean chkCMD_END()
{
  int cmdADC;
  
  delay(100);
  cmdADC = analogRead(inputPin);
  if ( (cmdADC < (ADCArray[CMD_END] + marginADC)) &&
       (cmdADC > (ADCArray[CMD_END] - marginADC)) )
  {
    Serial.println(cmdADC);
    Serial.println(ADCArray[CMD_END] + marginADC);
    Serial.println("Found CMD_END");
    digitalWrite(ENDpin, HIGH);
    return true;
  }
  else
  {
    /*
    Serial.println(cmdADC);
    Serial.println(ADCArray[CMD_END] + marginADC);
    Serial.println("NOT yet found CMD_END");
    */
    digitalWrite(ENDpin, LOW);
    return false;   
  }
  //delay(500);
}//chkCMD_END()

//---------------------------------------------        
void chkCMD()
{
  int cmdADC, cmdIdx;
  
  //to diminish noise influence
  delay(100);
  cmdADC = analogRead(inputPin);
  if (cmdADC < 1000)
  {
    Serial.print("cmdADC");
    Serial.println(cmdADC);
    delay(100);
    cmdIdx = getCmdIdx(cmdADC);
    if (usrCmdIdx < maxUsrCmd)
    {
      digitalWrite(CMDpin, HIGH);
      usrCmdArray[usrCmdIdx] = cmdIdx;
      Serial.println(cmdIdx);
      Serial.println("Found CMD");
      Serial.print("strCMD");
      Serial.println(strCMD[cmdIdx]);
      usrCmdIdx++;
      delay(1000);
      digitalWrite(CMDpin, LOW);
      delay(500);
    }

    if (cmdIdx == CMD_END)
    {
      digitalWrite(ENDpin, HIGH);
      foundCMD_END = true;
    }
    else
    {
      digitalWrite(ENDpin, LOW);
      foundCMD_END = false;
    }
  }//if (cmdADC < 1000)
  
}//chkCMD()

//--------------------------------------------- 
int getCmdIdx(int valADC)
{
  boolean foundCMD=false;
  int i=0;
  
  while ((!foundCMD) && (i < maxCmdIdx))
  {
    if (valADC < (ADCArray[i] + marginADC))
    {
      foundCMD = true;
      return i;
      //break;
    }
    else
      i++;
  }//while ((!foundCMD) && (i < maxCmdIdx))
  
  return -1;
}//getCmdIdx

//---------------------------------------------        
void doUsrLoop()
{
  int operationMode;
  
  operationMode = digitalRead(programmingPin);  
  //programming & write to EEPROM mode
  //the programmingPin swith ON and not yet write program to EEPROM
  if (operationMode == HIGH) 
  {
    if (!DONE_writeCmd2EEPROM)
      writeCmd2EEPROM();
    else
      readEEPROMnPrint();
    delay(5000);
  }
    
  //read from EEPROM & play mode
  if (operationMode == LOW)
    readEEPROMnPlay();
    
  /*
  int k;

  Serial.print("Num of usr cmd: ");
  Serial.println(usrCmdIdx);
  for(k=0; k<usrCmdIdx; k++)
  {
    Serial.print("Usr cmd[");
    Serial.print(k);
    Serial.print("]: ");
    Serial.println(usrCmdArray[k]);
    Serial.println(strCMD[usrCmdArray[k]]);
    switch (usrCmdArray[k])
    {
      case CMD_FWD:
        goForward();
        break;
      case CMD_LEFT:
        turnLeft();
        break;
      case CMD_RIGHT:
        turnRight();
        break;
      case CMD_TNCLK:
        turnClk();
        break;
      case CMD_TNANT:
        turnAntClk();
        break;
      default:
      ;
        //goForward();
    }//switch
    delay(1000);
      
  }
  //delay(2000);
  */  
}//doUsrLoop()

//---------------------------------------------        
void readEEPROMnPrint()
{
  int k;
  int storedCmdBeginBlock, storedCmdBeginAdrs; 
  int curCmdBeginAdrs;
  int prevCmd, curCmdAdrs;
  int offsetK=0;

  if (!DONE_readCMD)
  {
    Serial.println("Reading usr cmds from EEPROM...");  
  
    //Get the NUM of CMD 
    usrCmdIdx = EEPROM.read(EEPROM_UsrCmd_NumAdrs);
    delay(20);
  
    //storedCmdBeginBlock: 0 ~ 3
    storedCmdBeginBlock = EEPROM.read(EEPROM_UsrCmd_BeginBlock);
    delay(20);
    //storedCmdBeginAdrs: 0 ~ 255
    storedCmdBeginAdrs = EEPROM.read(EEPROM_UsrCmd_BeginAdrs);
    delay(20);
    
    curCmdBeginAdrs = (storedCmdBeginBlock * 256) + storedCmdBeginAdrs;
    
    for(k=0; k<usrCmdIdx; k++)
    {
      curCmdAdrs = curCmdBeginAdrs+k-offsetK;
  
      //if the curCmdAdrs is over EEPROMmaxAdrs
      if (curCmdAdrs > EEPROMmaxAdrs)
      {
        //set the curCmdAdrs to the very beginning address of cmd
        curCmdAdrs = EEPROM_UsrCmd_1stAdrs;
        //and reset the curCmdBeginAdrs as of EEPROM_UsrCmd_1stAdrs
        curCmdBeginAdrs = EEPROM_UsrCmd_1stAdrs;
        //the following iteration variable "k" 
        //should be offseted by "offsetK"
        offsetK = k;
      }//if (curCmdAdrs > EEPROMmaxAdrs)
      usrCmdArray[k] = EEPROM.read(curCmdAdrs);
      delay(20);
      if (k == (usrCmdIdx-1))
        DONE_readCMD = true;
    }//for(k=0; k<usrCmdIdx; k++)
  }//if (!DONE_readCMD)
  
  
  Serial.println("Stored usr cmds are... ");
  Serial.print("Num of usr cmd: ");
  Serial.println(usrCmdIdx);

  for(k=0; k<usrCmdIdx; k++)
  {
    Serial.print("Usr cmd[");
    Serial.print(k);
    Serial.print("]: ");
    Serial.print(usrCmdArray[k]);
    Serial.print(", ");
    Serial.println(strCMD[usrCmdArray[k]]);
  }//for(k=0; k<usrCmdIdx; k++)
  
}//readEEPROMnPrint

//---------------------------------------------        
void writeCmd2EEPROM()
{
  int k;
  int prevCmdEndBlock, prevCmdEndAdrs; 
  int curCmdBeginBlock, curCmdBeginAdrs; 
  int curCmdEndBlock, curCmdEndAdrs;
  int prevCmd, curCmdAdrs;
  int offsetK=0;

  Serial.print("Writing usr cmds to EEPROM...");  
  Serial.print("Num of usr cmd: ");
  Serial.println(usrCmdIdx);

  //******(1) Find the BEGIN address for the current set of user commands,
  //******    using the end address of previous commands set
  
  //Get the end address of previously stored set of user commands
  //prevCmdEndBlock: 0 ~ 3
  prevCmdEndBlock=EEPROM.read(EEPROM_UsrCmd_EndBlock);
  //prevCmdEndAdrs: EEPROM_UsrCmd_1stAdrs ~ EEPROMmaxAdrs
  prevCmdEndAdrs= (prevCmdEndBlock*256) + EEPROM.read(EEPROM_UsrCmd_EndAdrs);
  
  //Check the validity of address.
  //user command address should be equal or greather than EEPROM_UsrCmd_1stAdrs
  //curCmdBeginAdrs: EEPROM_UsrCmd_1stAdrs ~ EEPROMmaxAdrs
  if (prevCmdEndAdrs < EEPROM_UsrCmd_1stAdrs)
  {
    Serial.println("Wrong address of 'prevCmdEndAdrs'! and auto-modified it.");
    curCmdBeginAdrs = EEPROM_UsrCmd_1stAdrs;
  }
  else
    curCmdBeginAdrs = prevCmdEndAdrs+1;
  
  //Check the upper limit of EEPROM address.
  //when prevCmdEndAdrs is EEPROMmaxAdrs, 
  //  then curCmdBeginAdrs is over the upper limit of EEPROM address,
  //  so that we need to reset the curCmdBeginAdrs as of the top address of usr cmd
  if (curCmdBeginAdrs > EEPROMmaxAdrs)
    curCmdBeginAdrs = EEPROM_UsrCmd_1stAdrs;
    
  //******(2) Record the BEGIN address to EEPROM for the current set of user commands
  //****** 
  //the value of EEPROM stored in EPROM_UsrCmd_BeginBlock address: 0 ~ 3
  EEPROM.write(EEPROM_UsrCmd_BeginBlock, curCmdBeginAdrs / 256);
  //the value of EEPROM stored in EEPROM_UsrCmd_BeginAdrs address: 0 ~ 255
  EEPROM.write(EEPROM_UsrCmd_BeginAdrs, curCmdBeginAdrs % 256);
  
  //******(3) Record the current set of user CMD to EEPROM
  //****** 
  //now that we got the begin address for current set of commands
  //write the commands into EEPROM
  for(k=0; k<usrCmdIdx; k++)
  {
    Serial.print("Usr cmd[");
    Serial.print(k);
    Serial.print("]: ");
    Serial.print(usrCmdArray[k]);
    Serial.print(", ");
    Serial.println(strCMD[usrCmdArray[k]]);
    
    //"offsetK" is conceived 
    //when it wraps around the upper limit of EEPROM address
    curCmdAdrs = curCmdBeginAdrs+k-offsetK;
    
    //if the curCmdAdrs is over EEPROMmaxAdrs
    if (curCmdAdrs > EEPROMmaxAdrs)
    {
      //set the curCmdAdrs to the very beginning address of cmd
      curCmdAdrs = EEPROM_UsrCmd_1stAdrs;
      //and reset the curCmdBeginAdrs as of EEPROM_UsrCmd_1stAdrs
      curCmdBeginAdrs = EEPROM_UsrCmd_1stAdrs;
      //the following iteration variable "k" 
      //should be offseted by "offsetK"
      offsetK = k;
    }//if (curCmdAdrs > EEPROMmaxAdrs)
    
    //to enlengthen the EEPROM write-cycle life, 
    //minimize the writing frequency to the EEPROM cell
    prevCmd = EEPROM.read(curCmdAdrs);
    if (prevCmd != usrCmdArray[k])
      EEPROM.write(curCmdAdrs, usrCmdArray[k]);
    //writing cycle requires 3.3ms for ATmega328P chip
    //and use some safe margin for each writing cycle
    delay(20);
  }//for(k=0; k<usrCmdIdx; k++)
  
  //******(4) Record the END address to EEPROM for the current set of user commands
  //****** 
  //the value of EEPROM stored in EPROM_UsrCmd_BeginBlock address: 0 ~ 3
  //the result of dividing by 256
  curCmdEndBlock = curCmdEndAdrs / 256;
  EEPROM.write(EEPROM_UsrCmd_EndBlock, curCmdEndBlock);
  //the value of EEPROM stored in EEPROM_UsrCmd_BeginAdrs address: 0 ~ 255
  //the residual of dividing by 256, that is modular by 256
  curCmdEndAdrs = curCmdAdrs % 256;
  EEPROM.write(EEPROM_UsrCmd_EndAdrs, curCmdEndAdrs);
  
  //******(5) Record the NUM of CMD to EEPROM for the current set of user commands
  //****** 
  EEPROM.write(EEPROM_UsrCmd_NumAdrs, usrCmdIdx);
  
  DONE_writeCmd2EEPROM = true;
  
}//writeCmd2EEPROM()

//---------------------------------------------        
void readEEPROMnPlay()
{
  int k;
  int EEPROMval;
  
  readEEPROMnPrint();
  
  for(k=0; k<usrCmdIdx; k++)
  {
    Serial.print("Usr cmd[");
    Serial.print(k);
    Serial.print("]: ");
    Serial.println(usrCmdArray[k]);
    Serial.print(", ");
    Serial.println(strCMD[usrCmdArray[k]]);
    switch (usrCmdArray[k])
    {
      case CMD_FWD:
        goForward();
        break;
      case CMD_LEFT:
        turnLeft();
        break;
      case CMD_RIGHT:
        turnRight();
        break;
      case CMD_TNCLK:
        turnClk();
        break;
      case CMD_TNANT:
        turnAntClk();
        break;
      case CMD_LOOPBEGIN:
        goForward();
        break;
      case CMD_LOOPEND:
        goBackward();
        break;
      default:
      ;
        //goForward();
    }//switch
    delay(1000);
      
  }//for(k=0; k<usrCmdIdx; k++)
  
}//readEEPROMnPlay()

//---------------------------------------------        
int a = 0, value;
void EEPROMsimpleRead()
{
  value = EEPROM.read(a);
  Serial.print(a);
  Serial.print("\t");
  Serial.println(value);

  a = a + 1;

  if (a == EEPROMmaxAdrs)
    a = 0;

  delay(500);
  
}
//---------------------------------------------        
//---------------------------------------------        
//---------------------------------------------        

