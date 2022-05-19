/// Includes
#include "program.h"
#include "display.h"
#include "circle.h"
#include "ports.h"
#include "msp430.h"
#include "calibrate.h"
#include "text.h"
#include "commands.h"
#include "iot.h"
#include "switches.h"
#include "timersB0.h"
#include "text.h"
#include "adc.h"
#include <stdbool.h>
#include <string.h>

/// Global Variables
bool circle_flag = false;
ProgramState program_state = WAIT_FOR_WIFI; 

/// Functions
// start the main program
void run_program(void) {
  // Increment stop count
  if (inc_cmd_recieved) {
    inc_cmd_recieved = false;
    display_line[0][9]++;
    display_changed = true;
  }
  
  // Update displayed timer value
  if (timer_enable && timer_updated) {
    // Reset Flag
    timer_updated = false;
    
    // Display Timer
    hex_to_bcd(timer_count);
    for (int i = 0; i < 3; i++)
      display_line[3][i+6] = ADC_CHAR[i+1];
    display_line[3][9] = 's'; 
    display_changed = true;
  }
  
  switch (program_state) {
    
    // Wait for switch press 
    // It's intended that you press switch after wifi is connected
    case WAIT_FOR_WIFI:
      if (get_sw1()) program_state++;
      break;
    
    // Start calibration
    // Continue when finished
    case CALIBRATE:
      P6OUT |= IR_EMITTER; // On [High]
      if (calibrate()) {
        strcpy(display_line[0], " Waiting  ");
        strcpy(display_line[1], "for input ");
        center_cpy(display_line[2], ip_addr1);
        center_cpy(display_line[3], ip_addr2);
        display_changed = true;
        program_state++;
      } break;
          
    // Wait for any command
    case WAIT_FOR_CMD:
      if (cmd_recieved) {
        // Reset Flag
        cmd_recieved = false;
        
        // Update display
        strcpy(display_line[0], "Arrived 00");
        center_cpy(display_line[1], ip_addr1);
        center_cpy(display_line[2], ip_addr2);
        display_changed = true;
        
        // Enable timer
        timer_enable = true;
        
        // Advance state
        program_state++;
      } break;
    
    // Wait for circle command
    case WAIT_FOR_CIRC:
      if (circle_cmd_recieved) {
        // Reset Flag
        circle_cmd_recieved = false;
        
        // Update display
        strcpy(display_line[3], "Auto.");
        display_changed = true;
        
        // Advance to next state
        program_state++;
      } break;

      
    // Intercept circle and then continue
    case INTERCEPTING:
      if (intercept_circle2()) program_state++;
      break;
      
    // Follow the circle until the exit command was recieved
    case CIRCLING:
      if (PROGRAM_COUNT >= 120*TIME_1_SECS) {
         center_cpy(display_line[0], "BL CIRCLE");
         display_changed = true;
      }
      if (exit_cmd_recieved) program_state++;
      else follow_circle();
      break;
      
    // Exit the circle and stop at least 2 feet away
    // Continue once finished
    case EXITING:
      if (exit_circle()) {
        // Disable Timer
        timer_enable = false;
        
        // Update Display
        center_cpy(display_line[0], " BL STOP  ");
        center_cpy(display_line[1], " That was ");
        center_cpy(display_line[2], "easy!! ;-)");
        strcpy(display_line[3], "          ");
        strcpy(display_line[3], "Time: ");
        display_changed = true;
        
        // Advance to next state
        program_state++;
      } break;
      
    // Do nothing forever
    case FINISHED:
      break;
  }      
}