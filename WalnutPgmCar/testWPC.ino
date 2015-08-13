/**************************************************************
  Target MCU: Arduino Pro Mini (ATmega328P)
  Clock type: External as in Arduino Pro Mini
  Clock speed: 16Mhz as of Arduino Pro Mini 16Mhz 5V version
  Name    : testWPC.ino
  Author  : Insoo Kim (insoo@hotmail.com)
  Date    : Thu Aug 06, 2015
  Update  : Sun Aug 09, 2015
  Objective: Program self-driving car by Walnut or any thing. Code by Thing!
  Desc    : This is an accompanying code module for WalnutPgmCar.ino
            Code snippets for test purpose
    
  Ref:
*****************************************************************/ 

//------------------------------------------------
void testGoForward()
{
  goForward();
  delay(1000);
  stopAll();
  delay(1000);
}//testGoBackward

//------------------------------------------------
void testGoBackward()
{
  goBackward();
  delay(1000);
  stopAll();
  delay(1000);
}//testGoBackward

//------------------------------------------------
void testTurnLeft()
{
  turnLeft();
  delay(1000);
  stopAll();
  delay(1000);
}//testTurnLeft

//------------------------------------------------
void testTurnRight()
{
  turnRight();
  delay(1000);
  stopAll();
  delay(1000);
}//testTurnRight

//------------------------------------------------
void testTurnClk()
{
  turnClk();
  delay(1000);
  stopAll();
  delay(1000);
}//testTurnClk

//------------------------------------------------
void testTurnAntClk()
{
  turnAntClk();
  delay(1000);
  stopAll();
  delay(1000);
}//testTurnAntClk

//------------------------------------------------
void testRightMotorBackward()
{
  digitalWrite(MR_ONOFF, HIGH);

  digitalWrite(MR_FWDREV, LOW);
  delay(1000);
  digitalWrite(MR_FWDREV, HIGH);
  delay(1000);
  digitalWrite(MR_FWDREV, LOW);

  digitalWrite(MR_ONOFF, LOW);
  delay(2000);
}//testRightMotorBackward

//------------------------------------------------
void testLeftMotorBackward()
{
  digitalWrite(ML_ONOFF, HIGH);

  digitalWrite(ML_FWDREV, LOW);
  delay(1000);
  digitalWrite(ML_FWDREV, HIGH);
  delay(1000);
  digitalWrite(ML_FWDREV, LOW);

  digitalWrite(ML_ONOFF, LOW);
  delay(2000);
}//testLeftMotorBackward


