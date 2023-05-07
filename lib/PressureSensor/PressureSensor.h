#ifndef PressureSensor_H  
#define PressureSensor_H

#include "Arduino.h"

const int analogPin = A0;
const int inputResolution = 1023;
int PressureData=0;


void readPressure();
int getPressure();


#endif