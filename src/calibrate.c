/// Includes
#include "calibrate.h"
#include "adc.h"
#include "display.h"
#include "switches.h"
#include <string.h>
#include <stdbool.h>

/// Globals
CalibrationState calibration_state = PLACE_ON_W;
int max_left_white  = 76;
int max_right_white = 94;
int max_left_black  = 595;
int max_right_black = 582;
extern void monitor_ir_sensors(void);


/// Functions
// Returns true once calibration is finished
bool calibrate(void) {
  if (NEW_ADC_VALUES) {
    
    // Display current ADC Values
    if (calibration_state != DISPLAY_VALUES) monitor_ir_sensors();
    
    switch (calibration_state) {
      
      // Wait for a switch press so that the user has
      // time to place the car on white
      case PLACE_ON_W:
        
        // Update display
        strcpy(display_line[0], "  Place   ");
        strcpy(display_line[1], " on white ");
        display_changed = true;
        
        if (get_sw1()) {
          // Update dipslay
          strcpy(display_line[0], " Getting  ");
          strcpy(display_line[1], "  white   ");
          display_changed = true;
          
          // Reset calibration values
          max_left_white = 0;
          max_right_white = 0;
          max_left_black = 0;
          max_right_black = 0;
          
          // Advance to next state
          calibration_state++;
        } break;
        
      case GETTING_W:
        if (LEFT_IR_VALUE  > max_left_white)  max_left_white  = LEFT_IR_VALUE;
        if (RIGHT_IR_VALUE > max_right_white) max_right_white = RIGHT_IR_VALUE;
        if (get_sw1()) {
          strcpy(display_line[0], " Getting  ");
          strcpy(display_line[1], "  black   ");
          display_changed = true;
          calibration_state++;
        } break;
        
      case GETTING_B:      
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
          calibration_state++;
        } break;
        
      case DISPLAY_VALUES:
        if (get_sw1()) {
          calibration_state = PLACE_ON_W;
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