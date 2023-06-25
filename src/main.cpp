//Include Libraries
#include <Arduino.h>

//#include"PressureSensor.h"

#include "HX711-multi.h"

#define CLK 7      // clock pin to the load cell amp
#define DOUT1 6    // data pin to the first
#define DOUT2 A2    // data pin to the second
#define DOUT3 A3    // data pin to the third


byte DOUTS[3] = {DOUT1, DOUT2, DOUT3};

#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte) //

long int results[CHANNEL_COUNT];

//HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);
/*
void sendRawData() {
  scales.read(results);
  for (int i=0; i<scales.get_count(); ++i) {;
    Serial.print( -results[i]);  
    Serial.print( (i!=scales.get_count()-1)?"\t":"\n");
  }  
  delay(10);
}
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial initialization");
  pinMode(CLK,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Test");
  //sendRawData();

}




