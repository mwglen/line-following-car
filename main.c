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
#include "computer.h"
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
  //init_display();
  init_wheels();
  init_timer_B0();
  init_timer_B1();
  init_adc();
  init_pc(4, 0x5551); //115200 
  init_iot(4, 0x5551); //115200
 
  strcpy(display_line[0], " Waiting! ");
  strcpy(display_line[2], " 115,200  ");
  display_changed = true;
  
  // Clear Display  
  while(true) {
    
    // Run Program
    //program_start();
    check_wheels();
    
    //if (PROGRAM_COUNT >= TIME_2_SECS) {
    //   strcpy(iot_transmission, "Hello World!");
    //   transmit_pc(iot_transmission);
    //   PROGRAM_COUNT = 0;
    //}

    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}