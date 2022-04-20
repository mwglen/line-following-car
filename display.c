/// Includes
#include "display.h"
#include "iot.h"
#include "adc.h"
#include <string.h>
#include <stdbool.h>

/// Globals
bool display_iot_flag = false;

/// Functions
// Updates display if necessary
unsigned int timer_count = 0;
bool timer_enable = false;
void display_process(void){
  // Update Display
  if (update_display){
    
    // Display and Update Timer
    if (timer_enable) {
      // Increment Timer
      timer_count += 2;
      
      // Display Timer
      hex_to_bcd(timer_count);
      strcpy(display_line[1], "xxx.x secs");
      for (int i = 0; i <= 2; i++)
        display_line[1][i] = ADC_CHAR[i];
      display_line[1][4] = ADC_CHAR[3];
    }
    
    update_display = false;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

// Puts the display into a known state
void init_display(void) {
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "Connecting");
  strcpy(display_line[2], " to WIFI! ");
  strcpy(display_line[3], "          ");
  display_changed = true;
}

