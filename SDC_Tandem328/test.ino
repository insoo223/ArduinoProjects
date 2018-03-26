
//------ test read a single bit from CD4021BE 
void testCD4021BE()
{
  OffBothMotors();
  Serial.print("PGM_MODEpinCD4021: ");
  Serial.println( SingleReadCD4021BE(PGM_MODEpinCD4021) );
  delay(200);
  Serial.print("TactileButtonPinCD4021: ");
  Serial.println( SingleReadCD4021BE(TactileButtonPinCD4021) );
  delay(200);
  Serial.print("SlideSwitch2ndPinCD4021: ");
  Serial.println( SingleReadCD4021BE(SlideSwitch2ndPinCD4021) );
  delay(200);
  Serial.println("-------------------");
  
}

//-------------------------------------------------- 
void showSameNum() 
{
  
  for (int i=0; i<4; i++)
  {
    for(int k=0; k<50; k++)
    {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //control 1s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x02); //control 10s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x04); //control 100s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x08); //control 1000s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
    }
  }
}//showSameNum

//-------------------------------------------------- 
void showDiffNum() 
{
  
  for (int i=0; i<10; i++)
  {
    for(int k=0; k<50; k++)
    {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i+3]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x02); //control 10s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i+2]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x04); //control 100s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i+1]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x08); //control 1000s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
    }
  }
}//showDiffNum

//-----------------------------------------
// Test Motor
//-----------------------------------------
//-----------------------------------------
void testTurnLeft()
{
  while (!doneTL)
  {
    turnLeft(RECT_TL_ITERATION);
    OffBothMotors();
    delay(500);
    turnLeft(SHORT_TURN_ITERATION);
    OffBothMotors();
    delay(STOP_DURATION);
    
    doneTL=true;
  }
}//testTurnLeft

//----------------------------------------------
void testTurnRight()
{
  while (!doneTR)
  {
    turnRight(RECT_ITERATION);
    OffBothMotors();
    delay(STOP_DURATION);
    doneTR=true;
  }
}//testTurnRight

//----------------------------------------------




