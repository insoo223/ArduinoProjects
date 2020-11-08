/*
 * HMC5883L Demo.
 * dipmicro electronics, 2014
 *
 * Hardware Used:
 * 
 * Arduino UNO or compatible
 * GY271 module (dipmicro part DE4196
 *  Arduino GND -> GY271/HMC5883L GND
 *  Arduino 3.3V -> GY271/HMC5883L VCC
 *  Arduino A4 (SDA) -> GY271/HMC5883L SDA
 *  Arduino A5 (SCL) -> GY271/HMC5883L SCL 
 ref) https://www.dipmicro.com/store/GY271-MOD 
 */

#include <Wire.h> //I2C Arduino Library

#define HMC5883L_ADDR 0x1E //0011110b, I2C 7bit address of HMC5883

bool haveHMC5883L = false;

#define HMC5883_WriteAddress 0x1E //  i.e 0x3C >> 1
#define HMC5883_ModeRegisterAddress 0x02
#define HMC5883_ContinuousModeCommand 0x00
#define HMC5883_DataOutputXMSBAddress  0x03
 
int regb=0x01;
int regbdata=0x40;
 
int outputData[6];

//-----------------------------------
bool detectHMC5883L ()
{
  // read identification registers
  Wire.beginTransmission(HMC5883L_ADDR); //open communication with HMC5883
  Wire.write(10); //select Identification register A
  Wire.endTransmission();
  Wire.requestFrom(HMC5883L_ADDR, 3);
  if(3 == Wire.available()) {
    char a = Wire.read();
    char b = Wire.read();
    char c = Wire.read();
    if(a == 'H' && b == '4' && c == '3')
      return true;
  }

  return false;
}//detectHMC5883L

//-----------------------------------
void setupCompass()
{
  //Initialize Serial and I2C communications
  //Serial.begin(9600);
  //Serial.println("GY271 TEST");
  Wire.begin();
  // lower I2C clock http://www.gammon.com.au/forum/?id=10896
  TWBR = 78;  // 25 kHz 
  TWSR |= _BV (TWPS0);  // change prescaler  
}//detectHMC5883L

