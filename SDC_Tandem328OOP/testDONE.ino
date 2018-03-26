/* --------------------------------------------
  testDONE.ino
  Successfully accomplished test functions.
  Created: Oct 27, 2015
  Updated: Oct 29, 2015
  Author: Insoo Kim (insoo@hotmail.com)
-------------------------------------------- */

//-------------------------------------------
void dispGlobals(int duration)
{
    Serial.print(gCurTopX); Serial.print(", "); 
    Serial.print(gCurMidX); Serial.print(", "); 
    Serial.println(gCurBotX); 
    delay(duration);
}//dispGlobals

//-------------------------------------------
// tripple 74HC595 output expansion chips' controls
//-------------------------------------------
void test_trippleX()
{
  for (int i=0; i<4; i++)
  {
    X.ctrlAll(0x5, 1<<i, 0);
    dispGlobals(1000);
  }
  
  X.ctrlAll(0,0,0); //0,0,0
  dispGlobals(1000);
  
  X.ctrlSingle(0x20,HIGH); //1,0,0
  dispGlobals(1000);  
  
  X.ctrlSingle(0x10,HIGH); //1,1,0
  dispGlobals(1000);
  
  X.ctrlSingle(0x20,LOW); //0,1,0
  dispGlobals(1000);
}//test_trippleX  

//-------------------------------------------
// 7 segment LEDs (4 digits)
//-------------------------------------------
// Seen from the car rear side(default aspect). Display from 0 to 9999. Takes time.
void test_trippleX_7segNum()
{

  for (int i=0; i<10000; i++)
    _7X.disp4digits(i,0, 20);
}//test_trippleX_7segNum

//-------------------------------------------
// Display from A to Z and some movement action defined by Insoo
void test_trippleX_7segChar()
{
  _7X.disp4chars("ABCD", 200);
  _7X.disp4chars("EFGH", 200);
  _7X.disp4chars("IJKL", 200);
  _7X.disp4chars("MNOP", 200);
  _7X.disp4chars("QRST", 200);
  _7X.disp4chars("UVWX", 200);
  _7X.disp4chars("YZAB", 200);
  _7X.disp4chars(" FWD", 200); //forward
  _7X.disp4chars(" BWD", 200); //backward
  _7X.disp4chars("TNCK", 200); //turn clockwise
  _7X.disp4chars("TNAC", 200); //turn anti-clockwise
}//test_trippleX_7segChar

//-------------------------------------------
// Seen from the car front side. Display from 0 to 9999. Takes time.
void test_trippleX_7segNum_UpsideDown()
{
  for (int i=0; i<10000; i++)
    _7X.disp4digits_UpsideDown(i,0, 20);
}//test_trippleX_7segNum_UpsideDown

//-------------------------------------------
// L293D Motor driver
//-------------------------------------------
// Forward Move. With 3.7V mobile rechargeable battery, 
//  sometimes initial torque of right side motor is not enouph to kick-off.
void test_L293DX_fwd()
{
  motor.fwd(2000);
  motor.stop(1000);
}//test_L293DX_fwd

void test_L293DX_bwd()
{
  motor.bwd(2000);
  motor.stop(1000);
}//test_L293DX_bwd

