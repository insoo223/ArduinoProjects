int FcombinedRcv, FambientRcv, FpureRcv, prevFpureRcv;
int LcombinedRcv, LambientRcv, LpureRcv, prevLpureRcv;
int RcombinedRcv, RambientRcv, RpureRcv, prevRpureRcv;

//Pin connected to ST_CP of 74HC595 & CD4021BE
int latchPin, latch4021;
//Pin connected to SH_CP of 74HC595 & CD4021BE
int clockPin, clock4021;  
////Pin connected to DS of 74HC595 & CD4021BE
int dataPin, data4021;

bool UNO = false; //false true

char curState74HC595=0x00; //0x00 0x08
char curBottomX=0x00, curMiddleX=0x00, curTopX=0x00;

bool doneTL=false;
bool doneTR=false;

const float tireD=6.60;
//const float  pi=3.14;
unsigned long cntOpenSlot=0, cntTirePerimeter=0;
int prevState=0, curState=0;
float tirePerimeter, travelDist, travelSpeed, prevSpeed;
float travelSpeedHistory[SPEED_HISTORY_LIMIT], aveSpeedHistory[AVE_HISTORY_LIMIT];
int aveCnt;
float sumSpeed=0, aveSpeed, grandSum=0, grandAve;
bool grandAveAvailable=false;

unsigned long lapse, lapseCnt,prevTime, curTime; 
unsigned long startTime,loopLapse, loopPrevTime=0;

// ADC value of Front IR sensor at 30cm away from an object using 3.7V LIPO battery
int FIR30; 
// ADC value of Front IR sensor at 15cm away from an object using 3.7V LIPO battery
int FIR15;

// ADC value of Left IR sensor at 30cm away from an object using 3.7V LIPO battery
int LIR30;
// ADC value of Left IR sensor at 15cm away from an object using 3.7V LIPO battery
int LIR15;

// ADC value of Right IR sensor at 30cm away from an object using 3.7V LIPO battery
int RIR30;
// ADC value of Right IR sensor at 15cm away from an object using 3.7V LIPO battery
int RIR15;

// backward distance
int FIR_BW = 700; 
int Ux_BW = 5;

int prevMode;
int cntTURN_LEFT_MODE=0;
int cntTURN_RIGHT_MODE=0;

int  Ldist, Rdist, Lunit, Runit;
int ref_front;
int ref_frontUx = 8;

int IRmode;

bool calibratedSpeed=false;

int statusTop74HC595;
int statusMiddle74HC595;
int statusBottom74HC595;

bool nightMode=false; //false true
int singleDigitDelay=4;

int playMode = MENU_SYSTEM;


