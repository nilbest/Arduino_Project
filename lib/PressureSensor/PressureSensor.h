#ifndef PressureSensor_H  
#define PressureSensor_H

#include "Arduino.h"



const int analogPin = A0;
const int inputResolution = 1023;
const int maxVoltIn = 5;

//Used if not made in Classes
extern int SensorValue;
extern float voltageSensor;


void readSensor();
void getVoltageSensor();

float getPressure();








#endif