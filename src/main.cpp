//Include Libraries
#include <Arduino.h>
#include "PressureSensor.h"
#include "Multi_PressureSensor.h"

#include <LiquidCrystal.h>

// Pins am Arduino für HX711
HX711 P1("P1",4,4,32,true);
HX711 P2("P2",5,6,32,true);
HX711 P3("P3",6,8,32,true);
multi_HX711 All_HX711("All_HX711", 3, 32);

long Loop_counter = 0;

//Setup Display
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(115200);
  delay(500);

  //Setup LCD 20x4
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("hello, world!");


  
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


  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}
