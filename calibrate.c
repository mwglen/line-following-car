/// Includes
#include "calibrate.h"
#include "program.h"
#include "adc.h"
#include "ports.h"
#include "msp430.h"
#include "switches.h"
#include "display.h"
#include <string.h>

/// Globals
unsigned int max_white_left  = 300;
unsigned int max_white_right = 300;
unsigned int max_black_left  = 650;
unsigned int max_black_right = 650;
ProjectState CALIBRATE_STATE = SETUP;

/// Defines
#define LIV (LEFT_IR_VALUE)
#define RIV (RIGHT_IR_VALUE)
#define MWL (max_white_left)
#define MWR (max_white_right)
#define MBL (max_black_left)
#define MBR (max_black_right)

/// Functions
void calibrate(void) {
  if (NEW_ADC_VALUES) {
    
    // Calibrate System
    switch (CALIBRATE_STATE) {
      case SETUP:
        // Display State
        strcpy(display_line[0], "  PLACE   ");
        monitor_ir_sensors();
        
        P6OUT |= IR_EMITTER; // On [High]
        MBL = 0; MBR = 0; MWL = 0; MWR = 0;
        if (get_sw1()) CALIBRATE_STATE++;
        break;
        
      // Calibrate White Value
      case STEP0:
        // Display State
        strcpy(display_line[0], "  WHITE   ");
        monitor_ir_sensors();
        
        if (LIV > MWL) MWL = LIV;
        if (RIV > MWR) MWR = RIV;
        if (get_sw1()) {
          CALIBRATE_STATE++;
        } break;
        
      // Calibrate Black Value
      case STEP1:
        // Display State
        strcpy(display_line[0], "  BLACK   ");  
        monitor_ir_sensors();
        
        if (LIV > MBL) MBL = LIV;
        if (RIV > MBR) MBR = RIV;
        if (get_sw1()) {
          CALIBRATE_STATE++;
          P6OUT &= ~IR_EMITTER; // Off [Low]
          display_ir_calibration();
        } break;
        
      // Turn off IR and Display Results
      case STEP2:
        if (get_sw1()) {        
          CALIBRATE_STATE = SETUP;
          CURR_EVENT = MAIN_MENU;
        } break;
    }
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
  NEW_ADC_VALUES = false;
}

void  display_ir_calibration(void) {
  char mwl_string[] = " WL: xxxx ";
  char mwr_string[] = " WR: xxxx ";
  char mbl_string[] = " BL: xxxx ";
  char mbr_string[] = " BR: xxxx ";
    
  // Fill in right IR sensor values
  hex_to_bcd(MBL);
    for (int i = 0; i < 4; i++)
      mbl_string[i+5] = ADC_CHAR[i];
  
  // Fill in left IR sensor values
  hex_to_bcd(MBR);
  for (int i = 0; i < 4; i++)
    mbr_string[i+5] = ADC_CHAR[i];
  
  // Fill in mwl values
  hex_to_bcd(MWL);
  for (int i = 0; i < 4; i++)
    mwl_string[i+5] = ADC_CHAR[i];

  // Fill in mwr values
  hex_to_bcd(MWR);
  for (int i = 0; i < 4; i++)
    mwr_string[i+5] = ADC_CHAR[i];

  // Print to display
  strcpy(display_line[0], mwl_string);
  strcpy(display_line[1], mwr_string);
  strcpy(display_line[2], mbl_string);
  strcpy(display_line[3], mbr_string);
  display_changed = true;
}