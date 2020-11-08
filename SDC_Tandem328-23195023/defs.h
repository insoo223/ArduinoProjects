//**************************************************************//
//  Name    : defs.h
//  Author  : Insoo Kim
//  Create  : Oct 8, 2015 
//  Update  : Jan 3, 2019    
//  Notes   : Defines for SDC_Tandem328.ino 
//            Self-driving car on a single perforated board with w/ATmega328P in parallel with Arduino Pro Mini
//            Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//****************************************************************
#define CLOCK_ANIMATION_WHEEL false
//----------------------------------------------
//--- MACRO
//----------------------------------------------

// Get the first byte from any size of data type
// A global variable is always takes 4 bytes in its size.
#define _BYTE(X) (X & 0xFF)

// Converts a bit number into a byte value.
//#define _BV(X) (1 << X)

//----------------------------------------------
//--- Direct Pin Assignments of Arduino Pro Mini 
//----------------------------------------------

//ArduinoProMini digital pin 2 for disc encoder interrupt
#define MR_SPEEDpin     2 
//Legacy assignment for previous version
//#define MR_SPEEDpin   7 //arduino digital pin 7
//#define MR_SPEEDpin   11 //arduino digital pin 11

//ArduinoProMini digital pin 3-4 for CD4021BE input expansion chip
#define CD4021_LATCHpin   3 
#define CD4021_CLOCKpin   4 
#define CD4021_DATApin   	5 

//ArduinoProMini digital pin 6-8 for 74HC595 output expansion chip
// Number cannot be a beginning charater of a variable or macro
#define _74HC595_LATCHpin   6 
#define _74HC595_CLOCKpin   7 
#define _74HC595_DATApin    8

//ArduinoProMini digital pin 9 for EN2 of L293D DC-motor drive chip
//  EN2 is connected to the Right-side geared-DC motor
#define EN2pin 9  

//ArduinoProMini digital pin 10 for EN1 of L293D DC-motor drive chip
//  EN1 is connected to the Left-side geared-DC motor
#define EN1pin 10   

//ArduinoProMini digital pin 11 for DHT11 temperature & huminidy sensor
#define DHT_DATApin      11  
//Legacy assignment for previous version
//#define DHT_DATApin      8  //ver01 arduino digital pin 7
//#define DHT_DATApin      12  //arduino digital pin 12

//ArduinoProMini digital pin 12 for Ultrasound (front) disance measure sensor
#define UxECHOpin     12
//Legacy assignment for previous version
//#define UxECHOpin     13 
#define BUZpin     13

//ArduinoProMini digital pin 13 is reserved. 

//----------------------------------------------
//--- Pin assignments for tripple 74HC595s
//----------------------------------------------

// Bit location of 74HC595 sector 
// A "sector" tells which of three 74HC595 chips holds a corresponding signal: top, middle, or bottom 
#define SECTOR_BIT_LOCATION_74HC595   4  

// bit mask for a corresponding signal ID
#define DATA_BITS_MASK_74HC595   0x07

//--- bottom 74HC595 pin assignments
// signal controlled by "trippleXwrite"function has a sector & pin number
#define IR_FRONT_CLIFF   0x04 //power pin to the front IR of Cliff
#define IR_FRONT_CLIFF_THRESHOLD   80
/*
#define MR_FWDREV   0x01
#define ML_FWDREV   0x02 
#define MR_ONOFF    0x03
#define ML_ONOFF    0x04
#define PGM_START   0x05
#define GREEN_LED   PGM_START
#define PGM_CMD     0x06
#define YELLOW_LED  PGM_CMD
#define PGM_END     0x07
#define RED_LED     PGM_END
*/
//--- middle 74HC595 pin assignments
#define IR_FRONT   0x14 //power pin to the front IR
#define IR_LEFT    0x15 //power pin to the left IR
#define IR_RIGHT   0x16 //power pin to the right IR
#define Ux_TRIG    0x17 //trigger pin to the Ultrasound transducer

//--- bottom 74HC595 pin assignments

// signal controlled by "control74HC595"function only has pin number
/* ver01 - 3 pcs boards
#define DIGIT_K_PIN  4  // Kilo(1000s) digit control pin
#define DIGIT_H_PIN  5  // Hundres(100s) digit control pin
#define DIGIT_T_PIN  6  // Tens(10s) digit control pin
#define DIGIT_U_PIN  7  // Units(1s) digit control pin
*/
#define DIGIT_K_PIN  3  // Kilo(1000s) digit control pin
#define DIGIT_H_PIN  2  // Hundres(100s) digit control pin
#define DIGIT_T_PIN  1  // Tens(10s) digit control pin
#define DIGIT_U_PIN  0  // Units(1s) digit control pin

//--- top 74HC595 pin assignments

/*
#define LATCH4digit7seg   0x20
#define CLOCK4digit7seg   0x21
#define DATA4digit7seg    0x22
*/

#define pi 3.14

#define DP 17

#define IR_Rcv_FRONTpin A0
#define IR_Rcv_LEFTpin A1
#define IR_Rcv_RIGHTpin A2
#define IR_Rcv_CLIFF_FRONTpin A3

//pin assignment of CD4021BE digital input expansion chip
//#define PGM_MODEpinCD4021         2 //ver01  
#define PGM_MODEpinCD4021           0 //ver02  
//#define TactileButtonPinCD4021    1 //ver01
#define TactileButtonPinCD4021      1 //ver02
#define SlideSwitch2ndPinCD4021     2 //ver01 & ver02

