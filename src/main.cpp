//Include Libraries
#include <Arduino.h>
#include <FlowSensor.h>


// pin -> interrupt pin
FlowSensor Sensor(YFS201, 2); //2=>D2
unsigned long timebefore = 0; // Same type as millis()


void count(){
  Sensor.count();
}



void setup() {
  Serial.begin(115200);
  Sensor.begin(count);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (millis() - timebefore >= 1000) //1000 = 1sec
  {
    Sensor.read();// calibrate = Number in read() [_numpuls+calibrate]
    Serial.print("Flow (L/min) : ");
    Serial.print(Sensor.getFlowRate_m());
    Serial.print("\tAll Puls count: ");
    Serial.print(Sensor.getPulse());
    /*Serial.print("\t\tMillis: ");
    Serial.print(millis());
    Serial.print("\tTime Before: ");
    Serial.print(timebefore);*/
    Serial.print("\tFl sec: ");
    Serial.print(Sensor.getFlowRate_s());
    Serial.print("\tFlow/min Simple: ");
    //Serial.print(Sensor.getFlowRate_m_simple()); //Not Shure what I wanted there
    Serial.print("\tLitter: ");
    Serial.println(Sensor.getVolume());
    timebefore = millis();
  }
}