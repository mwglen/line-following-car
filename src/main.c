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
#include "iot.h"
#include "pc.h"
#include <string.h>

/// Global Variables
volatile char slow_input_down;
unsigned int test_value;
char chosen_direction;
char change;
bool transmission_sent = false;
bool transmission_recieved = false;

/// External Functions
void Init_LCD(void);

/// Functions
// main function
void main(void){  
  // set PM5CTL0
  PM5CTL0 &= ~LOCKLPM5;

  // Initialize system
  init_ports();           // Initialize Ports
  init_clocks();          // Initialize Clock System
  init_conditions();      // Initialize Variables and Initial Conditions
  Init_LCD();             // Initialize LCD
  init_display();
  init_wheels();
  init_timer_B0();
  init_adc();
<<<<<<< HEAD:src/main.c

  // Update display for project 8
  strcpy(display_line[0], " Waiting! ");
  strcpy(display_line[2], "BR:       ");
  display_changed = true;

  // Clear Display  
=======
  init_pc(4, 0x5551); //115200 
  init_iot(4, 0x5551); //115200
   
>>>>>>> b117484771621bb0e6ac1b03d80fe6db86e19d5f:main.c
  while(true) {
    
    // Run Program
    run_program();
    
    // Make sure wheels aren't moving forwards and 
    // backwords at the same time
    check_wheels();
    
<<<<<<< HEAD:src/main.c
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
=======
    // Run processes
    adc_process();
    display_process();
    wheels_process();
    iot_process();
    pc_process();
>>>>>>> b117484771621bb0e6ac1b03d80fe6db86e19d5f:main.c
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}