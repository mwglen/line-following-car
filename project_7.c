#include "program.h"
#include "project_7.h"
#include "wheels.h"
#include "adc.h"
#include "switches.h"
#include "ports.h"
#include "msp430.h"
#include "timersB0.h"
#include "project_6.h"
#include "calibrate.h"
#include "display.h"
#include <string.h>

int error = 0;
long int prev_error = 0;
long int int_error  = 0;
long int diff_error = 0;
ProjectState PROJECT7_STATE = SETUP;

// LEFT PID
int left_error = 0;
long int left_prev_error = 0;
long int left_int_error  = 0;
long int left_diff_error = 0;

// RIGHT PID
int right_error = 0;
long int right_prev_error = 0;
long int right_int_error  = 0;
long int right_diff_error = 0;

unsigned int project7_timer_ms = 0;
unsigned int project7_timer_secs = 0;
unsigned int PREV_PROGRAM_COUNT = 0;
char time_string[] = " ### secs ";

void project_7(void) {
    
  switch (PROJECT7_STATE) {
    // Turn on IR_EMITTER and callibrate system using SW1
    case SETUP:
      P6OUT |= IR_EMITTER; // On [High]
      PROJECT7_STATE++;
      reset_timers();
      PREV_PROGRAM_COUNT = 0;
      project7_timer_ms = 0;
      project7_timer_secs = 0;
      break;
    
    // Wait for SW1
    case STEP0:
      monitor_ir_sensors();
      strcpy(display_line[0], "Place Car!");
      if (get_sw1()) {
        PROJECT7_STATE++;
        reset_timers();
      } break;
      
    // Intercepting
    case STEP1:
      LEFT_SPEED = WHEEL_PERIOD/6;
      RIGHT_SPEED = WHEEL_PERIOD/6;
      strcpy(display_line[0], "Intercept!");
      if (LEFT_IR_VALUE >= max_white_left
          || RIGHT_IR_VALUE >= max_white_right) {
        strcpy(display_line[0], " Waiting! ");
        stop_wheels();
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Waiting
    case STEP2:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        strcpy(display_line[0], " Turning! ");
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
      
    // Back up a bit
    case STEP3:
      LEFT_SPEED = -WHEEL_PERIOD/6;
      RIGHT_SPEED = -WHEEL_PERIOD/6;
      if (PROGRAM_COUNT >= TIME_100_MS) {
        PROGRAM_COUNT = 0;
        PROJECT7_STATE++;
      } break;
      
    // Rotate until lined up
    case STEP4:
      LEFT_SPEED  =  WHEEL_PERIOD/6;
      RIGHT_SPEED = -WHEEL_PERIOD/6;
      if (LEFT_IR_VALUE > max_white_left) {
        reset_timers();
        stop_wheels();
        PROJECT7_STATE++;
        strcpy(display_line[0], " Waiting! ");
      } break;
      
    // Waiting
    case STEP5:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
        strcpy(display_line[0], " Circling ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "   Time:  ");
        strcpy(display_line[3], " ### secs ");   
        display_changed = true;
      } break;
      
    // Circling
    case STEP6:
      
      // Keep up with Current Time
      if (PROGRAM_COUNT2 >= TIME_1_SECS) {
        project7_timer_secs++;
          
        // Update display to show time
        hex_to_bcd(project7_timer_secs);
        for (int i = 0; i < 3; i++) {
            display_line[3][i+1] = ADC_CHAR[i+1];
            display_changed = true;
        }
        PROGRAM_COUNT2 = 0;
      }
      
      // Drive around circle
      //drive_pid();
      
      // Choose whether to leave
      if (PROGRAM_COUNT >= 10*TIME_1_SECS) {
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
        strcpy(display_line[0], " Exiting! ");
      } break;
    
     
    // Rotate Out
    case STEP7:
      LEFT_SPEED = WHEEL_PERIOD/5;
      RIGHT_SPEED = -WHEEL_PERIOD/5;
      if (PROGRAM_COUNT >= TURN_TIME) {
        PROJECT7_STATE++;
        PROGRAM_COUNT = 0;
      } break;
     
    // Go to center
    case STEP8:
      LEFT_SPEED = WHEEL_PERIOD/5;
      RIGHT_SPEED = WHEEL_PERIOD/5;
      if (PROGRAM_COUNT >= CENTER_TIME) {
        strcpy(display_line[0], " Stopped! ");
        stop_wheels();
        PROJECT7_STATE++;
      } break;
      
    case STEP9:
      if (get_sw1()) {
        PROJECT7_STATE = SETUP;
        CURR_EVENT = MAIN_MENU;
      } break;
  }
}

ProjectState DRIVE_STATE = SETUP;
bool RUN_LEFT = false;
bool RUN_RIGHT = false;
void drive_algorithm(void) {
  switch (DRIVE_STATE) {
    case SETUP:
      // Figure out which wheels to run
      if (LEFT_IR_VALUE < (max_white_left + 300)
          && RIGHT_IR_VALUE > (max_white_right + 150)) {
        RUN_RIGHT = true;
        RUN_LEFT = true;
      } else if (RIGHT_IR_VALUE < (max_white_right + 50)) {
        RUN_LEFT = true;
        RUN_RIGHT = false;
      } else {
        RUN_LEFT = false;
        RUN_RIGHT = true;
      }
      
      // Run wheels
      LEFT_SPEED = RUN_LEFT ? 8000 : 0;
      RIGHT_SPEED = RUN_RIGHT ? 8000 : 0;
      
      // Go to next state
      reset_timers();
      DRIVE_STATE++;
      break;
    
    // Go Forwards
    case STEP0:
      // Run that wheel for 100ms
      if (PROGRAM_COUNT >= TIME_100_MS) {
        reset_timers();
        DRIVE_STATE++;
      } break;
      
    
    // Go Reverse
    case STEP2:
      if (RUN_LEFT && RUN_RIGHT) {
        DRIVE_STATE = SETUP;
        break;
      }
      LEFT_SPEED  = RUN_LEFT  ? -8000 : 0;
      RIGHT_SPEED = RUN_RIGHT ? -8000 : 0;
      
      if (PROGRAM_COUNT >= TIME_100_MS) {
        reset_timers();
        DRIVE_STATE++;
      }
      break;
  
    // Wait for a bit
    case STEP3:
      LEFT_SPEED = 0;
      RIGHT_SPEED = 0;
      if (PROGRAM_COUNT >= TIME_100_MS) DRIVE_STATE = SETUP;
      break;
  }
}

void drive_pid(void) {
  if (NEW_ADC_VALUES) {
    // Update previous error
    prev_error = error;
    
    // Calculate Error
    error = RIGHT_IR_VALUE - LEFT_IR_VALUE;
    
    // Calculate Derivative Value
    diff_error = error - prev_error;
    
    // Calculate Integral Value
    int_error += error;
    
    // Calculate Speeds
    LEFT_SPEED  = base_speed + k_p * error + k_d * diff_error; // + k_i * int_error;
    RIGHT_SPEED = base_speed - k_p * error - k_d * diff_error; // - k_i * int_error;   

    // Ignore overflowed values
    if (LEFT_SPEED < 0) {
      LEFT_SPEED = WHEEL_PERIOD;
      P1OUT &= ~RED_LED; // Turn on Red LED
    } else {
      P1OUT |=  RED_LED; // Turn off Red LED   
    }
    if (RIGHT_SPEED < 0) {
      RIGHT_SPEED = WHEEL_PERIOD;
      P1OUT &= ~RED_LED; // Turn on Red LED
    } else {
      P1OUT |=  RED_LED; // Turn off Red LED          
    }
    
    // Reset NEW_ADC_VALUES
    NEW_ADC_VALUES = false;
  }
}

void drive_2_pid(void) {
  if (NEW_ADC_VALUES) {
    // Update prev error
    left_prev_error = left_error;
    right_prev_error = right_error;
    
    // Determine new error
    left_error = max_black_left - LEFT_IR_VALUE;
    right_error = max_black_right - RIGHT_IR_VALUE;
    if (left_error < 0) left_error = 0;
    if (right_error < 0) right_error = 0;
    
    // Determine Derivative Value
    left_diff_error = left_error - left_prev_error;
    right_diff_error = right_error - right_prev_error;
    
    // Determine Integral Value
    left_int_error += left_error;
    right_int_error += right_error;
    
    // Determine Speeds
    LEFT_SPEED  = base_speed + k_p * left_error + k_d * left_diff_error; // + k_i * left_int_error;
    RIGHT_SPEED = base_speed + k_p * right_error + k_d * right_diff_error; // + k_i * right_int_error;   

    // Ignore overflowed values
    if (LEFT_SPEED < 0) {
      LEFT_SPEED = WHEEL_PERIOD;
      P1OUT &= ~RED_LED; // Turn on Red LED
    } else {
      P1OUT |=  RED_LED; // Turn off Red LED   
    }
    if (RIGHT_SPEED < 0) {
      RIGHT_SPEED = WHEEL_PERIOD;
      P1OUT &= ~RED_LED; // Turn on Red LED
    } else {
      P1OUT |=  RED_LED; // Turn off Red LED          
    }
    
    // Reset NEW_ADC_VALUES
    NEW_ADC_VALUES = false;
  }
}