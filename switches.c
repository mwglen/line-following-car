/// Includes
#include "msp430.h"
#include "primitives.h"
#include "switches.h"
#include "ports.h"
#include "serial.h"
#include "display.h"
#include "timersB0.h"
#include <string.h>

/// Functions
// Switch 1 Configuration
bool SW1_PRESSED = false;
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void){
  // Handle Interrupt 
  if (P4IFG & SW1) {
    P4IFG &= ~SW1;    // Clear Interrupt Flags
    P2IFG &= ~SW2;    // Clear Interrupt Flags
    P4IE &= ~SW1;     // P4.2 interrupt disabled
    P2IE &= ~SW2;     // P4.2 interrupt disabled
    TB0CCTL2 |= CCIE; // CCR3 enable interrupt
    
    // Tell system that switch was pressed
    SW1_PRESSED = true;

    //------------------------------------
    // HOMEWORK 8
    //------------------------------------
    // Reinitialize UCA0 with buad-rate
    Init_Serial_UCA0(1, 0x4A11); //460,800

    // Transmit Message for Homework 8
    send_transmission = true;

    // Load Baud Rate on Display
    strcpy(display_line[0], "Homework 8");
    strcpy(display_line[1], "Baud Rate:");
    strcpy(display_line[2], " 460,800  ");
    update_display = true;
    
    // Reset Timer
    PROGRAM_COUNT = 0;
  }
}

// Switch 2 Configuration
bool SW2_PRESSED = false;
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
  // Handle Interrupt 
  if (P2IFG & SW2) {
    P4IFG &= ~SW1;    // Clear Interrupt Flags
    P2IFG &= ~SW2;    // Clear Interrupt Flags
    P4IE &= ~SW1;     // P4.2 interrupt disabled
    P2IE &= ~SW2;     // P4.2 interrupt disabled
    TB0CCTL2 |= CCIE; // CCR3 enable interrupt
    
    // Tell program that switch was pressed
    SW2_PRESSED = true;

    //------------------------------------
    // HOMEWORK 8
    //------------------------------------
    // Reinitialize UCA0 with buad-rate
    Init_Serial_UCA0(4, 0x5551); //115200

    // Transmit Message for Homework 8
    send_transmission = true;
    
    // Load Baud Rate on Display
    strcpy(display_line[0], "Homework 8");
    strcpy(display_line[1], "Baud Rate:");
    strcpy(display_line[2], " 115,200  ");
    update_display = true;
    
    // Reset Timer
    PROGRAM_COUNT = 0;
  }
}

bool get_sw1(void) {
  if (SW1_PRESSED) {
    SW1_PRESSED = false;
    return true;
  } else return false;
}

bool get_sw2(void) {
  if (SW2_PRESSED) {
    SW2_PRESSED = false;
    return true;
  } else return false;
}
