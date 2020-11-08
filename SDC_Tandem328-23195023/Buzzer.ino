//**************************************************************//
//  Name    : Buzzer.ino
//  Author  : Insoo Kim
//  Create  : Tue Mar 26, 2019 
//  Update  : Thu Mar 28, 2019
//  Notes   : Play melody of "Twinkle twinkle little star
//  Ref:    : 
//      Bibibip http://www.hobbytronics.co.uk/arduino-tutorial7-piezo-beep
//      Melody  https://www.arduino.cc/en/tutorial/melody
//  Why not using "tone"function? : There's conflict with existing library and tone function.
//      http://forum.arduino.cc/index.php?topic=143376.0
//      https://forum.arduino.cc/index.php?topic=342619.0          
//      https://forum.arduino.cc/index.php?topic=120955.0
//      http://forum.arduino.cc/index.php?topic=133553.0
//****************************************************************


void bibibip()  
{ 
  beep(50);
  beep(50);
  beep(50);
  delay(1000);
}// bibibip

void beepWarning()  
{ 
  beep(200); 
}


void beep(unsigned char delayms)
{
  digitalWrite(BUZpin, HIGH);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  digitalWrite(BUZpin, LOW);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}//beep  


//--- Melody ----------------------
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300; //original value: 300

//---------------------------------------------
// added by Insoo (Mar 26, 2019)
void playTone(int tone, int duration) {
  
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(BUZpin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZpin, LOW);
    delayMicroseconds(tone);
  }
}//playTone

//---------------------------------------------
// added by Insoo (Mar 26, 2019)
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  //int tones[] = { 1047, 1175, 1319, 1397, 1568, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}//playNote

//---------------------------------------------
// added by Insoo (Mar 26, 2019)
void loopMelody() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
  }
  delay(3000);
}//loopMelody

//---------------------------------------------
// added by Insoo (Mar 26, 2019)
void testBuzzer() 
{
  digitalWrite(BUZpin, HIGH);
  delay(50);
  digitalWrite(BUZpin, LOW);
  delay(50);
}//testBuzzer