//------------------------------------
void getXYZ(bool SerialDebug)
{
  bool detect = detectHMC5883L();

  if(!haveHMC5883L) 
  {
    if(detect) 
    {
      haveHMC5883L = true;
      if (SerialDebug)
        Serial.println("We have HMC5883L, moving on");
      // Put the HMC5883 IC into the correct operating mode
      Wire.beginTransmission(HMC5883L_ADDR); //open communication with HMC5883
      Wire.write(0x02); //select mode register
      Wire.write(0x00); //continuous measurement mode
      Wire.endTransmission();
    }
    else
    {  
      if (SerialDebug)
        Serial.println("No HMC5883L detected!");
      delay(2000);
      return;
    }
  }
  else
  {
    if(!detect) {
      haveHMC5883L = false;
      if (SerialDebug)
        Serial.println("Lost connection to HMC5883L!");
      delay(2000);
      return;
    }
  }
  
  int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC5883L_ADDR, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  
  float angle= atan2((float)y,(float)x) * (180 / 3.14159) + 180;
  //Print out values of each axis
  if (SerialDebug)
  {
    Serial.print("x: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(angle);
    
    //disp4char_trippleX("   A", 30);
    //disp4digit7seg_trippleX(angle, 0, 50);
    //disp4char_trippleX("   X", 30);
    //disp4digit7seg_trippleX(x, 0, 50);
    Serial.print("  y: ");
    //disp4char_trippleX("   Y", 30);
    //disp4digit7seg_trippleX(abs(y), 0, 50);
    Serial.print(y);
    Serial.print("  z: ");
    Serial.println(z);
  
    delay(500);
  }
  
  //check NORTH using angle
  if (angle > angleNORTH-angleNORTH_margin)
    //NORTH angle varies too fast, so that check x-coord. to get more precise directivity
    if ( (y > yNORTH-yNORTH_margin) && (y < yNORTH+yNORTH_margin) )
      disp4char_trippleX("  NN", 10);

  //check SOUTH using angle
  if (angle < angleSOUTH+angleSOUTH_margin)
    if ((y > ySOUTH_marginLow) && (y < ySOUTH_marginHigh) )
      disp4char_trippleX("  SS", 10);
    
  if ( (angle > angleEAST-angleEAST_margin) && (angle < angleEAST+angleEAST_margin) )
  {
    if ((x > xEAST-xEAST_margin) && (x < xEAST+xEAST_margin))
      disp4char_trippleX("  EE", 10);
  }
  if ( (angle > angleWEST-angleWEST_margin) && (angle < angleWEST+angleWEST_margin) )
  {
    if ((x > xWEST-xWEST_margin) && (x < xWEST+xWEST_margin))
      disp4char_trippleX("  WW", 50);
  }
}//getXYZ
	
//-----------------------

int getDirection(bool SerialDebug, int *x, int *y, int *z, float *angle)
{
  bool detect = detectHMC5883L();

  if(!haveHMC5883L) 
  {
    if(detect) 
    {
      haveHMC5883L = true;
      if (SerialDebug)
        Serial.println("We have HMC5883L, moving on");
      // Put the HMC5883 IC into the correct operating mode
      Wire.beginTransmission(HMC5883L_ADDR); //open communication with HMC5883
      Wire.write(0x02); //select mode register
      Wire.write(0x00); //continuous measurement mode
      Wire.endTransmission();
    }
    else
    {  
      if (SerialDebug)
        Serial.println("No HMC5883L detected!");
      delay(2000);
      return 0;
    }
  }
  else
  {
    if(!detect) {
      haveHMC5883L = false;
      if (SerialDebug)
        Serial.println("Lost connection to HMC5883L!");
      delay(2000);
      return 0;
    }
  }
  

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC5883L_ADDR, 6);
  if(6<=Wire.available()){
    *x = Wire.read()<<8; //X msb
    *x |= Wire.read(); //X lsb
    *z = Wire.read()<<8; //Z msb
    *z |= Wire.read(); //Z lsb
    *y = Wire.read()<<8; //Y msb
    *y |= Wire.read(); //Y lsb
  }
  
  *angle= atan2((float)*y,(float)*x) * (180 / 3.14159) + 180;
  //Print out values of each axis
  if (SerialDebug)
  {
    Serial.print("x: ");
    Serial.print(*x);
    Serial.print(", ");
    Serial.print(*angle);
    Serial.print("  y: ");
    Serial.print(*y);
    Serial.print("  z: ");
    Serial.println(*z);
  
    delay(500);
  }
  
  //check NORTH using angle
  if (*angle > angleNORTH-angleNORTH_margin)
    //NORTH angle varies too fast, so that check x-coord. to get more precise directivity
    if ( (*y > yNORTH-yNORTH_margin) && (*y < yNORTH+yNORTH_margin) )
    {
      //disp4char_trippleX("  NN", 10);
      return NORTH;
    }

  //check SOUTH using angle
  if (*angle < angleSOUTH+angleSOUTH_margin)
  {
    //disp4char_trippleX("  SS", 10);
    return SOUTH;
  }
    
  if ( (*angle > angleEAST-angleEAST_margin) && (*angle < angleEAST+angleEAST_margin) )
  {
    if ((*x > xEAST-xEAST_margin) && (*x < xEAST+xEAST_margin))
    {
      //disp4char_trippleX("  EE", 10);
      return EAST;
    }
  }
  
  if ( (*angle > angleWEST-angleWEST_margin) && (*angle < angleWEST+angleWEST_margin) )
  {
    if ((*x > xWEST-xWEST_margin) && (*x < xWEST+xWEST_margin))
    {
      //disp4char_trippleX("  WW", 10);
      return WEST;
    }
  }
  
  return -1;
}//getDirection

