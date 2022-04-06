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
  init_ports();                        // Initialize Ports
  init_clocks();                       // Initialize Clock System
  init_conditions();                   // Initialize Variables and Initial Conditions
  Init_LCD();                          // Initialize LCD
//  init_display();
  init_wheels();
  init_timer_B0();
  init_timer_B1();
  init_adc();
  Init_Serial_UCA1(4, 0x5551); //115200
  
  Init_Serial_UCA0(4, 0x5551); //115200
  buad_rate = 115200; 
  strcpy(display_line[0], " Waiting! ");
  strcpy(display_line[2], " 115,200  ");
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
    
    if (send_transmission) {
       strcpy(transmission, recieved_message);
       UCA0IE |= UCTXIE;
       
       // Put message on display
       strcpy(display_line[0], "Transmit! ");
       strcpy(display_line[1], transmission);
       display_changed = true;
       
       // Update states
       send_transmission = false;
       transmission_sent = true;
       recieve_index = 0;
       PROGRAM_COUNT = 0;
    }
    
    if (transmission_sent && (PROGRAM_COUNT >= TIME_2_SECS)) {
       strcpy(display_line[0], " Waiting! ");
       strcpy(display_line[1], "          ");
       display_changed = true;
       transmission_sent = false;
    }
    
    if (message_recieved) {
       // Update Display
       strcpy(display_line[0], "Recieved! ");
       strcpy(display_line[3], recieved_message);
       display_changed = true;

       // Update States
       message_recieved = false;
       transmission_recieved = true;
       PROGRAM_COUNT = 0;
    }
    
    if (transmission_recieved && (PROGRAM_COUNT >= TIME_2_SECS)) {
       strcpy(display_line[0], " Waiting! ");
       strcpy(display_line[3], "          ");
       display_changed = true;
       transmission_recieved = false;
    }

    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
