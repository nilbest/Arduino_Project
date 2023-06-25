//Include Libraries
#include <Arduino.h>

//#include"PressureSensor.h"

#include "HX711.h"

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial initialization");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, 128);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Test");
  Serial.println(scale.is_ready());
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.print(reading);
    Serial.print("\tAverage: ");
    long average = (scale.read_average()); 
    Serial.println(average);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
}




