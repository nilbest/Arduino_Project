#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include "Arduino.h"



int countDigitsBeforeDecimal(float value, int digits = 10);
void Serial_print_format_number_spaces(float number , String Start_String = "",String End_String = "", int digits = 4, int decimal_palce = 2);


#endif