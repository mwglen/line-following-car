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
#include "follow_line.h"
#include <string.h>
#include <stdlib.h>

/// Defines
#define CIRCLE_TIME (100 * TIME_1_SECS)

/// Globals
int right_error = 0;
int left_error = 0;
ProjectState PROJECT7_STATE = SETUP;

/// Functions
void project_7(void) {
  
  // Monitor ADC Values
  if (PROJECT7_STATE != SETUP) monitor_ir_sensors();
  
  switch (PROJECT7_STATE) {
    // Turn on IR_EMITTER and callibrate system using SW1
    case SETUP:
      P6OUT |= IR_EMITTER; // On [High]
      if (calibrate()) {
        strcpy(display_line[0], "Place Car!");
        PROJECT7_STATE++;
      } break;
    
    // Wait for SW1
    case STEP0:
      if (get_sw1()) {
        strcpy(display_line[0], "Intercept!");
        timer_enable = true;
        fwd_left();
        fwd_right();
        PROJECT7_STATE++;
      } break;
      
    // Intercepting
    case STEP1:
      if (LEFT_IR_VALUE > (max_left_white + 100)
          || RIGHT_IR_VALUE > (max_right_white + 100)) {
        strcpy(display_line[0], " Waiting! ");
        stop_wheels();
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Waiting
    case STEP2:
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        strcpy(display_line[0], " Turning! ");
        LEFT_SPEED  = WHEEL_PERIOD/8;
        RIGHT_SPEED = WHEEL_PERIOD/8;
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;

    case STEP3:
      if (PROGRAM_COUNT >= TIME_100_MS) {
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Rotate until lined up
    case STEP4:
      if (RIGHT_IR_VALUE > (max_right_white + 300)) {
        stop_wheels();
        strcpy(display_line[0], " Waiting! ");
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Waiting
    case STEP5:
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        strcpy(display_line[0], " Circling ");
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Circling
    case STEP6:
      // Update Status
      if (PROGRAM_COUNT >= 2*CIRCLE_TIME) {
        strcpy(display_line[0], " Exiting! ");
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        PROGRAM_COUNT = 0;
        PROJECT7_STATE++;
      } else follow_circle();
      break;
      
    case STEP7:
      if (PROGRAM_COUNT >= 8*TIME_100_MS) {
        LEFT_SPEED  =  WHEEL_PERIOD/8;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        PROGRAM_COUNT = 0;
        PROJECT7_STATE++;
      } break;
      
    // Exiting
    case STEP8:
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        stop_wheels();
        timer_enable = false;
        strcpy(display_line[0], " Stopped! ");
        PROJECT7_STATE++;
      } break;
 
    // Stopped
    case STEP9:
      if (get_sw1()) {
        PROJECT7_STATE = 0;
        CURR_EVENT = MAIN_MENU;
      } break;
  }
}

// Writes IR Sensor data to display and updates display
void monitor_ir_sensors(void) {
    char left_ir_str[]  = " L:  xxxx ";
    char right_ir_str[] = " R:  xxxx ";
    
    // Fill in right IR sensor values
    hex_to_bcd(LEFT_IR_VALUE);
    for (int i = 0; i < 4; i++)
      left_ir_str[i+5] = ADC_CHAR[i];
    
    // Fill in left IR sensor values
    hex_to_bcd(RIGHT_IR_VALUE);
    for (int i = 0; i < 4; i++)
      right_ir_str[i+5] = ADC_CHAR[i];
    
    // Print to display
    strcpy(display_line[2], left_ir_str);
    strcpy(display_line[3], right_ir_str);
    display_changed = true;
}