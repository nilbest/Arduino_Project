//Include Libraries
#include <Arduino.h>
#include "PressureSensor.h"
#include "Multi_PressureSensor.h"

// Pins am Arduino für HX711
HX711 P1("P1",11,4,32,true);
HX711 P2("P2",12,6,32,true);
HX711 P3("P3",13,8,32,true);

multi_HX711 All_HX711("All_HX711", 10, 32);

long Loop_counter = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("");
  Serial.println("_______Starting Setup for Sensors_______\n");
  delay(500);

  //Setup Pressure Sensors
  P1.setup();
  P1.set_U_m_and_U_b(1.1688 , 0.0989);
  P1.set_P_m_and_P_b(0.1972 , 4.0057);
  //P1.print_private_Data();

  P2.setup();
  P2.set_U_m_and_U_b(1.1546 , 0.2049);
  P2.set_P_m_and_P_b(0.2021 , 5.1914);
  //P2.print_private_Data();

  P3.setup();
  P3.set_U_m_and_U_b(1.1685 , 0.1738);
  P3.set_P_m_and_P_b(0.2002 , 2.9633);
  //P3.print_private_Data();
  
  All_HX711.add_HX711(&P1);
  All_HX711.add_HX711(&P2);
  All_HX711.add_HX711(&P3);
  All_HX711.setup_overwrite();
  All_HX711.print_multi_HX711();
  
  //test_print(&P1);
  Serial.println("\n___________Finished Setup for Pressure Sensors__________");
  Serial.println("\n\n\n");
  delay(500);
  Serial.print("________Start________");
}

void loop() {
  Loop_counter ++;
  Serial.print("\n\nMessung ");
  Serial.print(Loop_counter);
  //Reading Pressure Sensors

  
  All_HX711.read();
  All_HX711.printTest();
  //print_Test(&P1);
  //All_HX711.printTest();
  // Ausgabe auf der seriellen Konsole

  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}
