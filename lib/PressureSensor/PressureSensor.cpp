#include <Arduino.h>
#include "PressureSensor.h"

HX711::HX711(String name, int dout, int sck, byte gain, bool switch_sign){
    //Safes the HX711 Class with the Pin Config
    this->name = name+" HX711";
    this->DOUT_PIN=dout;
    this->SCK_PIN=sck;
    this->SCALE_FACTOR = 1.0;
    this->OFFSET= 0.0;
    this->OFFSET_RAW=-80000000;
    this->gain= gain;
    this->switch_sign= switch_sign;
    set_gain(gain);
    Serial.println(this->name+" initialized");
}

HX711::~HX711(){
}

//#############################################################
//_________________All User Fuctions___________________

void HX711::setup(){
    //Calls the pinMode 
    pinMode(this->DOUT_PIN, INPUT);
    pinMode(this->SCK_PIN, OUTPUT);
    set_slope_and_yIntercept();
    Serial.print(this->name+" HX711 finished setup\n");
}

void HX711::read() {
  long value = 0;
  uint8_t data[3] = {0};
  uint8_t filler = 0x00;
  
  while (digitalRead(this->DOUT_PIN));  // Warte, bis DOUT auf LOW geht
  int count_Pulse = 0;
  // Pulse the clock pin 24 times to read the data.

  for (int i = 2; i >= 0; i--) {
    int count=0;
    for (int j = 7; j >= 0; j--) { //Schleife von 7 bis 0 (8lang) beginnt mit dem MSB
      digitalWrite(SCK_PIN, HIGH); // Setze die Taktleitung auf HIGH
      bitWrite(data[i], j, digitalRead(this->DOUT_PIN)); // Einfügen des gelesenen Bits
      digitalWrite(SCK_PIN, LOW); // Setze die Taktleitung auf LOW
      count ++;
      count_Pulse ++;
    }
  }

  //Debug Print Pulse counts
  //Serial.print("\nAnz. Pulse in Loop: ");
  //Serial.println(count_Pulse);
  //print_Data_Test(data);

  // Zusätzliche SCK-Impulse entsprechend dem ausgewählten Gain hinzufügen
  for (int i = 0; i < this->GAIN; i++) {
    digitalWrite(this->SCK_PIN, HIGH);
    digitalWrite(this->SCK_PIN, LOW);
    count_Pulse++;
  }

  //Debug Print Pulse counts
  //Serial.print("\nAnz. Pulse all: ");
  //Serial.println(count_Pulse);

// Replicate the most significant bit to pad out a 32-bit signed integer
	if (data[2] & 0x80) {
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = ( static_cast<unsigned long>(filler) << 24
			| static_cast<unsigned long>(data[2]) << 16
			| static_cast<unsigned long>(data[1]) << 8
			| static_cast<unsigned long>(data[0]) );

   if(this->switch_sign){
    value = -value;
   };
   this->rawValue=value;
   set_voltage();
   set_pressure_mmHg();
   set_pressure_psi();
   set_pressure_kpa();
}

//#############################################################
//_________________All Set Fuctions___________________

void HX711::set_SCALE_FACTOR(float new_Scale_Factor){
    this->SCALE_FACTOR=new_Scale_Factor;
};

void HX711::set_OFFSET(float new_Offset){
    this->OFFSET=new_Offset;
};

void HX711::set_gain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			this->GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			this->GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			this->GAIN = 2;
			break;
	};
};

