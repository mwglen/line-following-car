//******************************************************************************
//
//  Description: This file contains function definitions related to LEDs
//
//  Matthew Glen
//  Feb 2022
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (5.40.1)
//******************************************************************************

#include "functions.h"
#include "macros.h"
#include "msp430.h"

void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}

