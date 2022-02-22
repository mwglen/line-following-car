#include "msp430.h"
#include "ports.h"

/// Functions
void Init_LEDs(void){
  P1OUT |=  RED_LED; // Turn off Red LED
  P6OUT &= ~GRN_LED; // Turn on Green LED
}

