/**************************************************************
  Target MCU: Arduino UNO (ATmega328P)Í
  Clock type: External as in Arduino UNO
  Clock speed: 16Mhz as of Arduino UNO 16Mhz 5V version
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
void testGoBackward()
{
  goBackward();
  delay(1000);
  stopAll();
  delay(1000);
}//testGoBackward

//------------------------------------------------
void testRightMotorBackward()
{
  digitalWrite(rightTR, HIGH);

  digitalWrite(rightRelay, LOW);
  delay(1000);
  digitalWrite(rightRelay, HIGH);
  delay(1000);
  digitalWrite(rightRelay, LOW);

  digitalWrite(rightTR, LOW);
  delay(2000);
}//testRightMotorBackward

//------------------------------------------------
void testLeftMotorBackward()
{
  digitalWrite(leftTR, HIGH);

  digitalWrite(leftRelay, LOW);
  delay(1000);
  digitalWrite(leftRelay, HIGH);
  delay(1000);
  digitalWrite(leftRelay, LOW);

  digitalWrite(leftTR, LOW);
  delay(2000);
}//testLeftMotorBackward


