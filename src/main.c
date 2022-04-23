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
#include "serial.h"
#include <string.h>

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
  init_timer_B0();
  init_timer_B1();
  init_adc();

  // Update display for project 8
  strcpy(display_line[0], " Waiting! ");
  strcpy(display_line[2], "BR:       ");
  display_changed = true;

  // Clear Display  
  while(true) {
    
    // Run Program
    //program_start();
    
    // Make sure that the wheels are safe to drive and then drive 
    if ((LEFT_FORWARD_SPEED && LEFT_REVERSE_SPEED)
        || (RIGHT_FORWARD_SPEED && RIGHT_REVERSE_SPEED)) {
      stop_wheels();
      P1OUT &= ~RED_LED; // Turn on Red LED
      while (true) {}    // Halt Program
    }
    
    // Send transmission for Homework 8 
    if (send_transmission && (PROGRAM_COUNT >= TIME_2_SECS)) {

       // Put a string into transmission global
       strcpy(transmission, "ABCDEFGHIJ");

       // Put other info on display
       strcpy(display_line[0], " Transmit ");
       strcpy(display_line[1], transmission);

       // Enable transmission interrupt
       UCA0IE |= UCTXIE;
       
       recieve_index = 0;
       
       send_transmission = false;
    }

    // Display recieved messages for Homework 8
    if (message_recieved) {

       // Say that you've handled the message
       message_recieved = false;

       // Put other info on display
       strcpy(display_line[0], " Recieved ");
       
       // Put message on display
       for (int i = 0; i < 10; i++)
          display_line[3][i] = recieved_message[i];

       // Update the display
       display_changed = true;
    }
    
    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
