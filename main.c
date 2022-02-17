//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Jim Carlson
//  Jan 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>

// Global Variables
volatile char slow_input_down;
unsigned char display_mode;
unsigned int test_value;
char chosen_direction;
char change;

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------

void start_clock();

void main(void){

  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD

  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "  SYSTEM  ");
  strcpy(display_line[2], " STARTED! ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;

  set_wheels_dir(0, 0);
  // This function never returns
  start_clock();
}

unsigned int old_Time_Sequence;
void start_clock() {
  while(ALWAYS) {
    // Update Clock
    if (Time_Sequence != old_Time_Sequence) {
      old_Time_Sequence = Time_Sequence;
      run();
    }
    drive_car();
    
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
  }
}

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
