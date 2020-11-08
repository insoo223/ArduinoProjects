

extern int FcombinedRcv, FambientRcv, FpureRcv, prevFpureRcv;
extern int LcombinedRcv, LambientRcv, LpureRcv, prevLpureRcv;
extern int RcombinedRcv, RambientRcv, RpureRcv, prevRpureRcv;
extern int CliffFcombinedRcv, CliffFambientRcv, CliffFpureRcv, CliffprevFpureRcv;

extern bool UNO;

//Pin connected to ST_CP of 74HC595 & CD4021BE
extern int latchPin, latch4021;
//Pin connected to SH_CP of 74HC595 & CD4021BE
extern int clockPin, clock4021;
////Pin connected to DS of 74HC595 & CD4021BE
extern int dataPin, data4021;

extern char LEDtest[9];
extern char OnesDigit[18];


extern char curState74HC595;
extern char curBottomX, curMiddleX, curTopX;

extern bool doneTL;
extern bool doneTR;
extern float tirePerimeter, travelSpeed, prevSpeed;
extern const float tireD;
extern unsigned long curTime, prevTime, lapse, lapseCnt;
extern float travelSpeedHistory[SPEED_HISTORY_LIMIT], aveSpeedHistory[AVE_HISTORY_LIMIT];
extern int aveCnt;
extern float sumSpeed, aveSpeed, grandSum, grandAve;
extern bool grandAveAvailable;

// ADC value of Front IR sensor at 30cm away from an object using 3.7V LIPO battery
extern int FIR30;
// ADC value of Front IR sensor at 15cm away from an object using 3.7V LIPO battery
extern int FIR15;
extern int FIR10;

// ADC value of Left IR sensor at 30cm away from an object using 3.7V LIPO battery
extern int LIR30; 
// ADC value of Left IR sensor at 15cm away from an object using 3.7V LIPO battery
extern int LIR15;
extern int LIR10; 

// ADC value of Right IR sensor at 30cm away from an object using 3.7V LIPO battery
extern int RIR30;
// ADC value of Right IR sensor at 15cm away from an object using 3.7V LIPO battery
extern int RIR15;
extern int RIR10;

// backward distance
extern int FIR_BW;
extern int Ux_BW;
extern int prevMode;
extern int cntTURN_LEFT_MODE;
extern int cntTURN_RIGHT_MODE;

extern int  Ldist, Rdist, Lunit, Runit;
extern int ref_front;
extern int ref_frontUx;
extern int IRmode;

extern bool calibratedSpeed;

extern int statusTop74HC595;
extern int statusMiddle74HC595;
extern int statusBottom74HC595;

extern bool nightMode;
extern int singleDigitDelay;

extern int playMode;
