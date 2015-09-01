/**************************************************************
  Target MCU: Arduino Pro Mini (ATmega328P)
  Clock type: External as in Arduino Pro Mini
  Clock speed: 16Mhz as of Arduino Pro Mini 16Mhz 5V version
  Name    : WalnutPgmCar.ino
  Author  : Insoo Kim (insoo@hotmail.com)
  Date    : Thu Aug 06, 2015
  Update  : Sun Aug 09, 2015
  Objective: Program self-driving car by Walnut or any thing. Code by Thing!
  Desc    : There are two modes of operation: Coding & Playing
    ** Coding mode ** 
    0) Set some navigation target and route from your starting point of the car.
        and draw the sequence of your path-finding route 
        using the predefined walnut commands.
    
    1) Set PGM_Mode switch On, to code the car using walnuts.
       
    2) Put the Start walnut on the WPP(Walnut Programming Pad), 
        successful contacts with your Start walnut terminal and the WPP 
        will turn on Green LED.
         
    3) Put each of command walnut on the WPP by the sequence of your navigation path.
        successful contacts with your each command walnut terminal and the WPP 
        will turn on Yellow LED.
    
    4) Put the End walnut on the WPP, 
        successful contacts with your End walnut terminal and the WPP 
        will turn on Red LED.

    5) Now that the coding is done, you can run or play the car according to your code.

    ** Playing mode ** 
    1) Set PGM_Mode switch Off.
    
    2) The car will automatically run, so that put the car gently on the starting point,
        then press the reset button of the Arduino UNO.
        
    3) It will follow the path you have coded to get to the target of your intention.
    
  Ref:
  How to separate Arduino sketch files
  http://allgood38.io/understand-arduino-development.html
*****************************************************************/ 

//------pin assignments
#define MR_ONOFF 9
#define ML_ONOFF 8

//if both relays are off the car moves forward 
//if both relays are on the car moves backward
#define MR_FWDREV 11
#define ML_FWDREV 10

#define MR_SPEED 13

//1000 ms is for nearly 180 degree at 4V battery
#define TurnLeftRightDuration 500 

//prevent coninuous reading of IR sensor 
#define afterTurningDelay 500 //ms

//test period for debugging purpose
#define testPeriod 5000 //ms

#define speedCheckInterval 2000 //ms

unsigned long cntOpenSlot=0, cntTirePerimeter=0;
int prevState=0, curState=0;
float tirePerimeter, travelDist, travelSpeed, prevSpeed;
unsigned long lapse, prevTime, curTime; 
unsigned long startTime,loopLapse, loopPrevTime=0;

const float tireD=6.60;
const float  pi=3.14;
#define NUMSLOTS 20

//--------------------------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(MR_ONOFF, OUTPUT);
  pinMode(ML_ONOFF, OUTPUT);
  
  digitalWrite(MR_ONOFF, LOW);
  digitalWrite(ML_ONOFF, LOW);
  //digitalWrite(MR_ONOFF, HIGH);
  //digitalWrite(ML_ONOFF, HIGH);
  
  curTime = millis();
  prevTime = curTime;

  //forward & backward control to motor  
  pinMode(MR_FWDREV, OUTPUT);
  pinMode(ML_FWDREV, OUTPUT);

  pinMode(MR_SPEED, INPUT);
  
  startTime = millis();
  loopPrevTime = startTime;
  tirePerimeter = tireD * pi; 
  setupCBT();

  //testGoForward();

}//setup

//--------------------------------------------------
void loop()
{
  masterTest();
  
  //loopCBT();
  //EEPROMsimpleRead();
  //readEEPROMnPrint();
  //delay(5000);
}//loop

//------------------------------------------------
void masterTest()
{
  goForward();
  checkSpeed();
  //testGoForward();
  /*
  testGoForward();
  testGoBackward();
  testTurnLeft();
  testTurnRight();  
  testTurnClk();
  testTurnAntClk();
  */
}//masterTest

