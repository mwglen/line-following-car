/// Includes
#include "msp430.h"
#include "ports.h"

/// Functions
void Init_LEDs(void){
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT |= GRN_LED;
}

