#include "program.h"
#include "project_6.h"
#include "wheels.h"
#include "switches.h"
#include "ports.h"
#include "msp430.h"
#include "timersB0.h"
#include "adc.h"
#include "display.h"
#include <string.h>

/// Globals
ProjectState PROJECT6_STATE = STEP0;
char program_status[] = " Program6 ";
char ADC_CHAR[4];
int LEFT_IR_VALUE;
int RIGHT_IR_VALUE;
unsigned int left_target_value  = 300;
unsigned int right_target_value = 300;

void project_6(void) {
 
  // Monitor ADC Values
  monitor_ir_sensors();
   
  // Monitor program state Values
  strcpy(display_line[0], program_status);
  
  switch (PROJECT6_STATE) {
    // Wait for SW1
    case STEP0:
      if (get_sw1()) {
        // Turn on Emitter
        P6OUT |= IR_EMITTER; // On [High]
        PROGRAM_COUNT = 0;
        PROJECT6_STATE++;
      } break;
      
    // Wait 1 Second
    case STEP1:
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT6_STATE++;
      } break;
    
    // Move forward until line is detected
    case STEP2:
      fwd_left();
      fwd_right();
      if (LEFT_IR_VALUE > left_target_value 
          || RIGHT_IR_VALUE > right_target_value) {
        stop_wheels();
        PROGRAM_COUNT = 0;
        PROJECT6_STATE++;
      } break;
      
    // Stop for 4 Seconds
    case STEP3:
      strcpy(display_line[0], "          ");
      strcpy(display_line[1], "BLACK LINE");
      strcpy(display_line[2], " DETECTED ");
      strcpy(display_line[3], "          ");
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT6_STATE++;
      } break;
      
    // Move Forward until center of car is on line
    case STEP4:
      fwd_left();
      fwd_right();
      if (PROGRAM_COUNT >= TIME_100_MS) {
        PROGRAM_COUNT = 0;
        PROJECT6_STATE++;
      } break;
      
    // Wait to avoid running FWD directly after REV
    case STEP5:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_100_MS) {
        PROGRAM_COUNT = 0;
        PROJECT6_STATE++;
      } break;
      
    // Rotate until lined up
    case STEP6:
      fwd_left();
      bwd_right();
      if (LEFT_IR_VALUE > left_target_value) {
        stop_wheels();
        PROJECT6_STATE = STEP0;
        CURR_EVENT = MAIN_MENU;
      } break;
  }
}

// Writes IR Sensor data to display and updates display
void monitor_ir_sensors(void) {
    char left_ir_str[]  = " L:  xxxx ";
    char right_ir_str[] = " R:  xxxx ";
    const char ir_off[] = "  IR OFF  ";
    const char ir_on[]  = "EMITTER ON";
    
    // Fill in right IR sensor values
    hex_to_bcd(LEFT_IR_VALUE);
    for (int i = 0; i < 4; i++)
      left_ir_str[i+5] = ADC_CHAR[i];
    
    // Fill in left IR sensor values
    hex_to_bcd(RIGHT_IR_VALUE);
    for (int i = 0; i < 4; i++)
      right_ir_str[i+5] = ADC_CHAR[i];
    
    // Print to display
    strcpy(display_line[1], (P6OUT & IR_EMITTER) ? ir_on : ir_off);
    strcpy(display_line[2], left_ir_str);
    strcpy(display_line[3], right_ir_str);
    display_changed = true;
}