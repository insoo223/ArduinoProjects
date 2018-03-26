void setupL293D()
{
  pinMode(EN1pin, OUTPUT); // PWM
  pinMode(EN2pin, OUTPUT); // PWM
}

//--------------------------------------------
//--- LEFT MOTOR
//--------------------------------------------

void testLeftMotor_293D()
{
  LFwd();
  delay(1000);
  LStop();
  delay(1000);
  LBwd();
  delay(1000);
  LStop();
  delay(1000);
}//testLeftMotor_293D

void LFwd()
{
  digitalWrite(EN1pin, HIGH);
  curBottomX |= _BV(MLPpin); 
  curBottomX &= ~_BV(MLNpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
}

void LBwd()
{
  digitalWrite(EN1pin, HIGH);
  curBottomX |= _BV(MLNpin); 
  curBottomX &= ~_BV(MLPpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
}

void LStop()
{
  digitalWrite(EN1pin, LOW);
}

//--------------------------------------------
//--- RIGHT MOTOR
//--------------------------------------------
void testRightMotor_293D()
{
  RFwd();
  delay(1000);
  RStop();
  delay(1000);
  RBwd();
  delay(1000);
  RStop();
  delay(1000);
}//testLeftMotor_293D

void RFwd()
{
  digitalWrite(EN2pin, HIGH);
  curBottomX |= _BV(MRPpin); 
  curBottomX &= ~_BV(MRNpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
}

void RBwd()
{
  digitalWrite(EN2pin, HIGH);
  curBottomX |= _BV(MRNpin); 
  curBottomX &= ~_BV(MRPpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
}

void RStop()
{
  digitalWrite(EN2pin, LOW);
}

//--------------------------------------------
//--- Go Forward & Backward, All stop
//--------------------------------------------
void testFwdBwd()
{
  goForward();
  delay(2000);
  OffBothMotors();
  delay(2000);

  goBackward();
  delay(2000);
  OffBothMotors();
  delay(2000);
}

void goForward()
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  //disp4char_trippleX("FORD", 100);
  //disp4char_trippleX("FORD", 10);
  curBottomX |= _BV(MLPpin) | _BV(MRPpin); 
  curBottomX &= ~_BV(MLNpin) & ~_BV(MRNpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
}//goForward

void goBackward()
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  //disp4char_trippleX("BACK", 10);
  curBottomX |= _BV(MLNpin) | _BV(MRNpin);
  curBottomX &= ~_BV(MLPpin) & ~_BV(MRPpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
  delay(BWD_DURATION);
}//goBackward

void OffBothMotors()
{
  digitalWrite(EN1pin, LOW);
  digitalWrite(EN2pin, LOW);
  //disp4char_trippleX("STOP", 10);
}//OffBothMotors

//--------------------------------------------
//--- Turn Left & Right
//--------------------------------------------
void testTurn()
{
  turnLeft(RECT_TL_ITERATION);
  delay(2000);
  OffBothMotors();
  delay(2000); 
  

  turnRight(RECT_ITERATION);
  delay(2000);
  OffBothMotors();
  delay(2000);
  
}

void turnLeft(int duration)
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  //disp4char_trippleX("LEFT", 10);
  curBottomX |= _BV(MLNpin) | _BV(MRPpin);
  curBottomX &= ~_BV(MLPpin) & ~_BV(MRNpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
  delay(duration);
  OffBothMotors();
}//turnLeft

void turnRight(int duration)
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  //disp4char_trippleX("RITE", 10);
  curBottomX |= _BV(MLPpin) | _BV(MRNpin);
  curBottomX &= ~_BV(MLNpin) & ~_BV(MRPpin);
  control74HC595 (curTopX, curMiddleX, curBottomX);
  delay(duration);
  OffBothMotors();
}//turnRight

void turnClk()
{
  turnRight(TURN_CLOCK_ITERATION);
}//turnClk

void turnAntClk()
{
  turnLeft(TURN_ANT_CLOCK_ITERATION);
}//turnAntClk

//-------------------------------
void test293D()
{
  testLeftMotor_293D();
  delay(1000);
  testRightMotor_293D();
  delay(1000);
  testFwdBwd();
  delay(1000);
  testTurn();
  delay(1000);
}//test293D

//-------------------------------
void loopL293D()
{
  //test Left motor
  //testLeftMotor_293D();
  
  //test Right motor
  //testRightMotor_293D();
  
  //test go forward & backward
  //testFwdBwd();
  
  test293D();
}
  
