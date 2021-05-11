/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include <Arduino.h>
#include "boards.h"

char* my_dtostrf (double val, signed char width, unsigned char prec, char *sout) {
    char fmt[20];
    sprintf(fmt, "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
}