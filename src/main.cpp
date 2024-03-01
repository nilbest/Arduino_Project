//Include Libraries
#include <Arduino.h>
#include <FlowSensor.h>
#include <LiquidCrystal.h>
//include own code
#include "PressureSensor.h"
#include "Multi_PressureSensor.h"
#include "MyUtilities.h"

// Pins am Arduino für HX711
HX711 P1("P1",11,4,32,true);
HX711 P2("P2",12,6,32,true);
HX711 P3("P3",13,8,32,true);
multi_HX711 All_HX711("All_HX711", 10, 32);

// Setup FlowSensor (pin -> interrupt pin)
FlowSensor Sensor(YFS201, 2); //2=>D2
unsigned long timebefore = 0; // Same type as millis()
void count(){
  Sensor.count();
}

long Loop_counter = 0;

//LCD variable setup
//const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  //Setup Serial
  Serial.begin(115200);
  delay(500);

  //Setup LCD
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
  P2.setup();
  P2.set_U_m_and_U_b(1.1546 , 0.2049);
  P2.set_P_m_and_P_b(0.2021 , 5.1914);
  P3.setup();
  P3.set_U_m_and_U_b(1.1685 , 0.1738);
  P3.set_P_m_and_P_b(0.2002 , 2.9633);
  All_HX711.add_HX711(&P1);
  All_HX711.add_HX711(&P2);
  All_HX711.add_HX711(&P3);
  All_HX711.setup_overwrite();
  All_HX711.print_multi_HX711();
  
  Serial.println("\n___________Finished Setup for Pressure Sensors__________");
  Serial.println("\n\n\n");
  delay(500);

  Serial.print("________Start________");
  //Flow Sensor
  Sensor.begin(count);
}

void loop() {
  //Loop Counter for Debugging
  /*
  Loop_counter ++;
  Serial.print("\n\nMessung ");
  Serial.println(Loop_counter);
  */
  //Reading Pressure Sensors
  All_HX711.read();
  
  if (millis() - timebefore >= 1000){
    Sensor.read();
    //Print Infos to Serial Flow Sensor Test Data
    /*
    Serial_print_format_number_spaces(Sensor.getPulse(), "Puls count:","", 8,0);
    Serial_print_format_number_spaces(Sensor.getFlowRate_m(), "\t, Flow 1: ","L/min");
    Serial_print_format_number_spaces(Sensor.getFlowRate_s(), "\t, Flow 2: ","L/sec");
    Serial_print_format_number_spaces(Sensor.getVolume(),"\t, Volume: ","L");
    */
    timebefore = millis();
  }

  //Serial print All_HX711 Test Data
  //All_HX711.printTest();

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
  lcd.print(Format_number_spaces(Sensor.getFlowRate_m(),"","",3,0,true));
  lcd.setCursor(11,0);
  lcd.print(Format_number_spaces(P1.get_Pressure_mmHg(),"","",3,1,true));
  lcd.setCursor(3,1);
  lcd.print(Format_number_spaces(P2.get_Pressure_mmHg(),"","",3,1,true)); 
  lcd.setCursor(11,1);
  lcd.print(Format_number_spaces(P3.get_Pressure_mmHg(),"","",3,1,true));

  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}


