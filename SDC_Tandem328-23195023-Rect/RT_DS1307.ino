// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

void setupDS1307 () {
  while (!Serial); // for Leonardo/Micro/Zero

  //Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC DS1307 is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //rtc.adjust(DateTime(2019, 3, 23, 20, 43, 0));
    rtc.adjust(DateTime(2019, 4, 8, 10, 5, 0));
  }
}
//---------------------------------------------
void setTimeDS1307()
{
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //rtc.adjust(DateTime(2019, 3, 26, 9, 20, 0));
    //rtc.adjust(DateTime(2019, 3, 26, 16, 28, 30));
    rtc.adjust(DateTime(2019, 4, 8, 10, 9, 0)); //Tue Apr 9, 2019: set 15sec faster 
}//setTimeDS1307

//---------------------------------------------
// added by Insoo (Mar 27, 2019)
void getNightMode()
{
  DateTime now = rtc.now();
  byte curHour = now.hour();
  byte curMonth = now.month();
  
  //Summer 
  if ((curMonth >= 3) && (curMonth <= 8))
  {
    //4digit 7seg LED in Dark mode before 7am and after 7pm, i.e. 7pm to next morning 7am 
    if ((curHour<7) || (curHour>18))
      nightMode = true;
    else
      nightMode=false;
  }
  //Winter 
  else
  {
    //4digit 7seg LED in Dark mode before 8am and after 5pm, i.e. 5pm to next morning 8am 
    if ((curHour<8) || (curHour>16))
      nightMode = true;
    else
      nightMode=false;
  }
  
}//getNightMode

//---------------------------------------------
void loopDS1307Serial() {
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
    delay(3000);
}//loopDS1307Serial

//---------------------------------------------
// added by Insoo (Mar 26, 2019)
void loopDS1307_4digit7Seg() {
    DateTime now = rtc.now();
    
    int y1,m1,d1,h1,mVal;
    //y1 = now.year();
    disp4digit7seg_trippleX(now.year(),0,50);
    disp4digit7seg_trippleX(now.month(),0,50);
    disp4digit7seg_trippleX(now.day(),0,50);
    
    disp4digit7seg_trippleX(now.dayOfTheWeek(),0,50); //0 SUN, 1 MON, 2 TUE ... 6 SAT
    disp4digit7seg_trippleX(now.hour(),0,50);
    disp4digit7seg_trippleX(now.minute(),0,50);
    disp4digit7seg_trippleX(now.second(),0,50);
}//loopDS1307_4digit7Seg

//---------------------------------------------
// added by Insoo (Tue, Mar 26, 2019)
void dispTimeYYYYMMDD_DS1307_4digit7Seg() 
{
    DateTime now = rtc.now();
    
    disp4digit7seg_trippleX(now.year(),0,50);
    disp4digit7seg_trippleX(now.month()*100 + now.day(),0,50);
    disp4digit7seg_trippleX(now.dayOfTheWeek(),0,50);
    //disp4char_trippleX(daysOfTheWeek[now.dayOfTheWeek()],50);
    //disp4digit7seg_trippleX(8888,0,50);
    //disp4char_trippleX("aaaa", 50);
    
    
}//dispTimeYYYYMMDD_DS1307_4digit7Seg

//---------------------------------------------
// added by Insoo (Mar 26, 2019)
void dispTimeHHMM_DS1307_4digit7Seg() {
    DateTime now = rtc.now();
    
    //disp4digit7seg_trippleX(now.dayOfTheWeek(),0,50); //0 SUN, 1 MON, 2 TUE ... 6 SAT
    disp4digit7seg_trippleX(now.hour()*100 + now.minute(),0,500);
    //disp4digit7seg_trippleX(now.second(),0,50);
}//dispTimeHHMM_DS1307_4digit7Seg

//---------------------------------------------
// added by Insoo (Tue Apr 2, 2019)
void loopRT_DS1307()
{
  //Serial.println("loopRT():");
  //Serial.println(analogRead(A5));
  //displayTime(); // display the real-time clock data on the Serial Monitor,
   dispTimeHHMM_DS1307_4digit7Seg();
  //delay(1000); // every second
}//loopRT_DS1307
