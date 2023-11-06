#include <Arduino.h>
#include "PressureSensor.h"

HX711::HX711(String name, int dout, int sck, byte gain, bool switch_sign){
    //Safes the HX711 Class with the Pin Config
    this->name = name+" HX711";
    this->DOUT_PIN=dout;
    this->SCK_PIN=sck;
    this->SCALE_FACTOR = 1.0;
    this->OFFSET= 0.0;
    this->gain= gain;
    this->switch_sign= switch_sign;
    set_gain(gain);
    set_U_m_and_U_b();
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
    Serial.print(this->name+" finished setup\n");
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
   set_pressure_pa();
}

//#############################################################
//_________________All Get Fuctions___________________

int HX711::get_Dout_Pin(){
  return this->DOUT_PIN;
}

bool HX711::get_SwitchSign(){
    return this->switch_sign;
};

String HX711::get_Name(){
    return this->name;
};

int HX711::get_SCK_PIN(){
    return this->SCK_PIN;
};

//#############################################################
//_________________All Set Fuctions___________________

void HX711::set_SCALE_FACTOR(float new_Scale_Factor){
    this->SCALE_FACTOR=new_Scale_Factor;
};

void HX711::set_OFFSET(float new_Offset){
    this->OFFSET=new_Offset;
};

void HX711::set_rawValue(long RawValue){
    this->rawValue=RawValue;
};

void HX711::set_SCK_PIN(int SCK_PIN){
    Serial.print("\n");
    Serial.println(this->name);
    Serial.print("Set SCK_PIN from: ");
    Serial.print(this->SCK_PIN);
    this->SCK_PIN=SCK_PIN;
    Serial.print(" to ");
    Serial.println(this->SCK_PIN);
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
    float voltage;
    voltage = (this->rawValue/pow(2,24))*(4.91/this->gain)*1000; //for mV *1000
    // (Data - b) / m
    if (this->U_b != 0 or this->U_m !=0){
        voltage = (voltage-this->U_b)/this->U_m; //(y-b)/m
    } 
    this->voltage= voltage;
}

void HX711::set_pressure_psi(){
    float pressure_psi;
    //via Point Kalibration
    pressure_psi = this->pressure_mmHg / 51.715;
    this->pressure_psi=pressure_psi;
};

void HX711::set_pressure_pa(){
    float pressure_pa;
    pressure_pa = this->pressure_mmHg * 133.322;
    this->pressure_pa=pressure_pa;
};

void HX711::set_pressure_mmHg(){
    float pressure_mmHg;
    pressure_mmHg =  (this->voltage - this->P_b) / this->P_m; //x = (y-b)/m
    if (pressure_mmHg <= 0){
        pressure_mmHg = 0.00;
        }
    //pressure_mmHg = (this->voltage * 4.3237 ) + (-20.655);
    //pressure_mmHg = (pressure_mmHg * this->SCALE_FACTOR) + this->OFFSET;
    this->pressure_mmHg = pressure_mmHg;
};

void HX711::set_U_m_and_U_b(float slope /*= 0*/ , float yintercept /*= 0*/){
    this->U_m = slope;
    this->U_b = yintercept;
    /*
    Serial.print("\nslope: ");
    Serial.print(this->U_m);
    Serial.print("\t intercept: ");
    Serial.println(this->U_b);
    */
};

void HX711::set_P_m_and_P_b(float slope /*= 0*/ , float yintercept /*= 0*/){
    this->P_m = slope;
    this->P_b = yintercept;
    /*
    Serial.print("\nslope: ");
    Serial.print(this->P_m);
    Serial.print("\t intercept: ");
    Serial.println(this->P_b);
    */
};


//#############################################################
//_________________All Print Fuctions___________________

void HX711::printTest(){
    //Serial.println(this->name);
    Serial.print("\nRohwert: ");

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
    Serial.print(this->voltage, 2);
    Serial.print(" mV");
    
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
    Serial.print(this->pressure_pa, 2);
    Serial.println(" kPa");
    */
};

void HX711::printData(){
    Serial.print(this->pressure_pa, 2);
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
  Serial.print("\nSetup Sensor '");
  Serial.print(this->name);
  Serial.println("'");
  Serial.print("DOUT-Pin: ");
  Serial.println(this->DOUT_PIN);
  Serial.print("SCK-Pin: ");
  Serial.println(this->SCK_PIN);
  Serial.print("Offset: ");
  Serial.println(this->OFFSET);
  Serial.print("U_m: ");
  Serial.println(this->U_m);
  Serial.print("U_b: ");
  Serial.println(this->U_b);
  Serial.print("P_m: ");
  Serial.println(this->P_m);
  Serial.print("P_b: ");
  Serial.println(this->P_b);
  Serial.print("Setup Gain: ");
  Serial.println(this->gain);
  Serial.print("Gain Pin Pulse: ");
  Serial.println(this->GAIN);
  Serial.print("Switch Sign: ");
  Serial.println(this->switch_sign);
}

uint8_t getFillerValue(uint8_t data) {
  // Replicate the most significant bit to pad out a 32-bit signed integer
	/*if (data & 0x80) {
		return (0xFF);
	} else {
		return (0x00);
	};
  */
    return (data & 0x80) ? 0xFF : 0x00;
}

//Bsp. for outsourcing Print Statements
void test_print(HX711* HX711_instance){
    Serial.print("\n\nTesting\n");
    Serial.print("Name: ");
    Serial.println(HX711_instance->get_Name());
};