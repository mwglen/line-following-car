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
#include "timersB0.h"
#include "timersB1.h"
#include "adc.h"

/// Global Variables
volatile char slow_input_down;
unsigned int test_value;
char chosen_direction;
char change;

/// Defines
#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED  (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED  (TB3CCR4)

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
  init_timer_B0();
  init_timer_B1();
  init_adc();
  
  while(true) {
    // Run Program
    program_start();
    
    // Make sure that the wheels are safe to drive and then drive 
    if ((LEFT_FORWARD_SPEED && LEFT_REVERSE_SPEED)
        || (RIGHT_FORWARD_SPEED && RIGHT_REVERSE_SPEED)) {
      stop_wheels();
      P1OUT &= ~RED_LED; // Turn on Red LED
      while (true) {}    // Halt Program
    }
    
    display_process();   // Update Display
    wheels_process();    // Update Wheels
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}