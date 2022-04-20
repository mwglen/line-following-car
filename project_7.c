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
#include <stdlib.h>

/// Globals
int right_error = 0;
int left_error = 0;
ProjectState PROJECT7_STATE = SETUP;
extern void follow_circle(void);
#define CIRCLE_TIME (60 * TIME_1_SECS)

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
      if (RIGHT_IR_VALUE > (max_right_white + 100)) {
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
      if (PROGRAM_COUNT >= CIRCLE_TIME) {
        strcpy(display_line[0], " Exiting! ");
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        PROGRAM_COUNT = 0;
        PROJECT7_STATE++;
      } else follow_circle();
      break;
      
    case STEP7:
      if (PROGRAM_COUNT >= 4*TIME_150_MS) {
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

// Returns true once calibration is finished
short int CALIBRATION_STATE = 0;
int max_left_white;
int max_right_white;
int max_left_black;
int max_right_black;
bool calibrate(void) {
  if (NEW_ADC_VALUES) {
    switch (CALIBRATION_STATE) {
      case 0:
        strcpy(display_line[0], "Place on W");
        display_changed = true;
        if (get_sw1()) {
          strcpy(display_line[0], "Getting  W");
          display_changed = true;
          max_left_white = 0;
          max_right_white = 0;
          max_left_black = 0;
          max_right_black = 0;
          CALIBRATION_STATE++;
        } break;
        
      case 1:
        if (LEFT_IR_VALUE  > max_left_white)  max_left_white  = LEFT_IR_VALUE;
        if (RIGHT_IR_VALUE > max_right_white) max_right_white = RIGHT_IR_VALUE;
        if (get_sw1()) {
          strcpy(display_line[0], "Place on B");
          display_changed = true;
          CALIBRATION_STATE++;
        } break;
        
      case 2:      
        if (LEFT_IR_VALUE  > max_left_black)  max_left_black  = LEFT_IR_VALUE;
        if (RIGHT_IR_VALUE > max_right_black) max_right_black = RIGHT_IR_VALUE;
        if (get_sw1()) {
          // Show Results
          strcpy(display_line[0], "MLW:  xxxx");
          strcpy(display_line[1], "MRW:  xxxx");
          strcpy(display_line[2], "MLB:  xxxx");
          strcpy(display_line[3], "MRB:  xxxx");
          
          // Fill in MLW sensor values
          hex_to_bcd(max_left_white);
          for (int i = 0; i < 4; i++)
            display_line[0][i+6] = ADC_CHAR[i];
          
          // Fill in MRW sensor values
          hex_to_bcd(max_right_white);
          for (int i = 0; i < 4; i++)
            display_line[1][i+6] = ADC_CHAR[i];

          // Fill in MLB sensor values
          hex_to_bcd(max_left_black);
          for (int i = 0; i < 4; i++)
            display_line[2][i+6] = ADC_CHAR[i];
          
          // Fill in MRB sensor values
          hex_to_bcd(max_right_black);
          for (int i = 0; i < 4; i++)
            display_line[3][i+6] = ADC_CHAR[i];
          
          display_changed = true;
          
          // Advance to next state        
          CALIBRATION_STATE++;
        } break;
        
      case 3:
        if (get_sw1()) {
          CALIBRATION_STATE = 0;
          return true;
        } break;
    }
  } return false;
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


#define FWD_SPEED (WHEEL_PERIOD/8)
bool turn_left;
bool turn_right;
void follow_circle(void) {
  if (NEW_ADC_VALUES) {
    turn_left  = false;
    turn_right = false;
    if ((LEFT_IR_VALUE < (max_left_white + 300)) && (RIGHT_IR_VALUE < (max_right_white + 150))) {
      turn_left = true;
      turn_right = true;
    } else {
      if (RIGHT_IR_VALUE < (max_right_white + 50)) {
        turn_left = true;
      } else turn_right = true;
    }
    
    LEFT_SPEED  = turn_left  ? FWD_SPEED : 0;
    RIGHT_SPEED = turn_right ? FWD_SPEED : 0;
  }
}