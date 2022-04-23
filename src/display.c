/// Includes
#include "display.h"
#include "primitives.h"
#include <string.h>

/// Global Variables
//bool DISPLAY_CHANGED = false;
//bool UPDATE_DISPLAY = false;

/// Functions
void display_process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
void init_display(void) {
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "  SYSTEM  ");
  strcpy(display_line[2], " STARTED! ");
  strcpy(display_line[3], "          ");
  display_changed = true;
}