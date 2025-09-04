#include <Arduino.h>
#include "digital.h"
#include "encoder_poll.h"

// PB5 = D13 (LED), PB1 = D9 (Encoder A), PB2 = D10 (Encoder B)
Digital_out ledPin(5);
Digital_in encoderPinA(1);
Digital_in encoderPinB(2);
Encoder_Poll encoder(encoderPinA, encoderPinB, ledPin);

// sample every 200 microseconds
const unsigned long samplePeriodMicroseconds = 200; // samples every 200 microseconds
// tested for 400, 800, 1000 and 2000 microseconds

int main() {
  init();                        
  Serial.begin(115200); //set serial communication

  encoder.begin();               // prepare encoder + LED

  unsigned long nextSampleTime = micros(); //scheduled time for next sample
  unsigned long nextPrintTime  = millis(); //scheduled time for next print

  while (1) {
    unsigned long currentTimeMicro = micros();  //read current time since program start
    //unsigned long is a 32-bit number that only stores positive values.

    // run update every samplePeriodMicroseconds
    // position is updated every samplePeriodMicroseconds
    if ((currentTimeMicro - nextSampleTime) >= 0) {
      encoder.update(); //runs edge detection and position update
      nextSampleTime += samplePeriodMicroseconds; //schedule next sample to be exactly 200millisec later not from now
    }
    //verify position
    //print position every 100 ms
    if (millis() - nextPrintTime >= 100) {
      nextPrintTime += 100;
      Serial.print("Position = ");
      Serial.println(encoder.position());
    }
  }
  return 0;
}
