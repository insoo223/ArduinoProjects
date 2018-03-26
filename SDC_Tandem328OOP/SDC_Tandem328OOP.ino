/*
  SDC_OOP.ino

  Created by In soo Kim on 10/24/15.
  Release to Public domain

  Updated on 10/29/15
*/

#include "defs.h"

#include <trippleX.h>
#include <Four7segX.h>
#include <L293DX.h>
#include <RTX.h>
#include <DHT11X.h>
#include <CD4021X.h>
#include <IRX.h>
#include <ButtonMenu.h>
#include <UXX.h>
/*******include
#include <EEPROM.h>

//#include <ICCX.h>
#include "Wire.h"
include*******/

trippleX X;
//1st 3 numbers: latch clock data - direct GPIO pin of Arduino 
//the following 8 numbers: A-G & DP pin from 74HC595 of the middle of trippleX
Four7segX _7X;
L293DX motor;
RTX rt;
DHT11X dht11;
CD4021X cd4021;
IRX ir;
BUTTONMENU menu;
UXX ux;
/*
*/
void setup()
{
  //Set time to DS3231 RTC module
  //rt.setDS3231time(30,35,11,5,19,5,17);

  Serial.begin(9600);
  //rt.dispBrightnessCtrl();
  
  X.ctrlAll(0,0,0);

}//setup

void loop()
{
  //Production-time code requires only the following single statement!
  //To test SDC_tandem328P for it has NO RTC
  //   so that, night time brightness cannot be controlled.
  gNightMode = false;
  menu.selectOpMode();
  
  //test_Master();
  //ir.saveRefPure_AllIR(true, true);
  //test_IRX_initEEPROM();
  //test_IRX_getPure_Single();
  //test_CD4021X();
  //test_DHT11X();
  //test_RTX();
  //test_L293DX();
  //test_trippleX_7segNum_UpsideDown();
  //test_trippleX_7segNum();
  //test_trippleX_7segChar();
}//loop