//------------------------------------------------
void noBlockDelay(unsigned long delayDuration)
{
  unsigned long noBlockCurTime, noBlockPrevTime, noBlockLapse;
  
  noBlockCurTime = millis();
  noBlockPrevTime = noBlockCurTime;
  noBlockLapse = noBlockCurTime - noBlockPrevTime;
  while (noBlockLapse < delayDuration)
  {
    noBlockCurTime = millis();
    noBlockLapse = noBlockCurTime - noBlockPrevTime; 
  }
}//noBlockageDelay

//------------------------------------------------
void goBackward()
{
  //reverse polarize right & left motors
  digitalWrite(MR_FWDREV, HIGH);
  digitalWrite(ML_FWDREV, HIGH);
  delay(50);
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
}//goBackward

//------------------------------------------------
void goForward()
{
  //normal polarize right & left motors
  digitalWrite(MR_FWDREV, LOW);
  digitalWrite(ML_FWDREV, LOW);
  //delay(50);
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
}//goForward

//------------------------------------------------
void stopAll()
{
  digitalWrite(ML_ONOFF, LOW);
  digitalWrite(MR_ONOFF, LOW);
  delay(50);
}//stopAll

//------------------------------------------------
void turnClk()
{
  //reverse polarize right & normal left motors
  digitalWrite(MR_FWDREV, HIGH);
  digitalWrite(ML_FWDREV, LOW);
  delay(50);
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
  delay(TurnLeftRightDuration);
  //goForward();
}//turnClk()

//------------------------------------------------
void turnAntClk()
{
  //normal polarize right & reverse left motors
  digitalWrite(MR_FWDREV, LOW);
  digitalWrite(ML_FWDREV, HIGH);
  delay(50);
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
  delay(TurnLeftRightDuration);
  //goForward();
}//turnAntClk()

//------------------------------------------------
void turnLeft()
{
  digitalWrite(ML_ONOFF, LOW);
  digitalWrite(MR_ONOFF, HIGH);
  delay(TurnLeftRightDuration);
  goForward();
}//turnLeft()

//------------------------------------------------
void turnRight()
{
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, LOW);
  delay(TurnLeftRightDuration);
  goForward();
}//turnRight()

//------------------------------------------------
void checkSpeed()
{
  //proceed time tick
  curTime = millis();

  //when photoTRrightansistor, that is MR_SPEED, is blocked
  if ( digitalRead(MR_SPEED) == HIGH)
  {
    //digitalWrite(sigLED, HIGH);
    //Serial.println("Blocked");
      curState = 1;
    if (prevState != curState)
    {
      prevState = 1;
    }
  }//if ( digitalRead(photoTRright) == HIGH)
  else
  {
    //digitalWrite(sigLED,  LOW);
    //Serial.println("Open");
    curState = 0;
    if (prevState != curState)
    {
      cntOpenSlot++;
      prevState = 0;
      
      //Serial.print("Passing slots: ");
      //Serial.println(cntOpenSlot);
    }//if (prevState != curState)
  }//else, that is ( digitalRead(photoTRright) == LOW)

  lapse = curTime - prevTime;
  if (lapse > 2000)
  {
    digitalWrite(ML_ONOFF, HIGH);
    digitalWrite(MR_ONOFF, HIGH);
  }
      
  if (lapse > speedCheckInterval)
  {
    travelDist = (float)tirePerimeter * (float)cntOpenSlot / (float)NUMSLOTS;
    Serial.print("cntOpenSlot: ");
    Serial.println(cntOpenSlot);
    Serial.print("travelDist: ");
    Serial.println(round(travelDist));
    travelSpeed = travelDist/(speedCheckInterval/1000);
    //Display speed
    //disp7seg2digits(travelSpeed, false, false);
    Serial.print("travelSpeed [cm/s]: ");
    Serial.println(round(travelSpeed));
    prevTime = curTime;
    cntOpenSlot=0;
    if ((prevSpeed == 0) && (travelSpeed == 0))
      //dispBlank();
    prevSpeed = travelSpeed;
  }//if (lapse > speedCheckInterval)
}//checkSpeed()  

//------------------------------------------------
//------------------------------------------------
//------------------------------------------------


