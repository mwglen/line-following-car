/// Includes
#include "program.h"
#include "project_7.h"
#include "wheels.h"
#include "adc.h"
#include "switches.h"
#include "ports.h"
#include "msp430.h"
#include "timersB0.h"
#include "display.h"
#include <string.h>

/// Globals
int right_error = 0;
int left_error = 0;
ProjectState PROJECT7_STATE = SETUP;
unsigned int left_target_value  = 300;
unsigned int right_target_value = 300;

/// Functions
void project_7(void) {
  
  // Monitor ADC Values
  monitor_ir_sensors();
  
  switch (PROJECT7_STATE) {
    // Turn on IR_EMITTER and callibrate system using SW1
    case SETUP:
      P6OUT |= IR_EMITTER; // On [High]
      if (calibrate()) {
        PROJECT7_STATE++;
      } break;
    
    // Wait for SW1
    case STEP0:
      strcpy(display_line[0], "Place Car!");
      if (get_sw1()) {
        PROJECT7_STATE++;
      } break;
      
    // Intercepting
    case STEP1:
      fwd_left();
      fwd_right();
      strcpy(display_line[0], "Intercept!");
      if (LEFT_IR_VALUE > left_target_value 
          || RIGHT_IR_VALUE > right_target_value) {
        stop_wheels();
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Waiting
    case STEP2:
      stop_wheels();
      strcpy(display_line[0], " Waiting! ");
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
        
    // Move Forward until center of car is on line
    case STEP3:
      fwd_left();
      fwd_right();
      if (PROGRAM_COUNT >= TIME_100_MS) {
        PROGRAM_COUNT = 0;
        PROJECT7_STATE++;
      } break;
      
    // Wait to avoid running FWD directly after REV
    case STEP4:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_100_MS) {
        PROGRAM_COUNT = 0;
        PROJECT7_STATE++;
      } break;
      
    // Rotate until lined up
    case STEP5:
      fwd_left();
      bwd_right();
      if (LEFT_IR_VALUE > left_target_value) {
        stop_wheels();
        PROJECT7_STATE++;
      } break;
      
    // Waiting
    case STEP6:
      stop_wheels();
      strcpy(display_line[0], " Waiting! ");
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Circling
    case STEP7:
      // Update Status
      strcpy(display_line[0], " Circling ");
      
      // Calculate errors
      if (NEW_ADC_VALUES) {
        left_error  = left_target_value - LEFT_IR_VALUE;
        right_error = right_target_value - RIGHT_IR_VALUE;
      
        // Ignore negative errors
        if (left_error < 0) left_error = 0;
        if (right_error < 0) right_error = 0;
                       
        // Set speeds
        LEFT_SPEED  = base_speed + (k_p_left * left_error);
        RIGHT_SPEED = base_speed + (k_p_right * right_error);
        NEW_ADC_VALUES = false;
      }
      
      //if (PROGRAM_COUNT >= 2*CIRCLE_TIME) {
      //  PROJECT7_STATE++;
      //  PROGRAM_COUNT = 0;
      //}
      break;
      
    // Exiting
    case STEP8:
      strcpy(display_line[0], " Exiting! ");
      PROJECT7_STATE = 0;
      CURR_EVENT = MAIN_MENU;
      break;
  }
}

// Returns true once calibration is finished
bool calibrate(void) {
  strcpy(display_line[0], "Target Cal");
  if (get_sw1()) {
    left_target_value  = LEFT_IR_VALUE  - (int)(0.2*LEFT_IR_VALUE);
    right_target_value = RIGHT_IR_VALUE - (int)(0.2*RIGHT_IR_VALUE);
    return true;
  }
  return false;
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