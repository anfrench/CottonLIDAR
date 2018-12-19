#include <Keyboard.h>

//pins
#define START 3
#define STEPPER 9
#define STEPPER2 8
#define POWERPIN 5
#define PHOTOPIN 16
#define GROUND 15

//Time
#define SCANNWAIT 1500
#define STEPWAIT 500
#define DELAYWAIT 13000
#define KEYWAIT 100
#define PHOTOWAIT 500

//Number vals
#define NUMSCANS 6
#define NUMSTEPS 500

#define DOLIDAR 0
#define DOPHOTO true

/*

  grd -
  grd -
  2 - do lidar
  3 - start
  4 - do photo
  5 - tostart
  6 -
  7 -
  8 - stepper1
  9 - stepper2

*/


void setup()
{
  pinMode(POWERPIN, OUTPUT);
  digitalWrite(POWERPIN, HIGH);
  pinMode(START, INPUT);

  pinMode(STEPPER, OUTPUT);
  pinMode(STEPPER2, OUTPUT);
  Keyboard.begin();

  pinMode(PHOTOPIN, OUTPUT);
  digitalWrite(PHOTOPIN, LOW);

  pinMode(GROUND, OUTPUT);
  digitalWrite(GROUND, LOW);
}

void loop()
{
  if (digitalRead(START))
  {
#if DOLIDAR
    printInit();
#endif

    scann360();
  }
  while (digitalRead(START)) {} // doesn't repeat program
}

void scann360()
{
  for (int i = 0; i < NUMSTEPS; i++)
  {
  #if DOLIDAR
    scann(NUMSCANS);
  #endif
  #if DOPHOTO
    takePhoto();
  #endif
    turnTable();
    delay(DELAYWAIT);
  }
}

//Presses enter than waits for scann to finish
void scann(int numScans)
{
  for (int i = 0; i < numScans; i++)
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
  delay(STEPWAIT);
  digitalWrite(STEPPER, LOW);
  digitalWrite(STEPPER2, LOW);
  delay(STEPWAIT);
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

void takePhoto()
{
  digitalWrite(PHOTOPIN, HIGH);
  delay(PHOTOWAIT);
  digitalWrite(PHOTOPIN, LOW);
}