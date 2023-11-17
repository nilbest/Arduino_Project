#include "MyUtilities.h"
#include "Arduino.h"
#include "LiquidCrystal.h"


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

void Serial_print_format_number_spaces(float number, String Start_String /*= ""*/ , String End_String /*= ""*/ , int digits /*= 4*/, int decimal_place /*= 2*/ ){
    Serial.print(Start_String);
    for ( int i = countDigitsBeforeDecimal(number) ; i<digits ;i++){
        Serial.print(" ");
    }
    Serial.print(number, decimal_place);
    Serial.print(End_String);

};

//Noch einfügen Runden Und Nachkommastellen anpassen!!!
String Format_number_spaces(float number, String Start_String /*= ""*/ , String End_String /*= ""*/ , int digits /*= 4*/, int decimal_place /*= 2*/ , bool round_num /*=false*/){
    String Value;
    String Spaces;
    Value += Start_String;
    for ( int i = countDigitsBeforeDecimal(number) ; i<digits ;i++){
        Spaces += " ";
    }
    Value += Spaces;
    if (round_num){
        Value += round(number);
    }
    else{
        Value += number;
    }
    Value += End_String;
    return Value;

};