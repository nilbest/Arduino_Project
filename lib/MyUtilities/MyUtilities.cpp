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
        if (decimal_place < 1){

            Value += round_to_int(number);
        }else{
            Value += round_to_float(number, decimal_place);
        }
    }else{
        Value += number;
    }
    Value += End_String;
    return Value;
};

//Rounds Float to wished float decimal_place
//Known Error when number does not have lots of digits
float round_to_float(float number, int round_to_decimal_place){

    int int_num = round(number * pow(10,round_to_decimal_place));

    float rounded_num = int_num / pow(10,round_to_decimal_place);
    
    Serial.print("\nTest Numers: ");
    Serial.print(number);
    Serial.print(" Round To: ");
    Serial.print(round_to_decimal_place);
    Serial.print("  ");
    Serial.print(int_num);
    Serial.print("  ");
    Serial.print(pow(10,round_to_decimal_place));
    Serial.print("  ");
    Serial.println(rounded_num);
    return rounded_num;
};

//Rounds Number to int
int round_to_int(float number){
    return round(number);
};