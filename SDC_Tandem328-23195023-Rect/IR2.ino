

//------------------------------
//added by Insoo (Apr 4, 2019)


//------------------------------
//added by Insoo (Apr 3, 2019)
void testSingleIR_Motor(byte loc)
{
  
  //getPureIRsignal_Single(loc, true, true);
  getPureIRsignal_Single(loc, false, false);
  
  //delay(200);
  if (FpureRcv > 22)
  {
    goBackward(); 
    delay(500);
    OffBothMotors();
    delay(200);
  }  
  else if (FpureRcv > 20)
  {
    OffBothMotors();
    disp4digit7seg_trippleX(FpureRcv, 0, 30);
    //getPureIRsignal_Single(IR_LEFT, false, false);
    //getPureIRsignal_Single(IR_RIGHT, false, false);
    //setUnitAndRefFront30();
    //Serial.print("L, R pureRcv: ");Serial.print(LpureRcv); Serial.print(", "); Serial.println(RpureRcv);
    //Serial.print("L, R dist: ");Serial.print(Ldist); Serial.print(", "); Serial.println(Rdist);
    //decideTurnDirection();
    //turnRight(RECT_ITERATION);
    getPureIRsignal_Single(loc, false, false);
    //goBackward();
    //delay(200);
  }
  else
  {
    goForward();  
    //delay(100);
  }
/*
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
*/
}//testSingleIR
