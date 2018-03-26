
volatile int encodingDiscCnt=0;
//------------------------------------------------
void ISRspeed()
{
  encodingDiscCnt++;
}

void chkSpeedByInterrupt(bool SerialDebug, bool sevenSeg)
{
  //delay(speedCheckInterval);
  travelDist = (float)tirePerimeter * (float)encodingDiscCnt / (float)NUMSLOTS;
  travelSpeed = travelDist/((float)speedCheckInterval/1000);
  if (SerialDebug)
  {
    Serial.print("encodingDiscCnt: ");
    Serial.println(encodingDiscCnt);
    Serial.print("travelDist: ");
    Serial.println(round(travelDist));
    Serial.print("travelSpeed [cm/s]: ");
    Serial.println(round(travelSpeed));
  }
  if (sevenSeg)
  {
    //disp4digit7seg_trippleX(travelSpeed, 0, 20);
  }
  encodingDiscCnt=0;
}//chkSpeedByInterrupt
//------------------------------------------------
bool checkSpeed_debugSerial=false; // false true
bool checkSpeed_debugSerialAvgSpeed=false; // false true
void checkSpeed()
{
  int adcSpeed;
  int adcData[25];
  int i=0; 
  int sum=0;
  int sentinel;
  //proceed time tick
  curTime = millis();

  calibratedSpeed=true;
  
  while (!calibratedSpeed)
  {
    goForward();
    //adcSpeed = SingleReadCD4021BE(MR_SPEEDpinCD4021);
    adcSpeed = digitalRead(MR_SPEEDpin);
    adcData[i++] = adcSpeed;
    delay(50);
    if (i==25)
    {
      calibratedSpeed = true;
      for (i=0; i<25; i++)
        sum += adcData[i];
      sentinel = sum/25;
      Serial.print("sentinel: ");
      Serial.println(sentinel);
    }
    
  }
  //Serial.println(adcSpeed);
  if (calibratedSpeed)
  {
    //when MR_SPEEDansistor is blocked
    //if ( digitalRead(MR_SPEEDpin) == HIGH)
    //adcSpeed = SingleReadCD4021BE(MR_SPEEDpinCD4021);
    adcSpeed = digitalRead(MR_SPEEDpin);
    
    //if ( adcSpeed > sentinel)
    if ( adcSpeed == HIGH)
    {
      //digitalWrite(sigLED, HIGH);
      if (checkSpeed_debugSerial)
        Serial.println("Blocked");
      curState = 1;
      if (prevState != curState)
      {
        prevState = 1;
      }
    }//if ( digitalRead(MR_SPEED) == HIGH)
    else
    {
      if (checkSpeed_debugSerial)
        Serial.println("Open");
      //digitalWrite(sigLED,  LOW);
      curState = 0;
      if (prevState != curState)
      {
        cntOpenSlot++;
        prevState = 0;
        
        //Serial.print("Passing slots: ");
        //Serial.println(cntOpenSlot);
      }//if (prevState != curState)
    }//else, that is ( digitalRead(MR_SPEED) == LOW)
  
    lapse = curTime - prevTime;
        
    if (lapse > speedCheckInterval)
    {
      travelDist = (float)tirePerimeter * (float)cntOpenSlot / (float)NUMSLOTS;
      if (checkSpeed_debugSerialAvgSpeed)
      {
        Serial.print("cntOpenSlot: ");
        Serial.println(cntOpenSlot);
        Serial.print("travelDist: ");
        Serial.println(round(travelDist));
      }
      travelSpeed = travelDist/(speedCheckInterval/1000);
      //Display speed
      //disp7seg2digits(travelSpeed, false, false);
      if (checkSpeed_debugSerialAvgSpeed)
      {
        Serial.print("travelSpeed [cm/s]: ");
        Serial.println(round(travelSpeed));
      }
      prevTime = curTime;
      cntOpenSlot=0;
      if ((prevSpeed == 0) && (travelSpeed == 0))
        //dispBlank();
      prevSpeed = travelSpeed;
    }//if (lapse > speedCheckInterval)
  }
}//checkSpeed()  


