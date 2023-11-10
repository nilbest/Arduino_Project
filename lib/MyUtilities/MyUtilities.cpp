#include "MyUtilities.h"
#include "Arduino.h"


int countDigitsBeforeDecimal(float value, int digits /*= 10*/){
    int digitCount = 1; // Mindestens eine Ziffer
    if (value < 0) {
        value = -value; // Negative Werte behandeln
    };
    while (value >= 10) {
        value /= 10;
        digitCount++;
    };
    return digitCount; 
}

void Serial_print_format_number_spaces(float number, String Start_String /*= ""*/ , String End_String /*= ""*/ , int digits /*= 4*/, int decimal_palce /*= 2*/ ){
    Serial.print(Start_String);
    for ( int i = countDigitsBeforeDecimal(number) ; i<digits ;i++){
        Serial.print(" ");
    }
    Serial.print(number, decimal_palce);
    Serial.print(End_String);

};