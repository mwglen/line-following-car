/// Includes
#include "primitives.h"
#include "msp430.h"
#include "ports.h"
#include "clocks.h"
#include "display.h"
#include "program.h"
#include "switches.h"
#include "wheels.h"
#include "init.h"

/// Global Variables
volatile char slow_input_down;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int OLD_TIME_SEQUENCE = 0;
extern volatile unsigned int Time_Sequence;

/// External Functions
void Init_Timers(void);
void Init_LCD(void);

/// Functions
// main function
void main(void){
  
  // set PM5CTL0
  PM5CTL0 &= ~LOCKLPM5;

  // Initialize system
  init_ports();                        // Initialize Ports
  init_clocks();                       // Initialize Clock System
  init_conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  init_display();

  while(true) {
    // Update Clock
    if (Time_Sequence != OLD_TIME_SEQUENCE) {
      OLD_TIME_SEQUENCE = Time_Sequence;
      program_start();
    }
    
    // Make sure that the wheels are safe to drive and then drive 
    if (((P6IN & L_FORWARD) && (P6IN & L_REVERSE)) 
        || ((P6IN & R_FORWARD) && (P6IN & R_REVERSE))) {
      P6OUT &= ~L_FORWARD;
      P6OUT &= ~L_REVERSE;
      P6OUT &= ~R_FORWARD;
      P6OUT &= ~R_REVERSE;
      P1OUT &= ~RED_LED;   // Turn on Red LED
      while (true) {}    // Halt Program
    } else drive_car();

    switches_process();                // Check for switch state change
    display_process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
  }
}