#include <stdlib.h>
#include <Arduino.h>
#include "timerISR.h"


// Define timer compare match value
//int timer1_compare_match;






void setTimerISR(){
    //START TIMER SETUP
    // Disable all interrupts
    noInterrupts();

  // Initialize Timer1
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_compare_match to the correct compare match register value
  // 256 prescaler & 31246 compare match = 2Hz => 1/2 sec on and off
  timer1_compare_match = 31249;

  // Preload timer with compare match value
  TCNT1 = timer1_compare_match;

  // Set prescaler to 256
  TCCR1B |= (1<<CS12);

  // Enable timer interrupt for compare mode
  TIMSK1 |= (1<<OCIE1A);

  // Enable all interrupts
  interrupts();
}