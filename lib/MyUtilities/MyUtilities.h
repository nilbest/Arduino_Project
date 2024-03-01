#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include "Arduino.h"

int countDigitsBeforeDecimal(float value, int digits = 10);
void Serial_print_format_number_spaces(float number , String Start_String = "",String End_String = "", int digits = 4, int decimal_place = 2);
String Format_number_spaces(float number , String Start_String = "",String End_String = "", int digits = 4, int decimal_place = 2, bool round_num = false);
float round_to_float(float number, int round_to_decimal_place = 2);
float round_to_float_2(float number);
int round_to_int(float number);

#endif