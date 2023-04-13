#ifndef WaterFlowSensor_H  
#define WaterFlowSensor_H

#include "Arduino.h"

//Constant Data Flow Sensor
int const FlowAmounts[]={0,120,240,360,480,600,720}; //in L/H
float const FlowFrequences[]={0,16,32.5,49.3,65.5,82,90.2};
int const FlowSensorPin=2;

// Variables for Flow Sensor
float FlowData;
bool FlowSendorEnabled=true;
bool FlowSensorError=false;


// Functions for Flow Sensor

void calculateFlowData();
float getFlowData();
float compareFlowData();

#endif