//------------------------ 
void printDirection() 
{
 
    int i,x,y,z;
    double angle;
 
    Wire.beginTransmission(HMC5883_WriteAddress);
    Wire.write(regb);
    Wire.write(regbdata);
    Wire.endTransmission();
 
    delay(1000);
    Wire.beginTransmission(HMC5883_WriteAddress); //Initiate a transmission with HMC5883 (Write address).
    Wire.write(HMC5883_ModeRegisterAddress);       //Place the Mode Register Address in send-buffer.
    Wire.write(HMC5883_ContinuousModeCommand);     //Place the command for Continuous operation Mode in send-buffer.
    Wire.endTransmission();                       //Send the send-buffer to HMC5883 and end the I2C transmission.
    delay(100);
 
 
    Wire.beginTransmission(HMC5883_WriteAddress);  //Initiate a transmission with HMC5883 (Write address).
    Wire.requestFrom(HMC5883_WriteAddress,6);      //Request 6 bytes of data from the address specified.
 
    delay(500);
 
 
    //Read the value of magnetic components X,Y and Z
 
    if(6 <= Wire.available()) // If the number of bytes available for reading be <=6.
    {
        for(i=0;i<6;i++)
        {
            outputData[i]=Wire.read();  //Store the data in outputData buffer
        }
    }
 
    x=outputData[0] << 8 | outputData[1]; //Combine MSB and LSB of X Data output register
    z=outputData[2] << 8 | outputData[3]; //Combine MSB and LSB of Z Data output register
    y=outputData[4] << 8 | outputData[5]; //Combine MSB and LSB of Y Data output register
 
 
    angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees
 
    /*
 
  Refer the following application note for heading calculation.
  http://www.ssec.honeywell.com/magnetic/datasheets/lowcost.pdf  
  ----------------------------------------------------------------------------------------
  atan2(y, x) is the angle in radians between the positive x-axis of a plane and the point
  given by the coordinates (x, y) on it. 
  ----------------------------------------------------------------------------------------
 
  This sketch does not utilize the magnetic component Z as tilt compensation can not be done without an Accelerometer
 
  ----------------->y
  |
  |
  |
  |
  |
  |
 \/
  x 
 
 
 
     N 
 NW  |  NE
     |  
W----------E
     |
 SW  |  SE
     S
 
 */ 
 
 
    //Print the approximate direction
 
    Serial.print("You are heading ");
    if((angle < 22.5) || (angle > 337.5 ))
    {
        Serial.print("South");
        disp4char_trippleX("SOTH", 50);
    }
    if((angle > 22.5) && (angle < 67.5 ))
    {
        Serial.print("South-West");
        disp4char_trippleX("SOWE", 50);
    }
    if((angle > 67.5) && (angle < 112.5 ))
    {
        Serial.print("West");
        disp4char_trippleX("WEST", 50);
    }
    if((angle > 112.5) && (angle < 157.5 ))
    {
        Serial.print("North-West");
        disp4char_trippleX("NOWE", 50);
    }
    if((angle > 157.5) && (angle < 202.5 ))
    {
        Serial.print("North");
        disp4char_trippleX("NOTH", 50);
    }
    if((angle > 202.5) && (angle < 247.5 ))
    {
        Serial.print("NorthEast");
        disp4char_trippleX("NOEA", 50);
    }
    if((angle > 247.5) && (angle < 292.5 ))
    {
        Serial.print("East");
        disp4char_trippleX("EAST", 50);
    }
    if((angle > 292.5) && (angle < 337.5 ))
    {
        Serial.print("SouthEast");
        disp4char_trippleX("SOEA", 50);
    }
 
    Serial.print(": Angle between X-axis and the South direction ");
    if((0 < angle) && (angle < 180) )
    {
        angle=angle;
    }
    else
    {
        angle=360-angle;
    }
    Serial.print(angle,2);
    Serial.println(" Deg");
    delay(100);
}//printDirection

