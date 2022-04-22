/// Includes
#include "circle.h"
#include "adc.h"
#include "calibrate.h"
#include "wheels.h"
#include "timersB0.h"
#include "program.h"
#include "display.h"
#include "text.h"
#include <string.h>
#include <stdbool.h>

// Defines
#define FWD_SPEED (WHEEL_PERIOD/4)

/// Globals
ProjectState CIRCLE_STATE = SETUP;
bool turn_left;
bool turn_right;

/// Functions
void follow_circle(void) {
  if (NEW_ADC_VALUES) {
    switch (CIRCLE_STATE) {
      // Determine which wheel to move and start moving it
      case SETUP:
        // If wheels on same color go forwards
        // else turn the wheel that is on white
        turn_right = (RIGHT_IR_VALUE < max_right_white + 100);
        turn_left  = (LEFT_IR_VALUE < max_left_white + 100);
        if (!turn_left && !turn_right) {
           turn_left = true;
           turn_right = true;
        }
    
        // Move wheels according to flags
        LEFT_SPEED  = turn_left  ? FWD_SPEED : 0;
        RIGHT_SPEED = turn_right ? FWD_SPEED : 0;
        
        // Go to next state
        CIRCLE_STATE++;
        TASK_COUNT = 0;
        break;

      // After 50ms, stop wheels
      case STEP0:
        if (TASK_COUNT > TIME_50_MS) {
          stop_wheels();
          CIRCLE_STATE++;
          TASK_COUNT = 0;
        } break;

      // After 50ms, return to first state
      case STEP1:
        if (TASK_COUNT > TIME_50_MS) CIRCLE_STATE = SETUP;
        break;
    }
  }
}


// Exits circle and stop car at least 2 feet away
// Returns true when finished
ProjectState exit_circle_state = SETUP;
bool exit_circle(void) {
  switch (exit_circle_state) {
    case SETUP:
      // Start turning wheels
      center_cpy(display_line[0], "BL Exit");
      LEFT_SPEED  = -WHEEL_PERIOD/4;
      RIGHT_SPEED =  WHEEL_PERIOD/8;
      
      // Advance to next state
      exit_circle_state++;
      PROGRAM_COUNT = 0;
      break;
      
    case STEP0: 
      // After turning normal to circle
      if (PROGRAM_COUNT <= 8*TIME_100_MS) {
        // Start moving away
        LEFT_SPEED  =  WHEEL_PERIOD/8;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        
        // Advance to next state
        PROGRAM_COUNT = 0;
        exit_circle_state++;
      } break;
  
   case STEP1:
    // After moving 2 feet away
    if (PROGRAM_COUNT >= TIME_4_SECS) {
      
      // Stop Wheels, reset state, and return true
      stop_wheels();
      exit_circle_state = SETUP;
      return true;
    } break;
  } return false;
}

// Interupt circle and then return true
ProjectState intercept_state = SETUP;
bool intercept_circle(void) {
  switch (intercept_state) {
    
    // Start Moving Towards Line
    case SETUP:
      // Update display
      center_cpy(display_line[0], "BL START");
      display_changed = true;
      
      // Start moving towards line
      LEFT_SPEED  = WHEEL_PERIOD/4;
      RIGHT_SPEED = WHEEL_PERIOD/8;
      
      // Advance to next state
      intercept_state++;
      break;
      
    // After hitting line, start moving fowards
    case STEP0:
      if (LEFT_IR_VALUE > (max_left_white + 100)
          || RIGHT_IR_VALUE > (max_right_white + 100)) {
        // Update display
        center_cpy(display_line[0], "Intercept");
        display_changed = true;
        
        // Stop wheels
        stop_wheels();
        
        // Advance to next state
        intercept_state++;
        PROGRAM_COUNT = 0;
      } break;
    
    // Wait 5 seconds, then begin turning
    case STEP1:  
      if (PROGRAM_COUNT >= TIME_5_SECS) {
        // Update display
        center_cpy(display_line[0], "BL Turn");
        display_changed = true;
        
        // Start moving forwards to center car on line
        LEFT_SPEED  = WHEEL_PERIOD/8;
        RIGHT_SPEED = WHEEL_PERIOD/8;
        
        // Advance to next state
        intercept_state++;
        PROGRAM_COUNT = 0;
      } break;
      
    // After 100 ms Turn Towards Line
    case STEP2:
      if (PROGRAM_COUNT >= TIME_100_MS) {
        // Turn towards line
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        
        // Advance to next state
        intercept_state++;
      } break;
      
    // Once car is aligned with line, stop wheels
    case STEP3:
      if (RIGHT_IR_VALUE > (max_right_white + 300)) {        
        // Stop Wheels
        stop_wheels();
        
        // Advance to next state
        intercept_state++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Wait 5 Seconds
    case STEP4:
      if (PROGRAM_COUNT >= TIME_5_SECS) {
        // Update Display
        center_cpy(display_line[0], "BL Travel");
        display_changed = true;
        
        // Advance to next state
        intercept_state++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Follow line until car has entered circle
    case STEP5:
      if ((LEFT_IR_VALUE < max_left_white) && (RIGHT_IR_VALUE < max_right_white)) {        
        // Advance to next state
        intercept_state++;
        PROGRAM_COUNT = 0;
      } else follow_circle();
      break;
    
    case STEP6:
      stop_wheels();
      break;
      
      
  } return false;
}