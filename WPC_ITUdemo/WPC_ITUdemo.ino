#include <EEPROM.h>

#define inputPin          A5
#define PGM_START_LEDpin  8
#define PGM_CMD_LEDpin    9
#define PGM_END_LEDpin    10
#define modeOPswitch      7

#define EEPROM_UsrCmd_BeginBlock  0
#define EEPROM_UsrCmd_BeginAdrs   1
#define EEPROM_UsrCmd_EndBlock    2  
#define EEPROM_UsrCmd_EndAdrs     3  
#define EEPROM_UsrCmd_NumAdrs     4  
#define EEPROM_UsrCmd_1stAdrs     5  
#define EEPROMmaxAdrs             1023

#define marginADC   20
#define maxCmdIdx  30
#define maxUsrCmd  30

#define DEBUG_CMD false // true 

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

//--------------------------------------------------
void setup()
{
  setupRelayCar();

  Serial.begin(9600);

  pinMode(modeOPswitch, INPUT);
  
  pinMode(PGM_START_LEDpin, OUTPUT);
  pinMode(PGM_CMD_LEDpin, OUTPUT);
  digitalWrite(PGM_START_LEDpin, HIGH); //HIGH to turn off LED
  turnOFF_CMD_LED();
  /*
  goForward();
  goForward();
  stopAll();
  */
}//setup

//--------------------------------------------------
void loop()
{
  
  loopCBT();
 //goForward();
 //chkSpeed();
 
  //test_goFwd();
  
  //test_ReadPrgCmd();
  //test_chkCMD();
  //test_turnDeg();
  
  //test_modeOPswitch();
  //ON_testWalnulProgrammingIndicatorLED();
  //testWalnulProgrammingIndicatorLED();
  //testRelay_Right();
  //testRelay_Left();
  //testMove();
  //DoneTestMove();
}//loop

//---------------------------------------------        
void test_goFwd()
{
  delay(3000);
  goForward();
  stopAll();
 
}//test_goFwd

//---------------------------------------------        
void test_turnDeg()
{
  delay(3000);
  turnLeft();
  stopAll();
  delay(3000);
  turnRight();
  stopAll();
  //turnRight();
  //stopAll();
}//test_turnDeg()

//---------------------------------------------        
void test_chkCMD()
{
  chkCMD();
  delay(500);
}//test_chkCMD

//---------------------------------------------        
void test_chkCMD_END()
{
  foundCMD_END = chkCMD_END();
  delay(500);
}//test_chkCMD_END

//---------------------------------------------        
void test_chkCMD_BEGIN()
{
  foundCMD_BEGIN = chkCMD_BEGIN();
  delay(500);
}//test_chkCMD_BEGIN

//---------------------------------------------        
void loopCBT()
{
  boolean testMode=false; //false true
  int operationMode;

  if (testMode)
    testMaster();
  else
  {
    operationMode = digitalRead(modeOPswitch);  
    //Serial.println(operationMode);
    //programming & write to EEPROM mode
    //the modeOPswitch swith ON and not yet write program to EEPROM
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
          
        //if ((!foundCMD_END) && (digitalRead(modeOPswitch) == LOW))
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
    {
      delay(3000);
      readEEPROMnPlay();
    }
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
  if (digitalRead(modeOPswitch) == LOW)
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
    turnON_START_LED();
    return true;
  }
  else
  {
    if (DEBUG_CMD)
    {
      Serial.println(cmdADC);
      Serial.println("NOT yet found CMD_BEGIN");
    }
    turnOFF_START_LED();
    return false;   
  }
}//chkCMD_BEGIN()

//---------------------------------------------        
void turnON_END_LED()
{
  byte i;
  for (i=0; i<3; i++)
  {
    turnON_START_LED();
    delay(200);
    turnOFF_END_LED();
    delay(200);
  }
}//turnON_START_LED

//---------------------------------------------        
void turnOFF_END_LED()
{
  digitalWrite(PGM_START_LEDpin, HIGH);
}//turnOFF_START_LED

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
    //digitalWrite(PGM_END_LEDpin, HIGH);
    turnON_END_LED();
    
    return true;
  }
  else
  {
    if (DEBUG_CMD)
    {
      Serial.println(cmdADC);
      Serial.println(ADCArray[CMD_END] + marginADC);
      Serial.println("NOT yet found CMD_END");
    }
    //digitalWrite(PGM_END_LEDpin, LOW);
     turnOFF_END_LED();
    return false;   
  }
  //delay(500);
}//chkCMD_END()

//---------------------------------------------        
void turnOFF_START_LED()
{
  digitalWrite(PGM_START_LEDpin, HIGH);
}//turnOFF_START_LED

//---------------------------------------------        
void turnON_START_LED()
{
  digitalWrite(PGM_START_LEDpin, LOW);
}//turnON_START_LED

//---------------------------------------------        
void chkCMD()
{
  int cmdADC, cmdIdx;
  
  //to diminish noise influence
  delay(100);
  cmdADC = analogRead(inputPin);
  if (DEBUG_CMD)
  {
    Serial.print("Testing chkCMD... ");
    Serial.println(cmdADC);
  }
  if (cmdADC < 1000)
  {
    Serial.print("cmdADC");
    Serial.println(cmdADC);
    delay(100);
    cmdIdx = getCmdIdx(cmdADC);
    if (usrCmdIdx < maxUsrCmd)
    {
      turnON_CMD_LED();
      usrCmdArray[usrCmdIdx] = cmdIdx;
      Serial.println(cmdIdx);
      Serial.println("Found CMD");
      Serial.print("strCMD");
      Serial.println(strCMD[cmdIdx]);
      usrCmdIdx++;
      delay(1000);
      turnOFF_CMD_LED();
      delay(500);
    }//if (usrCmdIdx < maxUsrCmd)

    if (cmdIdx == CMD_END)
    {
      turnON_END_LED();
      foundCMD_END = true;
    }//if (cmdIdx == CMD_END)
    else
    {
      turnOFF_END_LED();
      foundCMD_END = false;
    }//else if (cmdIdx == CMD_END)
  }//if (cmdADC < 1000)
  else
  {
    //Serial.print("NO valid cmdADC: ");
    //Serial.println(cmdADC);
  }
  
}//chkCMD()

//---------------------------------------------        
void turnOFF_CMD_LED()
{
   digitalWrite(PGM_CMD_LEDpin, HIGH);
}//turnOFF_CMD_LED

//---------------------------------------------        
void turnON_CMD_LED()
{
   digitalWrite(PGM_CMD_LEDpin, LOW);
}//turnON_CMD_LED

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
  
  operationMode = digitalRead(modeOPswitch);  
  //programming & write to EEPROM mode
  //the modeOPswitch swith ON and not yet write program to EEPROM
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
    stopAll();
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
//***********************************************************************
//***********************************************************************
//***********************************************************************
