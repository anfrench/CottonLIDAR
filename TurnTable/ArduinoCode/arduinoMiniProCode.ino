#include <Keyboard.h>
#define START 3
#define STEPPER 9
#define STEPPER2 8
#define TOSTART 5
#define SCANNWAIT 1500
#define STEPPERWAIT 7000
#define KEYWAIT 100
#define NUMSCANS 6

void setup() 
{
 pinMode(TOSTART, OUTPUT);
 digitalWrite(TOSTART, HIGH);
 pinMode(START, INPUT);

 
 pinMode(STEPPER, OUTPUT);
 pinMode(STEPPER2, OUTPUT);
 Keyboard.begin();
 
}

void loop() 
{
  if(digitalRead(START))
  {
    printInit();
    scann360();
  }
  while(digitalRead(START)){} // doesn't repeat program
}

void scann360()
{
  for(int i=0; i<500; i++)
  {
    scann(NUMSCANS);
    turnTable();
  }
}

//Presses enter than waits for scann to finish
void scann(int numScans)
{
  for(int i =0; i< numScans; i++)
  {
    Keyboard.press(176);
    delay(KEYWAIT);
    Keyboard.releaseAll();
    delay(SCANNWAIT);
  }
}

//sends a High signal to the stepper via a 5 volt relay
//waits for table to stop moving
void turnTable()
{ 
 digitalWrite(STEPPER, HIGH);
 digitalWrite(STEPPER2, HIGH);
 delay(STEPPERWAIT);
 digitalWrite(STEPPER, LOW);
 digitalWrite(STEPPER2, LOW);
}

//clears the textbox than enters starting peramiters
void printInit()
{
     Keyboard.press(178);
    delay(2000);
    Keyboard.releaseAll();
    Keyboard.print("sMN mLRreqtrigdata 21");
    Keyboard.press(176);
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.press(176);
    delay(100);
    Keyboard.releaseAll();
    Keyboard.press(178);
    delay(2000);
    Keyboard.releaseAll();
    Keyboard.print("sWN LRscnt 1");
}