//--------------------------------------------------
bool DONE_ROUND=false, DONE_QUARTER_ROUND=false;
//--------------------------------------------------
void loopCompass()
{
  /*
  while (!DONE_ROUND)
    chkRoundIteration();
  delay(2000);    
  while (!DONE_QUARTER_ROUND)
    chkQuarterRound();
  */
  chkCompass(true);
  //printDirection();
  //getXYZ(false);
  /*
  int d = getDirection(false);
  if (d != -1)
    //Serial.println(d);
    disp4digit7seg_trippleX(d, 0, 10);
  */
}

void chkQuarterRound()
{
  int i, k;

  for (k=0; k<4; k++)
  {
    for (i=0; i<ROUND_ITERATION/4; i++)
      turnRight(50);
    delay(1000);
  }

  DONE_QUARTER_ROUND=true;
}//chkRoundIteration

void chkRoundIteration()
{
  for (int i=0; i<ROUND_ITERATION; i++)
    turnRight(50);
  DONE_ROUND=true;
}//chkRoundIteration

//-----------------------------------------
void chkCompass(bool disp7seg)
{
  int i, k;
  int d;
  int x, y, z;
  float angle;
  float angleN[MAX_ANGLE_ARRAY], angleS[MAX_ANGLE_ARRAY], angleE[MAX_ANGLE_ARRAY], angleW[MAX_ANGLE_ARRAY];
  int cntN=0, cntS=0, cntE=0, cntW=0;
  
  //get rough direction range
  disp4char_trippleX("RNGE", 100);
  for (i=0; i<ROUND_ITERATION; i++)
  {
    turnRight(50);
    
    d = getDirection(false, &x, &y, &z, &angle);
    switch(d)
    {
      case NORTH:
        angleN[cntN++]=angle;
        if (disp7seg) 
          disp4char_trippleX("   N", 50);
        break;
      case EAST:
        angleE[cntE++]=angle;
        if (disp7seg) 
          disp4char_trippleX("   E", 50);
        break;
      case SOUTH:
        angleS[cntS++]=angle;
        if (disp7seg) 
          disp4char_trippleX("   S", 50);
        break;
      case WEST:
        angleW[cntW++]=angle;
        if (disp7seg) 
          disp4char_trippleX("   W", 50);
        break;
    }//switch(d)
    delay(200);
  }//for to get rough direction range
  
  disp4char_trippleX("PRCS", 100);
  for (i=0; i<ROUND_ITERATION; i++)
  {
    turnRight(50);
    
    d = getDirection(false, &x, &y, &z, &angle);
    switch(d)
    {
      case NORTH:
        if (abs(angle - angleN[cntN/2]) < angleNORTH_margin/5)
          if (disp7seg) 
            disp4char_trippleX("  NN", 50);
        break;
      case EAST:
        if (abs(angle - angleE[cntE/2]) < angleEAST_margin/5)
          if (disp7seg) 
            disp4char_trippleX("  EE", 50);
        break;
      case SOUTH:
        if (abs(angle - angleS[cntS/2]) < angleSOUTH_margin/5)
          if (disp7seg) 
            disp4char_trippleX("  SS", 50);
        break;
      case WEST:
        if (abs(angle - angleW[cntW/2]) < angleWEST_margin/10);
          if (disp7seg) 
            disp4char_trippleX("  WW", 50);
        break;
    }//switch(d)
    delay(200);
  }//for to show precise direction
}//chkCompass
  
//----------------------------------------
void dispDirec(bool disp7seg)
{
  int d;
  int x, y, z;
  float angle;
  
  d = getDirection(false, &x, &y, &z, &angle);
  switch(d)
  {
    case NORTH:
      if (disp7seg) 
        disp4char_trippleX("   N", 5);
      break;
    case EAST:
      if (disp7seg) 
        disp4char_trippleX("   E", 5);
      break;
    case SOUTH:
      if (disp7seg) 
        disp4char_trippleX("   S", 5);
      break;
    case WEST:
      if (disp7seg) 
        disp4char_trippleX("   W", 5);
      break;
  }//switch(d)
}
    
