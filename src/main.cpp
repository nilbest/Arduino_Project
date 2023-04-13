//Include Libraries
#include <Arduino.h>

//Include Own Code-Snippets
#include "timerISR.h"

//#include "FlowSensor.h"





// Define LED Pin
#define ledPin  13

// Define timer compare match value
int timer1_compare_match;

// Interrupt Service Routine for compare mode
ISR(TIMER1_COMPA_vect){
  // Preload timer with compare match value
  TCNT1 = timer1_compare_match;

  // Write opposite value to LED
  digitalWrite(ledPin, digitalRead(ledPin)^1);
}




void setup() {
  // put your setup code here, to run once:

  // Set LED as outupt
  pinMode(ledPin, OUTPUT);	
  

 void setTimerISR();
}

void loop() {
  // put your main code here, to run repeatedly:
}