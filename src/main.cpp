//Include Libraries
#include <Arduino.h>
#include "PressureSensor.h"
#include "Multi_PressureSensor.h"
#include "MyUtilities.h"

#include <LiquidCrystal.h>

// Pins am Arduino für HX711
HX711 P1("P1",11,4,32,true);
HX711 P2("P2",12,6,32,true);
HX711 P3("P3",13,8,32,true);
multi_HX711 All_HX711("All_HX711", 10, 32);
#include <FlowSensor.h>


// pin -> interrupt pin
FlowSensor Sensor(YFS201, 2); //2=>D2
unsigned long timebefore = 0; // Same type as millis()


void count(){
  Sensor.count();
}

long Loop_counter = 0;

//Setup Display
//const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(115200);
  delay(500);

  //Setup LCD 20x4
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Fl:");
  lcd.setCursor(8, 0);
  lcd.print("P1:");
  lcd.setCursor(0, 1);
  lcd.print("P2:");
  lcd.setCursor(8, 1);
  lcd.print("P3:");


  
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

  //Flow Sensor
  Sensor.begin(count);
}

void loop() {
  Loop_counter ++;
  Serial.print("\n\nMessung ");
  Serial.print(Loop_counter);
  //Reading Pressure Sensors

  // put your main code here, to run repeatedly:
    if (millis() - timebefore >= 1000) //1000 = 1sec
  {
    Sensor.read();// calibrate = Number in read() [_numpuls+calibrate]
    Serial.print("Flow rate (L/min) : ");
    Serial.print(Sensor.getFlowRate_m());
    Serial.print("\tTotal Pulse count: ");
    Serial.print(Sensor.getPulse());
    /*Serial.print("\t\tMillis: ");
    Serial.print(millis());
    Serial.print("\tTime Before: ");
    Serial.print(timebefore);*/
    Serial.print("\tFl sec: ");
    Serial.print(Sensor.getFlowRate_s());
    Serial.print("\tLitter: ");
    Serial.println(Sensor.getVolume());
    timebefore = millis();
  }


  
  All_HX711.read();
  All_HX711.printTest();
  //print_Test(&P1);
  //All_HX711.printTest();
  // Ausgabe auf der seriellen Konsole


  //Print Infos to Screen
  lcd.clear();
  lcd.print("Fl:");
  lcd.setCursor(8, 0);
  lcd.print("P1:");
  lcd.setCursor(0, 1);
  lcd.print("P2:");
  lcd.setCursor(8, 1);
  lcd.print("P3:");
  lcd.setCursor(3,0);
  //lcd.print(get.Flow());
  lcd.setCursor(11,0);
  lcd.print(Format_number_spaces(P1.get_Pressure_mmHg(),"","",3,0,true));
  lcd.setCursor(3,1);
  lcd.print(Format_number_spaces(P2.get_Pressure_mmHg(),"","",3,1,true)); //Error with exact number of Digits
  lcd.setCursor(11,1);
  lcd.print(Format_number_spaces(P3.get_Pressure_mmHg(),"","",3,0,true));
  //--


  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}