void HX711::set_voltage(){
    //4.63V Versorgung
    this->voltage = (this->rawValue/pow(2,24))*(4.91/this->gain); //for mV *1000
    //this->voltage = (this->rawValue * 4.91) / static_cast<float>(0x7FFFFF);

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

void HX711::set_pressure_mmHg(){
    float pressure_mmHg;
    pressure_mmHg = (this->voltage - 0.5) * (100.0 / 4.0);
    pressure_mmHg = (pressure_mmHg * this->SCALE_FACTOR) + this->OFFSET;
    this->pressure_mmHg = pressure_mmHg;
};

void HX711::set_slope_and_yIntercept(){
    this->slope = (this->U_5_8PSI - this->U_1PSI) / (this->P_5_8PSI - this->P_1PSI);
    this->yIntercept = this->U_1PSI - this->slope * this->P_1PSI;
};

//#############################################################
//_________________All Print Fuctions___________________

void HX711::printTest(){
    //Serial.println(this->name);
    Serial.print("Rohwert: ");

    int num_digits = 1; // Mindestens eine Stelle für den Rohwert

    // Berechnen Sie die Anzahl der Stellen im Rohwert
    long tempValue = this->rawValue;
    if (tempValue < 0) {
        tempValue = -tempValue; // Negative Werte behandeln
    };

    while (tempValue >= 10) {
        tempValue /= 10;
        num_digits++;
    };

    // Füllen Sie die verbleibenden Zeichen mit Leerzeichen auf, um rechtsbündige Ausgabe zu erreichen
    int max_width = 10; // Maximale Breite für die Ausgabe (anpassbar)
    for (int i = num_digits; i < max_width; i++) {
        Serial.print(" ");
    };

    // Ausgabe des Rohwerts
    Serial.print(this->rawValue, DEC);

    //Serial.print(this->rawValue, DEC);
    //Serial.println();
    //Serial.print("\t");
    
    Serial.print(", Spannung: ");
    for ( int i = countDigitsBeforeDecimal(this->voltage) ; i<4;i++){
        Serial.print(" ");
    }
    Serial.print(this->voltage, 4);
    Serial.print(" V");
    
    Serial.print(", Druck (mmHg): ");
    for ( int i = countDigitsBeforeDecimal(this->pressure_mmHg) ; i<4;i++){
        Serial.print(" ");
    }
    Serial.print(this->pressure_mmHg, 2);
    Serial.print(" mmHg");
    /*
    Serial.print(", Druck (PSI): ");
    Serial.print(this->pressure_psi, 2);
    Serial.print(" PSI");

    Serial.print(", Druck (kPa): ");
    Serial.print(this->pressure_kpa, 2);
    Serial.println(" kPa");
    */
};

void HX711::printData(){
    Serial.print(this->pressure_kpa, 2);
    Serial.print(" kPa");
};

int HX711::countDigitsBeforeDecimal(float value){
    int intValue = (int)value; // Ganzzahliger Teil extrahieren
    int digitCount = 1; // Mindestens eine Ziffer

    while (intValue >= 10) {
        intValue /= 10;
        digitCount++;
    }

    return digitCount;
}

void HX711::print_Data_Test(uint8_t data[3]) {
    //Prints all Data in the Data Array
    //Used for debugging
    
    unsigned long value = 0;
    // Ausgabe der Rohdaten im Zweierkomplementformat
    Serial.print("\nData Array (2's Complement): ");
    Serial.print(data[2], BIN);
    Serial.print(" ");
    Serial.print(data[1], BIN);
    Serial.print(" ");
    Serial.println(data[0], BIN);
    value = (static_cast<unsigned long>(data[2]) << 16
			| static_cast<unsigned long>(data[1]) << 8
			| static_cast<unsigned long>(data[0]) );
    Serial.println(value, BIN);
    signed long digit = value;
    Serial.println(digit, DEC);
}

void HX711::print_private_Data(){
  Serial.print("\nSetup Sensor ");
  Serial.println(this->name);
  Serial.print("DOUT-Pin: ");
  Serial.println(this->DOUT_PIN);
  Serial.print("SCK-Pin: ");
  Serial.println(this->SCK_PIN);
  Serial.print("Offset: ");
  Serial.println(this->OFFSET);
  Serial.print("Offset_Raw: ");
  Serial.println(this->OFFSET_RAW);
  Serial.print("Setup Gain: ");
  Serial.println(this->gain);
  Serial.print("Gain Pin Pulse: ");
  Serial.println(this->GAIN);
  Serial.print("Switch Sign: ");
  Serial.println(this->switch_sign);
}