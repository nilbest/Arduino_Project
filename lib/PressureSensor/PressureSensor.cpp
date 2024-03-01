#include <Arduino.h>
#include "PressureSensor.h"
#include "MyUtilities.h"

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

  // Zusätzliche SCK-Impulse entsprechend dem ausgewählten Gain hinzufügen
  for (int i = 0; i < this->GAIN; i++) {
    digitalWrite(this->SCK_PIN, HIGH);
    digitalWrite(this->SCK_PIN, LOW);
    count_Pulse++;
  }

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
}

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

long HX711::get_rawValue(){
    return this->rawValue;
};

int HX711::get_SCK_PIN(){
    return this->SCK_PIN;
};

float HX711::get_Voltage(bool Volt /*= false*/){
    if (Volt){
        return this->voltage/1000;
    };
    return this->voltage;
};

float HX711::get_Pressure_mmHg(){
    return this->pressure_mmHg;
};

float HX711::get_Pressure_psi(){
    return this->pressure_mmHg / 51.715;
};

float HX711::get_Pressure_pa(){
    return this->pressure_mmHg * 133.322;
};

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

void HX711::set_gain(byte gain /*= 128*/) {
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
    //Saves mV in Memory
    float voltage;
    voltage = (this->rawValue/pow(2,24))*(4.91/this->gain)*1000; //for mV *1000
    // (Data - b) / m
    if (this->U_b != 0 or this->U_m !=0){
        voltage = (voltage-this->U_b)/this->U_m; //(y-b)/m
    } 
    this->voltage= voltage;
}

void HX711::set_pressure_mmHg(){
    float pressure_mmHg;
    pressure_mmHg =  (this->voltage - this->P_b) / this->P_m; //x = (y-b)/m

    if (pressure_mmHg <= 0){
        pressure_mmHg = 0.00;
        }
    
    this->pressure_mmHg = pressure_mmHg;
};

void HX711::set_U_m_and_U_b(float slope /*= 0*/ , float yintercept /*= 0*/){
    this->U_m = slope;
    this->U_b = yintercept;
};

void HX711::set_P_m_and_P_b(float slope /*= 1*/ , float yintercept /*= 0*/){
    this->P_m = slope;
    this->P_b = yintercept;
};

//_________________All Print Fuctions___________________

//Prints Basic Values for calibration
void HX711::printTest(){
    Serial_print_format_number_spaces(this->rawValue, "Rohwert: ","", 8,0);
    Serial_print_format_number_spaces(this->voltage, "\t, Spannung: ","mV");
    Serial_print_format_number_spaces(this->pressure_mmHg, "\t, Druck: ","mmHg");
};

//Prints all Data in the Data Array
//Used for debugging
void HX711::print_Data_Test(uint8_t data[3]) {
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

//Output all private data
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
    return (data & 0x80) ? 0xFF : 0x00;
}


//_________________All HX711 related Functions with high Memory Usage_________________
//Bsp. for outsourcing Print Statements (Macht aber keinen Unterschied, Compiler optimiert schon)
void test_print(HX711* HX711_instance){
    Serial.print("\n\nTesting\n");
    Serial.print("Name: ");
    Serial.println(HX711_instance->get_Name());
};