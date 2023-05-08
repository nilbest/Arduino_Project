#include <stdlib.h>
#include <Arduino.h>
#include "PressureSensor.h"

int SensorValue;
float voltageSensor;

float configVolt;
float configPressure;

/*
void calibrateSensor(P1,P2,V1,V2){

    configVolt=;
    configPressure=;
}
*/


void readSensor(){
    SensorValue=analogRead(analogPin);
};

void getVoltageSensor(){
    voltageSensor = float(SensorValue)*(float(maxVoltIn)/float(inputResolution));
}

float getPressure(){
    return (0.9998*4.88*(pow(10,-3))*voltageSensor)-1.96126;
}