//------pin assignments
#define SPEED_INTR 2 //Yellow

#define MR_ONOFF 5 //Yellow
#define ML_ONOFF 3 //White

//if both relays are off the car moves forward 
//if both relays are on the car moves backward
#define MR_FWDREV 4 //Blue
#define ML_FWDREV 6 //Green

//100 ms is for nearly 180 degree at 4V battery
//#define TurnLeftRightDuration 100 

//for nearly 90 degree at 6V battery
#define TurnLeftDuration 250 
#define TurnRightDuration 250 

#define GoFwdDuration 500 
#define GoRevDuration 500 

//prevent coninuous reading of IR sensor 
#define afterTurningDelay 100 //ms

//test period for debugging purpose
#define testPeriod 1000 //ms
#define testPeriod_Relay 3000 //ms
#define testPeriod_LED 500 //ms

//For Speed meter
//Connect the signal to digital pin 2
int U_Pin = 2;   
float Val = 0;     
float time;  
float Speed; 

//--------------------------------------------------
void setupRelayCar()
{
  attachInterrupt(0,count,CHANGE);
  
  pinMode(modeOPswitch, INPUT);
  
  pinMode(MR_ONOFF, OUTPUT);
  pinMode(ML_ONOFF, OUTPUT);
  digitalWrite(MR_ONOFF, LOW);
  digitalWrite(ML_ONOFF, LOW);

  //forward & backward control to motor  
  pinMode(MR_FWDREV, OUTPUT);
  pinMode(ML_FWDREV, OUTPUT);
  digitalWrite(MR_FWDREV, LOW);
  digitalWrite(ML_FWDREV, LOW);
}//setupRelayCar

//------------------------------------------------
float valS, timeLapse;
void chkSpeed()
{
  time = millis();
  valS = Val;
  timeLapse = millis();
  while (timeLapse - time < 1000)
    timeLapse = millis();
  //Speed =  (Val/40)/(time/60000) ;
  //Serial.println(Speed*2);
  Serial.println((Val-valS)/2);
}//chkSpeed

//------------------------------------------------
void count()
{
  Val += 1;
}//count
//------------------------------------------------
void test_ReadPrgCmd()
{
  Serial.println(analogRead(inputPin));
  delay(testPeriod_LED);
}//test_ReadPrgCmd

//------------------------------------------------
void test_modeOPswitch()
{
  byte inputStatus = digitalRead(modeOPswitch);
  Serial.println(inputStatus);
  if (inputStatus == HIGH)
    digitalWrite(PGM_START_LEDpin, LOW); // LOW to turn on LED
  else
    digitalWrite(PGM_START_LEDpin, HIGH); // LOW to turn on LED
  delay(testPeriod_Relay);
}//test_modeOPswitch

//------------------------------------------------
void ON_testWalnulProgrammingIndicatorLED()
{
  digitalWrite(PGM_START_LEDpin, LOW);
  delay(testPeriod_LED);
  digitalWrite(PGM_START_LEDpin, HIGH);
  delay(testPeriod_LED);

   turnOFF_CMD_LED();
  delay(testPeriod_LED);
   turnOFF_CMD_LED();
  delay(testPeriod_LED);

}//testWalnulProgrammingIndicatorLED

//------------------------------------------------
void testWalnulProgrammingIndicatorLED()
{
  digitalWrite(PGM_START_LEDpin, HIGH);
   turnOFF_CMD_LED();
  digitalWrite(PGM_END_LEDpin, HIGH);
  delay(testPeriod_Relay);
  digitalWrite(PGM_START_LEDpin, LOW);
   turnOFF_CMD_LED();
  digitalWrite(PGM_END_LEDpin, LOW);
  delay(testPeriod_Relay);
}//testWalnulProgrammingIndicatorLED

//------------------------------------------------
void testRelay_Left()
{
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(ML_FWDREV, HIGH);
  delay(testPeriod_Relay);
  digitalWrite(ML_FWDREV, LOW);
  delay(testPeriod_Relay);
}//testRelay_Left

//------------------------------------------------
void testRelay_Right()
{
  digitalWrite(MR_ONOFF, HIGH);
  digitalWrite(MR_FWDREV, HIGH);
  delay(testPeriod_Relay);
  digitalWrite(MR_FWDREV, LOW);
  delay(testPeriod_Relay);
}//testRelay_Right

//------------------------------------------------
void testMove()
{
}//testMove

//------------------------------------------------
void testLeft()
{
  turnLeft();
  delay(testPeriod);
  stopAll();
  delay(testPeriod);
}//testLeft

//------------------------------------------------
void DoneTestMove()
{
  goForward();
  delay(testPeriod);
  stopAll();
  delay(testPeriod);
  
  turnLeft();
  delay(testPeriod);
  stopAll();
  delay(testPeriod);
  
  turnRight();
  delay(testPeriod);
  stopAll();
  delay(testPeriod);

  goBackward();
  delay(testPeriod);
  stopAll();
  delay(testPeriod);
}//testMove

//------------------------------------------------
void goForward()
{
  //normal polarize right & left motors
  digitalWrite(MR_FWDREV, LOW);
  digitalWrite(ML_FWDREV, LOW);
  delay(50);
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
  delay(GoFwdDuration);
  /*
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, LOW);
  delay(GoFwdDuration/20);
  */
}//goForward

//------------------------------------------------
void goBackward()
{
  //reverse polarize right & left motors
  digitalWrite(MR_FWDREV, HIGH);
  digitalWrite(ML_FWDREV, HIGH);
  delay(50);
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
  delay(GoRevDuration);
}//goBackward

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
  byte i;
  for (i=0; i<2; i++)
    turnRight();
  //goForward();
}//turnClk()

//------------------------------------------------
void turnAntClk()
{
  byte i;
  for (i=0; i<2; i++)
    turnLeft();
  //goForward();
}//turnAntClk

//------------------------------------------------
void turnLeft()
{
  digitalWrite(ML_FWDREV, HIGH); //rev Left
  digitalWrite(MR_FWDREV, LOW); //fwd Right
 
  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
  delay(TurnLeftDuration);
  //goForward();
}//turnLeft()

//------------------------------------------------
void turnRight()
{
  digitalWrite(ML_FWDREV, LOW); //fwd Left
  digitalWrite(MR_FWDREV, HIGH); //rev Right

  digitalWrite(ML_ONOFF, HIGH);
  digitalWrite(MR_ONOFF, HIGH);
  delay(TurnRightDuration);
  //goForward();
}//turnRight()
