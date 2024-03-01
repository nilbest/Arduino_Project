#include <Arduino.h>
#include "Multi_PressureSensor.h"
#include "PressureSensor.h"

//Adding new Sensor HX711 Collection
multi_HX711::multi_HX711(String name,int Shared_SCK_PIN, int gain){
        this->Name=name;
        // Initialize HX711 pointers to NULL
        for (int i = 0; i < Anz_HX711; i++) {
            HX711s[i] = nullptr;
        };
        this->is_empty = true;
        this->Shared_SCK_PIN=Shared_SCK_PIN;
        this->gain=gain;
        set_gain(this->gain);
};

multi_HX711::~multi_HX711(){

};

// Function to add an HX711 instance using a pointer
void multi_HX711::add_HX711(HX711* hx711Instance) {
        for (int i = 0; i < Anz_HX711; i++) {
            if (HX711s[i] == nullptr) {
                HX711s[i] = hx711Instance;
                HX711s[i]->set_SCK_PIN(this->Shared_SCK_PIN);//Overwriting all added SCK Pins for added instances
                return;  // Exit the function after adding
            };
        };
        Serial.println("\n\n################################################################################################");
        Serial.print("Error:\tHX711 instance could not be added.\n\tPlease correct the Array Size of the multi_HX711 instance. Or creat a new one\n");
        Serial.println("################################################################################################\n\n");
};

//Overwrites the Setup for each added HX711 device
void multi_HX711::setup_overwrite(){
    for (int i = 0; i < Anz_HX711; i++) {
            if (HX711s[i] != nullptr){
                HX711s[i]->setup();
            };
        };
};

void multi_HX711::read(){
    //check if the value and data instances match Anz_HX711
    /*
    Also this function is only programmed for 3 Sensor instances if you need you have to change:
    - Anz_HX711 in the Header file
    - add additional value_S
    - add additional value_S to all_values
    - add additional data_S
    - add additional filler_S
    - add if (data_S[2] & ....
    - add value_S = ( static_cast ...
    */
    long value_S1 = 0;
    long value_S2 = 0;
    long value_S3 = 0;
    long* all_values[] = {&value_S1, &value_S2, &value_S3};//Array for all values
    uint8_t data_S1[3] = {0};
    uint8_t data_S2[3] = {0};
    uint8_t data_S3[3] = {0};
    uint8_t filler_S1 = 0x00;
    uint8_t filler_S2 = 0x00;
    uint8_t filler_S3 = 0x00;

    //Loop through all HX711 and checks if they are ready
    for (int i = 0; i < Anz_HX711; i++) {
        if (HX711s[i] != nullptr) {
            while (digitalRead(HX711s[i]->get_Dout_Pin()) == HIGH) {
                // Waits until the DOUT pin of the current HX711 instance goes HIGH
            };
        };
    };

    //Pulse 24 times the clock Pin to read Data
    for (int i = 2; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) { //Loop from 7 to 0 (8 Times) beginning with MSB
            digitalWrite(this->Shared_SCK_PIN, HIGH); // Setze die Taktleitung auf HIGH
                bitWrite(data_S1[i], j, digitalRead(HX711s[0]->get_Dout_Pin())); // pulls the Bit into the Array
                bitWrite(data_S2[i], j, digitalRead(HX711s[1]->get_Dout_Pin()));
                bitWrite(data_S3[i], j, digitalRead(HX711s[2]->get_Dout_Pin()));
            digitalWrite(this->Shared_SCK_PIN, LOW); // Makes SCK PIN / Taktleitung LOW
        };
    };

    // Makes the last SCK Puls for the selected Gain
    for (int i = 0; i < this->GAIN; i++) {
        digitalWrite(this->Shared_SCK_PIN, HIGH);
        digitalWrite(this->Shared_SCK_PIN, LOW);
    };

    // Replicate the most significant bit to pad out a 32-bit signed integer
	if (data_S1[2] & 0x80) {filler_S1 = 0xFF;} else {filler_S1 = 0x00;};
    if (data_S2[2] & 0x80) {filler_S2 = 0xFF;} else {filler_S2 = 0x00;};
    if (data_S3[2] & 0x80) {filler_S3 = 0xFF;} else {filler_S3 = 0x00;};
    
    // Construct a 32-bit signed integer
	value_S1 = ( static_cast<unsigned long>(filler_S1) << 24
		| static_cast<unsigned long>(data_S1[2]) << 16
		| static_cast<unsigned long>(data_S1[1]) << 8
		| static_cast<unsigned long>(data_S1[0]) );
    value_S2 = ( static_cast<unsigned long>(filler_S2) << 24
		| static_cast<unsigned long>(data_S2[2]) << 16
		| static_cast<unsigned long>(data_S2[1]) << 8
		| static_cast<unsigned long>(data_S2[0]) );
    value_S3 = ( static_cast<unsigned long>(filler_S3) << 24
		| static_cast<unsigned long>(data_S3[2]) << 16
		| static_cast<unsigned long>(data_S3[1]) << 8
		| static_cast<unsigned long>(data_S3[0]) );

    //checks if it has to negate the value
    for(int i = 0; i < Anz_HX711; i++){
        if (HX711s[i] != nullptr) {
            if(HX711s[i]->get_SwitchSign()){
                *all_values[i] = -(*all_values[i]);
            };
        };
    };

    for(int i = 0; i < Anz_HX711; i++){
        if (HX711s[i] != nullptr) {
            HX711s[i]->set_rawValue(*all_values[i]);
            HX711s[i]->set_voltage();
            HX711s[i]->set_pressure_mmHg();
        };
    };
};

void multi_HX711::set_gain(byte gain = 128){
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

void multi_HX711::print_multi_HX711(){
    Serial.print("\n\n");
    Serial.print("\nSetup Sensor '");
    Serial.print(this->Name);
    Serial.println("'");
    Serial.print("All Sensors: ");
    for (int i = 0; i < Anz_HX711; i++) {
        if (i<Anz_HX711-1){
            Serial.print(HX711s[i]->get_Name());
            Serial.print("; ");
        }else{
           Serial.println(HX711s[i]->get_Name()); 
        };
    };
    Serial.print("DOUT PIN: ");
    for (int i = 0; i < Anz_HX711; i++) {
        if (i<Anz_HX711-1){
            Serial.print(HX711s[i]->get_Dout_Pin());
            Serial.print("; ");
        }else{
           Serial.println(HX711s[i]->get_Dout_Pin()); 
        };
    };
    Serial.print("SCK PIN: ");
    for (int i = 0; i < Anz_HX711; i++) {
        if (i<Anz_HX711-1){
            Serial.print(HX711s[i]->get_SCK_PIN());
            Serial.print("; ");
        }else{
           Serial.println(HX711s[i]->get_SCK_PIN()); 
        };
    };
    Serial.print("Is empty? ");
    Serial.println(this->is_empty);
    Serial.print("Shared SCK PIN: ");
    Serial.println(this->Shared_SCK_PIN);
    Serial.print("Set Gain: ");
    Serial.println(this->gain);
    Serial.print("Extra Pulse for Gain: ");
    Serial.println(this->gain);
    Serial.println("\n\n");
};

void multi_HX711::printTest(){
    for (int i = 0; i < Anz_HX711; i++) {
        if (HX711s[i] != nullptr) {
            Serial.println();
            Serial.println(HX711s[i]->get_Name());
            HX711s[i]->printTest();
        };
    };
};
