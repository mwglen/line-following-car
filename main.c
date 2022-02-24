/// Includes
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

/// External Functions
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
  Init_LCD();                          // Initialize LCD
  init_display();
  init_wheels();

<<<<<<< Updated upstream
  while(ALWAYS) {
    // Update Clock
    if (Time_Sequence != OLD_TIME_SEQUENCE) {
      OLD_TIME_SEQUENCE = Time_Sequence;
      program_start();
    }
    drive_car();
    
    switches_process();                // Check for switch state change
=======
  while(true) {
    // Run Program
    fwd_left();
    fwd_right();
    
    // Make sure that the wheels are safe to drive and then drive 
    if ((LEFT_FORWARD_SPEED && LEFT_REVERSE_SPEED)
        || (RIGHT_FORWARD_SPEED && RIGHT_REVERSE_SPEED)) {
      stop_wheels();
      P1OUT &= ~RED_LED;   // Turn on Red LED
      while (true) {}      // Halt Program
    }

>>>>>>> Stashed changes
    display_process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
  }
}