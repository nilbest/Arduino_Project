#include "MyUtilities.h"
#include "Arduino.h"


int countDigitsBeforeDecimal(float value, int digits /*= 10*/){
    int intValue = (int)value; // Ganzzahliger Teil extrahieren
    int digitCount = 1; // Mindestens eine Ziffer
    while (intValue >= digits) {
        intValue /= digits;
        digitCount++;
    }
    return digitCount;
}

void Serial_print_format_number_spaces(float number, String Start_String /*= ""*/ , int digits /*= 4*/, int decimal_palce /*= 2*/ , String End_String /*= ""*/){
    Serial.print(Start_String);
    for ( int i = countDigitsBeforeDecimal(number) ; i<digits ;i++){
        Serial.print(" ");
    }
    Serial.print(number, decimal_palce);
    Serial.print(End_String);

};