#ifndef PressureSensor_H  
#define PressureSensor_H

#include "Arduino.h"



uint8_t getFillerValue(uint8_t data);

/*
Achtung Sensoren müssen noch kalibriert werden!!!

Druck (Einheit) = (Rohwert - Rohwert_min) * (Druck_max - Druck_min) / (Rohwert_max - Rohwert_min) + Druck_min
Rohwert ist der vom HX711 ausgelesene Rohwert.
Rohwert_min ist der Rohwert bei deinem Mindestdruck.
Rohwert_max ist der Rohwert bei deinem Höchstdruck.
Druck_min ist der minimale Druckwert im gewünschten Einheitensystem (PSI oder kPa).
Druck_max ist der maximale Druckwert im gewünschten Einheitensystem (PSI oder kPa).



*/
class HX711
{
    private:
        String name;        // Name of Sensor
        int DOUT_PIN;       // DOUT pin am Arduino
        int SCK_PIN;        // SCK pin am Arduino
        float SCALE_FACTOR; // Skalierungsfaktor zur Umrechnung in Druckwerte
        float OFFSET;       // Offset-Korrektur (falls nötig)
        int gain;
        byte GAIN;
        bool switch_sign;

        long rawValue;
        float voltage;      
        float pressure_psi;
        float pressure_pa;
        float pressure_mmHg;

        //lineare Interpolation
        float U_m; //m
        float U_b; //b
        float P_m;
        float P_b;


    public:
        
        HX711(String name, int dout, int sck, byte gain = 128, bool switch_sign=false);
        virtual ~HX711();

        void setup();
        void read();


        int get_Dout_Pin();
        int get_SCK_PIN();
        bool get_SwitchSign();
        String get_Name();
        

        void set_SCK_PIN(int SCK_PIN);
        void set_rawValue(long RawValue);
        void set_SCALE_FACTOR(float new_Scale_Factor);
        void set_OFFSET(float new_Offset);
        void set_gain(byte gain = 128);


        //Interne Funktionen
        void set_U_m_and_U_b(float slope = 0 , float yintercept = 0); // slope = m ; intercept = b
        void set_P_m_and_P_b(float slope = 0 , float yintercept = 0); // slope = m ; intercept = b
        void set_voltage();
        void set_pressure_psi();
        void set_pressure_pa();
        void set_pressure_mmHg();
        int countDigitsBeforeDecimal(float value);

        //To get smaler RAM Usage make them using pointers and do not add them to each instance!!!
        void printTest();
        void printData();
        void print_private_Data();
        void print_Data_Test(uint8_t data[3]);
};

//_________________All HX711 related Functions with high Memory Usage_________________
void test_print(HX711* HX711_instance);


#endif