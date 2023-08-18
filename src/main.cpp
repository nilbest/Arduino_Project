//Include Libraries
#include <Arduino.h>
#include "PressureSensor.h"

// Pins am Arduino für HX711
HX711 P1("P1",2,3);
HX711 P2("P2",4,5);
HX711 P3("P3",6,7);



void setup() {
  Serial.begin(115200);

  //Setup Pressure Sensors
  P1.setup();
  P2.setup();
  P3.setup();

}



void loop() {
  //Reading Pressure Sensors
  P1.read();
  P2.read();
  P3.read();

  // Ausgabe auf der seriellen Konsole
  P1.printTest();
  P2.printTest();
  P3.printTest();

  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}
