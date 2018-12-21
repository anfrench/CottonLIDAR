/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability




#define pinA 2
#define VCC1 32
#define GND1 33
//#define NC1 34
//#define NC2 35
#define GND2 36
#define VCC2 37
#define pinB 3
#define VCC3 39
#define pinI 21 

Encoder myEnc(pinA, pinB);
//   avoid using pins with LEDs attached

void setup() 
{
  pinMode(VCC1, OUTPUT);
  pinMode(VCC2, OUTPUT);
  pinMode(VCC3, OUTPUT);
  
  pinMode(GND1, OUTPUT);
  pinMode(GND2, OUTPUT);

  digitalWrite(VCC1, HIGH);
  digitalWrite(VCC2, HIGH);
  digitalWrite(VCC3, HIGH);

  digitalWrite(GND1, LOW);
  digitalWrite(GND1, LOW);
  
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");

  attachInterrupt(digitalPinToInterrupt(pinI), setZero, CHANGE);
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  
}

void setZero()
{
  myEnc.write(0);
}

