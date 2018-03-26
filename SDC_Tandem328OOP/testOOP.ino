/* --------------------------------------------
  testOOP.ino
  Test functions in OOP methodology.
  Created: Oct 24, 2015
  Updated: Oct 29, 2015
  Author: Insoo Kim (insoo@hotmail.com)
-------------------------------------------- */
#include "defs.h"
#include "externs.h"

//-------------------------------------------
void test_Master()
{
  test_menuOOP();
}//test_Master

//-------------------------------------------
void test_menuOOP()
{
  menu.selectOpMode();
}
//-------------------------------------------
void test_UXX()
{
  ux.ProgByUX();
  delay(500);
  
}//test_UXX

//-------------------------------------------
/*-----------------
//-------------------------------------------
void test_ICCX()
{
  X.ctrlAll(0, 0, 0);
  Serial.println(ggCurTopX);
  delay(2000);
  X.ctrlAll(0xff, 0xff, 0xff);
  Serial.println(ggCurTopX);
  delay(2000);
}//test_ICCX

//-------------------------------------------

--------------------------*/

