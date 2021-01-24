//**************************************************************//
//  Name    : RT.ino
//  Author  : Insoo Kim
//  Create  : Oct 8, 2015 
//  Update  : Nov 8, 2020    
//  Caution : NOT for DS1307 (see RT_DS1307 for SeeedStudio PCB by Insoo (ver.JAN 2019)
//  Notes   : Real-Time clock module for SDC_Tandem328.ino 
//            Self-driving car on a single perforated board with w/ATmega328P in parallel with Arduino Pro Mini
//            Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//****************************************************************
#include "Wire.h"

//#define DS1307 //DS3231

//#ifdef DS3231
  #define DS3231_I2C_ADDRESS 0x68
//#ifdef DS1307
  //#define DS1307_I2C_ADDRESS 0x68


//----------------------------------------------------
void setupRT()
{
  Wire.begin();

  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //day 1 for Sunday, day 3 for Tuesday.. but will be substraced by one to match oriental concept
  //so that dispalying 1 for Monday
  //setDS3231time(0,37,13,3,7,11,17);
  //setDS3231time(0,37,20,6,23,3,19);
  //setDS3231time(0,40,20,0,4,10,20);
  //setDS3231time(0,34,16,1,5,10,20);
  //setDS3231time(0,50,9,0,18,10,20); //2020.10.18 Sun 0950
  //setDS3231time(0,37,17,0,8,11,20); //2020.11.8 Sun 1737
  //setDS3231time(0,40,8,4,10,12,20); //2020.12.10 Thu 0840
  //setDS3231time(0,55,15,4,31,12,20); //2020.12.31 Thu 1555
  //setDS3231time(0,58,20,0,24,1,21); //2021.1.24 Sun 2058
  setDS3231time(0,43,8,1,25,1,21); //2021.1.25 Mon 0843
}//setupRT

//----------------------------------------------------
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}//decToBcd

//----------------------------------------------------
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}//bcdToDec

//----------------------------------------------------
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}//setDS3231time

//----------------------------------------------------
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  if ((*hour >= 18) || (*hour <= 7))
  {
    nightMode = true;
    singleDigitDelay = 0;
  }
  else
  {
    nightMode = false; //false true
    singleDigitDelay = 4;
  }
  
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}//readDS3231time

//----------------------------------------------------
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,  &year);
  // send it to the serial monitor
  Serial.println("Current time is...");
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}//displayTime

//----------------------------------------------------
void displayTime_4digit7seg()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  int fourDigits;
  byte chinese_dayOfWeek;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  //kinda animated clock effect
  if (CLOCK_ANIMATION_WHEEL)
    if ((minute == 0) || (minute == 30))
      testFwdBwd();
  
  fourDigits = month*100 + dayOfMonth;
  disp4digit7seg_trippleX(fourDigits, 0, DISP_4digit7seg_ITERATION_DATE);  
  
  chinese_dayOfWeek = dayOfWeek-1;
  if (chinese_dayOfWeek == 0)
    chinese_dayOfWeek = 7;
  disp4digit7seg_trippleX(chinese_dayOfWeek, 0, DISP_4digit7seg_ITERATION_DAY);  

  fourDigits = hour*100 + minute;
  disp4digit7seg_trippleX(fourDigits, 0, DISP_4digit7seg_ITERATION_TIME);  
  /*--------------
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  --------------*/
}//displayTime_4digit7seg

//----------------------------------------------------
void loopRT()
{
  //Serial.println("loopRT():");
  //Serial.println(analogRead(A5));
  //displayTime(); // display the real-time clock data on the Serial Monitor,
  displayTime_4digit7seg();
  //delay(1000); // every second
}//loopRT
