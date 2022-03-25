/// Includes
#include "program.h"
#include "project_6.h"
#include "calibrate.h"
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
        reset_timers();
        PROJECT6_STATE++;
      } break;
      
    // Wait 1 Second
    case STEP1:
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        reset_timers();
        PROJECT6_STATE++;
      } break;
    
    // Move forward until line is detected
    case STEP2:
      fwd_left();
      fwd_right();
      if (LEFT_IR_VALUE > left_target_value 
          || RIGHT_IR_VALUE > right_target_value) {
        stop_wheels();
        reset_timers();
        PROJECT6_STATE++;
      } break;
      
    // Stop for 4 Seconds
    case STEP3:
      strcpy(display_line[0], "          ");
      strcpy(display_line[1], "BLACK LINE");
      strcpy(display_line[2], " DETECTED ");
      strcpy(display_line[3], "          ");
      if (PROGRAM_COUNT >= TIME_4_SECS) {
        reset_timers();
        PROJECT6_STATE++;
      } break;
      
    // Move Forward until center of car is on line
    case STEP4:
      fwd_left();
      fwd_right();
      if (PROGRAM_COUNT >= TIME_100_MS) {
        reset_timers();
        PROJECT6_STATE++;
      } break;
      
    // Rotate until lined up
    case STEP5:
      fwd_left();
      bwd_right();
      if (LEFT_IR_VALUE > left_target_value) {
        stop_wheels();
        PROJECT6_STATE = STEP0;
        CURR_EVENT = MAIN_MENU;
      } break;
  }
}