//-------------------------------------------
void test_L293DX()
{

  _7X.disp4chars(STR_LFWD, 100);
  motor.LFwd(1000);
  _7X.disp4chars(STR_LSTP, 100);
  motor.LStop(1000);
  _7X.disp4chars(STR_LBWD, 100);
  motor.LBwd(1000);
  _7X.disp4chars(STR_LSTP, 100);
  motor.LStop(1000);

  _7X.disp4chars(STR_RFWD, 100);
  motor.RFwd(1000);
  _7X.disp4chars(STR_RSTP, 100);
  motor.RStop(1000);
  _7X.disp4chars(STR_RBWD, 100);
  motor.RBwd(1000);
  _7X.disp4chars(STR_RSTP, 100);
  motor.RStop(1000);

  _7X.disp4chars(STR_FWD, 100);
  motor.fwd(1000);
  _7X.disp4chars(STR_STOP, 100);
  motor.stop(1000);
  _7X.disp4chars(STR_BWD, 100);
  motor.bwd(1000);
  _7X.disp4chars(STR_STOP, 100);
  motor.stop(1000);
  
  _7X.disp4chars(STR_TNLF, 100);
  motor.turnLeft(RECT_TURN_DURATION, STOP_DURATION_AFTER_TURN);
  _7X.disp4chars(STR_STOP, 100);
  motor.stop(1000);
  _7X.disp4chars(STR_TNRT, 100);
  motor.turnRight(RECT_TURN_DURATION, STOP_DURATION_AFTER_TURN);
  _7X.disp4chars(STR_STOP, 100);
  motor.stop(1000);
  
  _7X.disp4chars(STR_TNCK, 100);
  motor.turnClk(TURN_CLOCK_DURATION, STOP_DURATION_AFTER_TURN);
  _7X.disp4chars(STR_STOP, 100);
  motor.stop(1000);
  _7X.disp4chars(STR_TNAC, 100);
  motor.turnAntClk(TURN_ANT_CLOCK_DURATION, STOP_DURATION_AFTER_TURN);
  _7X.disp4chars(STR_STOP, 100);
  motor.stop(1000);
}//test_L293DX

//-------------------------------------------
// Real-time clock with DS3231 chip
//-------------------------------------------
// Display current time read from the RTC module.
void test_RTX()
{
  rt.displayTime_4digit7seg();
}//test_RTX

// Display temperature and humidity read from the DHT11 digital temperature & humidity sensor.
void test_DHT11X()
{
  dht11.displayTempHumidity(2000);
}//test_DHT11X
//-------------------------------------------
// single CD4021BE input expansion chip
//-------------------------------------------
// Slide switch test
void test_CD4021X()
{
  uint8_t inputVal1, inputVal2, inputVal3;
  
  inputVal1 = cd4021.readPGM_MODEslideSW(); //Left-most side slide switch
  inputVal2 = cd4021.readTactileButton(); // Push button switch
  inputVal3 = cd4021.read2ndSlideSW(); // the 2nd left side slide switch, the 3rd one is for power switch
  Serial.print(inputVal1);
  Serial.print(inputVal2);
  Serial.println(inputVal3);
  _7X.disp4digits(inputVal1*100 + inputVal2*10 + inputVal3, 0, 50);
  delay(500);
}//test_CD4021X

//-------------------------------------------
// IR sensitivity & motor driving cotrol logic 
//-------------------------------------------
void test_IRX_getPure_Single()
{
  int FpureRcv, LpureRcv, RpureRcv;

  Serial.print(STR_FRNT);
  FpureRcv = ir.getPure_SingleIR(IR_FRONT, true, true);
  Serial.print(STR_LEFT);
  LpureRcv = ir.getPure_SingleIR(IR_LEFT, true, true);
  Serial.print(STR_RITE);
  RpureRcv = ir.getPure_SingleIR(IR_RIGHT, true, true);
  Serial.println();
}//test_IRX_getPure_Single

//-------------------------------------------
void test_IRX_getPure_All()
{
  ir.getPure_AllIRs(true, true);
}//test_IRX_getPure_All

//-------------------------------------------
void test_IRX_initEEPROM()
{
  ir.initEEPROM();
}//test_IRX_initEEPROM

//-------------------------------------------
void test_IRX_saveRefPure_Single()
{
  ir.saveRefPure_SingleIR(IR_FRONT, true, true);
  ir.saveRefPure_SingleIR(IR_LEFT, true, true);
  ir.saveRefPure_SingleIR(IR_RIGHT, true, true);
}//test_IRX_saveRefPure_Single

//-------------------------------------------
void test_IRX_saveRefPure_All()
{
  ir.saveRefPure_AllIR(true, true);
}//test_IRX_saveRefPure_All

//-------------------------------------------
void test_IRX_runSDC()
{
  ir.runSDC(false, false);
}//test_IRX_runSDC
//-------------------------------------------
//-------------------------------------------
/*********** test functions
test functions ************/
