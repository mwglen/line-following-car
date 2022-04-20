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
extern void reset_follow_circle(void);

/// Functions
void project_7(void) {
  
  // Monitor ADC Values
  if (PROJECT7_STATE != SETUP) monitor_ir_sensors();
  
  switch (PROJECT7_STATE) {
    // Turn on IR_EMITTER and callibrate system using SW1
    case SETUP:
      P6OUT |= IR_EMITTER; // On [High]
      if (calibrate()) PROJECT7_STATE++;
      break;
    
    // Wait for SW1
    case STEP0:
      strcpy(display_line[0], "Place Car!");
      if (get_sw1()) {
        fwd_left();
        fwd_right();
        PROJECT7_STATE++;
      } break;
      
    // Intercepting
    case STEP1:
      strcpy(display_line[0], "Intercept!");
      if (LEFT_IR_VALUE > (max_left_white + 100)
          || RIGHT_IR_VALUE > (max_right_white + 100)) {
        stop_wheels();
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Waiting
    case STEP2:
      strcpy(display_line[0], " Waiting! ");
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        LEFT_SPEED  = WHEEL_PERIOD/8;
        RIGHT_SPEED = WHEEL_PERIOD/8;
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;

    case STEP3:
      if (PROGRAM_COUNT >= TIME_100_MS) {
        LEFT_SPEED  =  WHEEL_PERIOD/8;
        RIGHT_SPEED = -WHEEL_PERIOD/4;
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Rotate until lined up
    case STEP4:
      if (LEFT_IR_VALUE > (max_left_white + 100)) {
        stop_wheels();
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Waiting
    case STEP5:
      strcpy(display_line[0], " Waiting! ");
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Circling
    case STEP6:
      // Update Status
      strcpy(display_line[0], " Circling ");
      //if (PROGRAM_COUNT >= TIME_4_SECS) {
      if (false) {
        PROJECT7_STATE++;
        reset_follow_circle();
      } else follow_circle();
      break;
      
    // Exiting
    case STEP7:
      strcpy(display_line[0], " Exiting! ");
      PROJECT7_STATE = 0;
      CURR_EVENT = MAIN_MENU;
      break;
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


#define CIRCLE_SPEED (WHEEL_PERIOD/6)
#define FWD_SPEED (WHEEL_PERIOD/8)
#define BWD_SPEED (WHEEL_PERIOD/8)
#define BASE_SPEED   (WHEEL_PERIOD/4)
#define K_S (0)
#define K_P (25)
#define K_D (10)
int speed = BASE_SPEED;
int prev_error = 0;
ProjectState CIRCLE_STATE;
bool turn_left;
bool turn_right;
void follow_circle_raw(void) {
  if ((LEFT_IR_VALUE > max_left_white) && (RIGHT_IR_VALUE > max_right_white)) {
    LEFT_SPEED  =  FWD_SPEED;
    RIGHT_SPEED =  FWD_SPEED;
  } else if ((LEFT_IR_VALUE > max_left_white) && (RIGHT_IR_VALUE < max_left_white)) {
    LEFT_SPEED  = -BWD_SPEED;
    RIGHT_SPEED =  FWD_SPEED;
  } else if ((LEFT_IR_VALUE < max_left_white) && (RIGHT_IR_VALUE > max_left_white)) {
    LEFT_SPEED  =  FWD_SPEED;
    RIGHT_SPEED = -BWD_SPEED;
  } else {
    LEFT_SPEED  = 0;
    RIGHT_SPEED = 0;
  }
}

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


void reset_follow_circle(void) {
  CIRCLE_STATE = 0;
  turn_left  = false;
  turn_right = false;
}