#include <Arduino.h>
#include "PressureSensor.h"

HX711::HX711(String name, int dout, int sck){
    //Safes the HX711 Class with the Pin Config
    this->name = name;
    this->DOUT_PIN=dout;
    this->SCK_PIN=sck;
    this->SCALE_FACTOR = 1.0;
    this->OFFSET= 0.0;
    Serial.println(this->name+" as HX711 initialized");
}

HX711::~HX711(){
}

void HX711::setup(){
    //Calls the pinMode 
    pinMode(this->DOUT_PIN, INPUT);
    pinMode(this->SCK_PIN, OUTPUT);
    set_slope_and_yIntercept();
    Serial.print(this->name+" HX711 finished setup\n");
}

void HX711::set_SCALE_FACTOR(float new_Scale_Factor){
    this->SCALE_FACTOR=new_Scale_Factor;
};

void HX711::set_OFFSET(float new_Offset){
    this->OFFSET=new_Offset;
};


void HX711::read() {
  long value = 0;
  byte data[3] = {0};
  
  while (digitalRead(this->DOUT_PIN));  // Warte, bis DOUT auf LOW geht
  for (int i = 0; i < 3; ++i) {
    for (int j = 7; j >= 0; --j) {
      digitalWrite(SCK_PIN, HIGH);
      bitWrite(data[i], j, digitalRead(this->DOUT_PIN));
      digitalWrite(this->SCK_PIN, LOW);
    }
  }
  
  digitalWrite(this->SCK_PIN, HIGH);
  digitalWrite(this->SCK_PIN, LOW);
  
  // Setze den ADC-Wandler in den Leerzustand
  data[2] ^= 0x80;
  value = (static_cast<long>(data[2]) << 16) | (static_cast<long>(data[1]) << 8) | data[0];
  
   this->rawValue=value;
   set_voltage();
   set_pressure_psi();
   set_pressure_kpa();
   Serial.println(this->name+" finished Reading");
}


void HX711::set_voltage(){
    this->voltage = (this->rawValue * 5.0) / static_cast<float>(0x7FFFFF);
}

void HX711::set_pressure_psi(){
    float pressure_psi;

    
    //via Point Kalibration
    pressure_psi = (this->voltage - 0.5) * (100.0 / 4.0);
    pressure_psi = (pressure_psi * this->SCALE_FACTOR) + this->OFFSET;
    

    //via Linear Interpolation
    //pressure_psi= (voltage - yIntercept) / slope;

    this->pressure_psi=pressure_psi;
};

void HX711::set_pressure_kpa(){
    float pressure_kpa;
    pressure_kpa = this->pressure_psi * 6.89476;
    pressure_kpa = (pressure_kpa * this->SCALE_FACTOR) + this->OFFSET;
    this->pressure_kpa=pressure_kpa;
};

void HX711::set_slope_and_yIntercept(){
    this->slope = (this->U_5_8PSI - this->U_1PSI) / (this->P_5_8PSI - this->P_1PSI);
    this->yIntercept = this->U_1PSI - this->slope * this->P_1PSI;
}


void HX711::printTest(){
    Serial.println(this->name);
    Serial.print("Rohwert: ");
    Serial.print(this->rawValue);
  
    Serial.print(", Spannung: ");
    Serial.print(this->voltage, 4);
    Serial.print(" V");

    Serial.print(", Druck (PSI): ");
    Serial.print(this->pressure_psi, 2);
    Serial.print(" PSI");

    Serial.print(", Druck (kPa): ");
    Serial.print(this->pressure_kpa, 2);
    Serial.println(" kPa");
};

void HX711::printData(){
    Serial.print(this->pressure_kpa, 2);
    Serial.print(" kPa");
};