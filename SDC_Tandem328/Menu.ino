
#define MENU_BUTTON TactileButtonPinCD4021
#define ALT_BUTTON 7 //NOT YET assigned
#define menuReady 12
#define menuSelCompleteINTERVAL  2000

int menuCnt=0, altCnt=0, prevMenu=0;
unsigned long prevMS=0, curMS=0, menuLapse=0;  

char menuStr[10][5] = { "PUIR", "SETR", "TMHU", "CMPS", };

void menuSystem()
{
  //CountButton(ALT_BUTTON);
  if ( SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == HIGH) ////PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
    altCnt = 1;
  else
    altCnt = 0;
  CountButton(MENU_BUTTON);

  curMS = millis();
  menuLapse = curMS - prevMS;
  //disp4digit7seg_trippleX(menuCnt,0,20);
  if (menuLapse > menuSelCompleteINTERVAL)
  {
    //digitalWrite(menuReady, LOW); 
    if (menuCnt != 0)
    {
      prevMenu=menuCnt;
      //disp4digit7seg_trippleX(menuCnt,0,20);
      //ShowSelect();
    }
    if (altCnt == 0)
    {
      switch (menuCnt)
      {
        //main menu 1
        case 1:
        {
          //disp4char_trippleX("CKIR", 50);
          //getPureIRsignal(true, true);
          getPureIRsignal_SingleIR(false, true);
          //delay(1000);
          //disp4digit7seg_trippleX(1,1,50);
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //main menu 2
        case 2:
        {
          //disp4char_trippleX("TUIR", 50);
          IRmode = IR_SETmode; //IR_RUNmode IR_SETmode
          testIRs_ADC();
          //disp4digit7seg_trippleX(2,0,50);
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //main menu 3
        case 3:
        {
          loopRT();
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //main menu 4
        case 4:
        {
          //disp4char_trippleX("TIME", 50);
          loopRT();
          //disp4char_trippleX("TEMP", 50);
          loopDHT11();
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //main menu 5
        case 5:
        {
          bool dispUpsideDown;
          if ( SingleReadCD4021BE(SlideSwitch2ndPinCD4021) == HIGH) //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
            dispUpsideDown = true;
          else
            dispUpsideDown = false;
          measureUx(true, true, dispUpsideDown);
          //goForward();
          //chkSpeedByInterrupt(true, true);
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //main menu 6
        case 6:
        {
          initEEPROM();
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //main default menu
        default:
        {
          //loopRT();
          if (prevMenu == 0)
            disp4digit7seg_trippleX(0,0,30);
          menuCnt = prevMenu;
          altCnt = 0;
        }
      }//switch
    }//if (altCnt == 0)
    else if (altCnt == 1)
    {
      switch (menuCnt)
      {
        //alternative menu 1
        case 1: //and (altCnt == 1)
        {
          loopCompass();
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //alternative menu 2
        case 2: //and (altCnt == 1)
        {
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //alternative menu 3
        case 3: //and (altCnt == 1)
        {
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //alternative menu 4
        case 4:
        {
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //alternative menu 5
        case 5:
        {
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        //alternative menu 6
        case 6:
        {
          menuCnt = 0;
          altCnt = 0;
          break;
        }
        default:
        {
          menuCnt = 0;
          altCnt = 0;
        }
      }//switch
    }//else if (altCnt == 1)
    else
    {
        menuCnt = 0;
        altCnt = 0;
    }
    
    prevMS = millis();
    menuLapse = 0;
    //initPin();

  }//if (menuLapse > menuSelCompleteINTERVAL)
  else
  {
    ;
    //digitalWrite(menuReady, HIGH); 
  }//else
}//menuSystem

//------------------------------------
void CountButton(int button)
{
  //if pressed, LOW
  if (SingleReadCD4021BE(button) == LOW)
  {
    delay(200); // for debounce
    if (button == MENU_BUTTON)
    {
      menuCnt++;
      //disp4digit7seg_trippleX(menuCnt, 0, 30);
      ShowInterimSelect();
    }
    else if (button == ALT_BUTTON)
      altCnt++;
    prevMS = millis();
  }//if (digitalRead(button) == HIGH)
}//CountButton

//------------------------------------
void ShowInterimSelect()
{
  //disp4char_trippleX(menuStr[menuCnt], 30);
  disp4digit7seg_trippleX(menuCnt, 0, 10);
      
}//ShowInterimSelect

//------------------------------------
void SELF_DRIVING_IRfunc()
{
  int i;
  selfDriving_IRbased_obstacleAvoidance();
  selfDriving_UXbased_obstacleAvoidance();
  
  if (lapse>speedCheckInterval)
  {
    //check current speed 
    chkSpeedByInterrupt(false, true);
    //save the current speed in the speed history
    travelSpeedHistory[lapseCnt]=travelSpeed;
    //check average speed every SPEED_HISTORY_LIMIT times of unit lapse 
    if (lapseCnt == SPEED_HISTORY_LIMIT)
    {
      //calculated ave speed
      for (i=0; i<SPEED_HISTORY_LIMIT; i++)
        sumSpeed += travelSpeedHistory[i];
      aveSpeed=sumSpeed/SPEED_HISTORY_LIMIT;
      sumSpeed=0;
      
      /*-----------------
      if (aveCnt == AVE_HISTORY_LIMIT)
      {
        for (i=0; i<AVE_HISTORY_LIMIT; i++)
          grandSum += aveSpeedHistory[i];
        grandAve =  grandSum / AVE_HISTORY_LIMIT;
        grandSum=0;
        //show long-term ave speed
        //disp4digit7seg_trippleX(3000+grandAve, 0, 20);
        aveCnt=0;
        grandAveAvailable=true;
      }
      aveSpeedHistory[aveCnt]=aveSpeed;
      aveCnt++;
      --------------------------*/
      //if (grandAveAvailable)
        //if (aveSpeed < grandAve*0.85)
      /*-----
        if (travelSpeed < aveSpeed*0.9)
        {
          OffBothMotors();
          delay(100);
          goBackward();
          delay(100);
          decideTurnDirection();
          //grandAveAvailable=false;
        }
      ------*/
      //show ave speed
      disp4digit7seg_trippleX(1000+aveSpeed, 0, 20);
    
      lapseCnt=0;
    }//if (lapseCnt == SPEED_HISTORY_LIMIT)

    lapseCnt++;
    lapse=0;
    prevTime=millis();
  }//if (lapse>speedCheckInterval)
  

  //dispDirec(true);
}//SELF_DRIVING_IRfunc
