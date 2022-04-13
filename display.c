/// Includes
#include "display.h"
#include <stdbool.h>
#include "iot.h"
#include <string.h>

// Display Flags
bool display_iot_flag = false;

/// Functions
void display_process(void){
  if(update_display){
    update_display = false;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
void init_display(void) {
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "Connecting");
  strcpy(display_line[2], " to WIFI! ");
  strcpy(display_line[3], "          ");
  display_changed = true;
}

