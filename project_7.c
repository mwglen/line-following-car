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
extern void follow_circle(void);
extern void reset_follow_circle(void);

// MOVE THESE
#define TIME_50_MS (250)
#define TIME_150_MS (750)

/// Functions
void project_7(void) {
  
  // Monitor ADC Values
  if (PROJECT7_STATE != SETUP) monitor_ir_sensors();
  
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
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        PROJECT7_STATE++;
        reset_follow_circle();
      } else follow_circle();
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
short int CALIBRATION_STATE = 0;
int max_left_white;
int max_right_white;
int max_left_black;
int max_right_black;
bool calibrate(void) {
  switch (CALIBRATION_STATE) {
    case 0:
      strcpy(display_line[0], "Place on W");
      max_left_white = 0;
      max_right_white = 0;
      max_left_black = 0;
      max_right_black = 0;
      CALIBRATION_STATE++;
      break;
      
    case 1:
      if (LEFT_IR_VALUE  > max_left_white)  max_left_white  = LEFT_IR_VALUE;
      if (RIGHT_IR_VALUE > max_right_white) max_right_white = RIGHT_IR_VALUE;
      if (get_sw1()) {
        strcpy(display_line[0], "Place on B");
        CALIBRATION_STATE++;
      } break;
      
    case 2:      
      if (LEFT_IR_VALUE  > max_left_black)  max_left_black  = LEFT_IR_VALUE;
      if (RIGHT_IR_VALUE > max_right_black) max_right_black = RIGHT_IR_VALUE;
      if (get_sw1()) {
        // Show Results
        strcpy(display_line[0], "MLW: xxxxx");
        strcpy(display_line[1], "MRW: xxxxx");
        strcpy(display_line[2], "MLB: xxxxx");
        strcpy(display_line[3], "MRB: xxxxx");

        // Advance to next state        
        CALIBRATION_STATE++;
      } break;
      
    case 3:
      if (get_sw1()) {
        CALIBRATION_STATE = 0;
        return true;
      } break;
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


#define CIRCLE_SPEED (WHEEL_PERIOD/4)
short int CIRCLE_STATE = 0;
bool turn_left  = false;
bool turn_right = false;
void follow_circle(void) {
  switch (CIRCLE_STATE) {
    case 0:
      // Figure out which wheels to turn
      turn_left = false;
      turn_right = false;
      if ((LEFT_IR_VALUE < (max_left_white + 300)) && (RIGHT_IR_VALUE > (max_right_white + 150))) {
        turn_left = true; 
        turn_right = true;
      } else {
        if (RIGHT_IR_VALUE < max_right_white + 50) turn_left = true;
        else turn_right = true;
      }
      
      // Run that motor
      if (turn_left)  LEFT_SPEED  = CIRCLE_SPEED;
      if (turn_right) RIGHT_SPEED = CIRCLE_SPEED;
      
      // Advance to next state
      CIRCLE_STATE++;
      PROGRAM_COUNT = 0;
      break;
      
    case 1:
      // Wait for 100ms
      if (PROGRAM_COUNT <= TIME_100_MS) {
        
        // Stop Wheels
        LEFT_SPEED = 0;
        RIGHT_SPEED = 0;
        
        // Advance to next state
        CIRCLE_STATE++;
        PROGRAM_COUNT = 0;
      } break;

    case 3:
     // Wait for 150 ms, then proceed
      if (PROGRAM_COUNT >= TIME_50_MS) {
        // If both were forwards go back to step one
        if ((turn_left) && (turn_right)) CIRCLE_STATE = 0;
        
        else {
          // Put the wheel that was moving in reverse
          if (turn_left)  LEFT_SPEED  = -CIRCLE_SPEED;
          if (turn_right) RIGHT_SPEED = -CIRCLE_SPEED;
          
          // Advance to next state
          CIRCLE_STATE++;
          PROGRAM_COUNT = 0;
        }
      } break;
      
    
    case 4:
      // Wait for 8 ms, then proceed
      if (PROGRAM_COUNT >= TIME_50_MS) {
        // Turn all motors off
        LEFT_SPEED = 0; RIGHT_SPEED = 0;
        
        // Advance to next state
        CIRCLE_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    case 5:
      // Wait for 150 ms, then proceed
      if (PROGRAM_COUNT >= TIME_150_MS) {
        
        // Return to First State
        CIRCLE_STATE = 0;
      } break;
  }
}


void reset_follow_circle(void) {
  CIRCLE_STATE = 0;
  turn_left  = false;
  turn_right = false;
}