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
void display_process(void){
  // Update Display
  if (update_display){
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

