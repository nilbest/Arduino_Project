//Include Libraries
#include <Arduino.h>
#include "PressureSensor.h"

// Pins am Arduino für HX711
HX711 P1("P1",2,3,32);
/*
HX711 P2("P2",4,5);
HX711 P3("P3",6,7);
*/


void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("###################################################################");
  Serial.println("Starting Setup for Sensors\n");
  delay(500);
  //Setup Pressure Sensors
  P1.setup();
  P1.print_private_Data();
  /*
  delay(500);
  P2.setup();
  delay(500);
  P3.setup();
  delay(500);
  */
  Serial.println("\nFinished Setup for Sensors");
  Serial.println("###################################################################\n\n\n");
  delay(500);
}



void loop() {
  //Reading Pressure Sensors
  Serial.print("\n");
  P1.read();
  /*
  P2.read();
  P3.read();
  */
  //Serial.print("\n");

  // Ausgabe auf der seriellen Konsole
  P1.printTest();
  /*
  P2.printTest();
  P3.printTest();
  */
  delay(2000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}
