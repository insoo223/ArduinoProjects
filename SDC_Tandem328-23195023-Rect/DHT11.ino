#include <DHT_SimpleLib.h>
//#include <dht.h>

dht DHT;
 
void setupDHT11(){
  pinMode(DHT_DATApin, INPUT);
}//setup

void displayTemp_4digit7seg()
{
  DHT.read11(DHT_DATApin);
  disp4digit7seg_trippleX(DHT.temperature * 100 + DHT.humidity, 0x05, DISP_4digit7seg_ITERATION_TEMPHUMID); 
}

void loopDHT11()
{
  displayTemp_4digit7seg();
  //delay(3000);//Don't try to access too frequently... in theory
  //should be once per two seconds, fastest,
  //but seems to work after 0.8 second.
}// loop()
