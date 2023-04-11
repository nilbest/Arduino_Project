#ifndef timerISR_H  
#define timerISR_H 

//Declarations for the timer
#define ledPin  13

//===============================================================
// ISR's for the timer

// Interrupt Service Routine for compare mode
ISR(TIMER1_COMPA_vect){
  // Preload timer with compare match value
  TCNT1 = timer1_compare_match;

  // Write opposite value to LED
  digitalWrite(ledPin, digitalRead(ledPin)^1);
}


//=============================================================
//Important Functions for Timer

int timer1_compare_match;

void setTimerISR();



#endif