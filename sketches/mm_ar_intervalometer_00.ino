/*
openDolly intervalometer : test sketch
by Map of Emergence (Stefano Cieri)
based on http://openmoco.org/arduino/arduino-intervalometer-basics
(basically the same sketch as OpenMoCo's, with a few minor tweaks)
*/

#include <MsTimer2.h>        // allowing interrupts

#define SHUTTER  13      // camera shutter trigger

#define INTERVAL 5000    // shot interval time
#define EXPOSURE 2000    // exposure length (B pose is to be used)
#define SHOTS    15      // number of shots to fire

unsigned long lastShot = 0;  // last time our camera fired
unsigned int shotCount = 0;  // number of shots fired
bool exposing = false;       // shutter state; open when exposing = true
bool finished = false;       // sketch executed until the end

void setup() {
  pinMode(SHUTTER, OUTPUT);  // set shutter pin as output
  Serial.begin(9600);        // for debugging; data rate: 9600 bps ; was 19200
}

void loop() {

  if( !exposing && millis() - lastShot > INTERVAL && shotCount < SHOTS) { 
  // if camera is not exposing and timer elapsed, fire camera
  
    MsTimer2::set( EXPOSURE, closeShutter );   // set timer interrupt
    MsTimer2::start();                         // run timer
    
    digitalWrite(SHUTTER, HIGH);  // enable optocoupler
    exposing = true;
    shotCount++;
    
    Serial.println(String("shutter open : ") + shotCount);
  }
  
  if( shotCount==SHOTS && !exposing && !finished){
     Serial.println("last shot reached\n");
     finished = true;
  }
}

void closeShutter() {
  digitalWrite(SHUTTER, LOW);     // disable optocoupler
  MsTimer2::stop();               // disable timer
  // ***   
  lastShot = millis(); 
  exposing = false;
  
  Serial.println("shutter closed\n");
}

/***
interval time is measured from the time an image is completed until the time the next one is triggered
if you want the interval time to be measured between the time the image is triggered and the next image is 
triggered, move this to before bringing the shutter pin high
*/
