#include <stdlib.h>
#include <Arduino.h>
#include "PressureSensor.h"

void readPressure(){
    PressureData=analogRead(analogPin);
};

int getPressure(){
    return PressureData;
}