//#define UxECHOpinCD4021     0  // Echo Pin
//#define DHT11_DATApinCD4021 0 //used as both input and output, so that connect directly to Arduino digital pin 
//#define MR_SPEEDpinCD4021 0 // too fast, so that connect directly to Arduino digital pin


// bottom 74HC595 pin
#define MLPpin 0
#define MLNpin 1
#define MRNpin 2
#define MRPpin 3


// singlular operation: only the currently stated command will be exerted
#define SOLE_MODE 0
// add operational mode on top of other working state
// the currently stated command will be collaborated with the previouly set command state
#define FRIEND_MODE curState74HC595 

// duration of each operation(move fwd, reverse, left, right etc.)
// for demo purpose
#define OP_DURATION 4000 

#define RECT_ITERATION 300 
#define RECT_TL_ITERATION 300 
#define TURN_CLOCK_ITERATION 1500 
#define TURN_ANT_CLOCK_ITERATION 1500 
#define SHORT_TURN_ITERATION 80 

#define FWD_DURATION 200 // [ms]
#define BWD_DURATION 200 // [ms]
#define STOP_DURATION 500 // [ms]
#define STOP_DURATION_AFTER_TURN 500 // [ms]

#define speedCheckInterval 1000 //ms
#define SPEED_HISTORY_LIMIT 2
#define AVE_HISTORY_LIMIT 8
//num of machine clock counts to get speed
#define quickCheckSpeedIteration 20000 //times
#define NUMSLOTS 40

#define FWD_MODE 0
#define TURN_LEFT_MODE 1000
#define TURN_RIGHT_MODE 2000
#define BWD_MODE 3000

#define MAX_TURN_RIGHT_IN_A_ROW 3
#define MAX_TURN_LEFT_IN_A_ROW 3

#define MAX_ADC 1023

#define LIR_ABS_TURN (MAX_ADC*0.5)
//#define LIR_ABS_TURN (MAX_ADC*0.8)
#define RIR_ABS_TURN (MAX_ADC*0.5)
//#define RIR_ABS_TURN (MAX_ADC*0.8)

#define ADCdiff_30_15cm  50
#define REF_DIST  30 // no 15, only  30 for ver01
//#define REF_DIST  10 // for ver02 (as of Jan 4, 2019)

#define IR_SETmode 0
#define IR_RUNmode 1
#define IR_SIG_DEDUCTION 0

#define COMPENSATION_REF_FIR15 0.7

#define LED_ON_DURATION 0 //[ms]
#define LED_OFF_DURATION 0 //[ms]

#define EEPROM_READ_SAFEMARGIN 10 //[ms]

// location of the starting address of the current IR sensitivity reference value 
// 0 means the 1st address of EEPROM contains the starting address of IR sensitivity measured beforehand
#define EEPROM_IR_SENSITIVITY_ADDR_PTR 0 

#define EEPROM_IR_SENSITIVITY_FRONT_OFFSET   0x00
#define EEPROM_IR_SENSITIVITY_LEFT_OFFSET    0x01
#define EEPROM_IR_SENSITIVITY_RIGHT_OFFSET   0x02

//EEPROM address 0 to 255 is used for IR sensitivity storage
//  others are used for user command storage of UX PRG
#define EEPROM_IR_SENSITIVITY_MAX_ADDR  250

#define IR_ONOFF_SAFEMARGIN 1 //[ms]

#define DISP_4digit7seg_ITERATION_NORMAL     50 // Comfortable viewing time for 4digit 7segment LED
#define DISP_4digit7seg_ITERATION_DATE       50 
#define DISP_4digit7seg_ITERATION_DAY        50 
#define DISP_4digit7seg_ITERATION_TIME       1000
#define DISP_4digit7seg_ITERATION_TEMPHUMID  200 //10000 //1000 // duration to show temp & humidity
#define DISP_4digit7seg_ITERATION_test0to9999   30 // 30 is for human readable speed

#define SET_IR_SENSITIVITY_READY_ON_DURATION 1000 // [ms]
#define SET_IR_SENSITIVITY_READY_OFF_DURATION 2000 // [ms]
#define SET_IR_SENSITIVITY_COMPLETE_DURATION 300 // [ms]
#define SET_IR_SENSITIVITY_NEXT_DURATION 3000 // [ms]

#define PREFIX_DISP_IR_FRONT 1000
#define PREFIX_DISP_IR_LEFT  2000
#define PREFIX_DISP_IR_RIGHT 3000

#define BLANK4CHAR "BLNK"
#define STR_FRNT "FRNT"
#define STR_LEFT "LEFT"
#define STR_RITE "RITE"
#define STR_DONE "DONE"

#define MENU_SYSTEM      0
#define SELF_DRIVING_IR  1
#define SELF_DRIVING_UX  2
#define UX_PRG           3

#define NIGHT_BRIGHTNESS_DELAY 7

//checked using 3.7V LIPO rechargeable battery (Oct 16, 2015)
#define angleNORTH 150
#define angleNORTH_margin 10
#define yNORTH -280
#define yNORTH_margin 110 //-170 -390

#define angleSOUTH 45
#define angleSOUTH_margin 20
#define ySOUTH -225
#define ySOUTH_marginHigh -90
#define ySOUTH_marginLow -400 

#define angleEAST 210
#define angleEAST_margin 30
#define xEAST 150
#define xEAST_margin 100

#define angleWEST 100
#define angleWEST_margin 14
#define xWEST 120  
#define xWEST_margin 140   

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define ROUND_ITERATION 42

#define MAX_ANGLE_ARRAY 10

#define MAX_DISTANCE_UXArray 100
