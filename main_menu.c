#include "main_menu.h"
#include "wheels.h"
#include "display.h"
#include "switches.h"
#include "program.h"
#include <string.h>

Event CURR_SELECTION = MAIN_MENU;

void main_menu(void) {
  stop_wheels();
  strcpy(display_line[0], "MAIN  MENU");
  strcpy(display_line[1], "----------");
  
  switch (CURR_SELECTION) {
    case MAIN_MENU:
      if (get_sw2()) {
        CURR_SELECTION = PROJECT5;
        strcpy(display_line[2], " COMPLETE ");
        strcpy(display_line[3], " PROJECT7 ");
        display_changed = true;  
      } else get_sw1();
      break;
      
    case PROJECT7:
      if (get_sw2()) {
        CURR_SELECTION = PROJECT7;
        strcpy(display_line[2], " COMPLETE ");
        strcpy(display_line[3], " PROJECT7 ");
        display_changed = true; 
      } else if (get_sw1()) {
        CURR_EVENT = PROJECT7;
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "PREFORMING");
        strcpy(display_line[2], " PROJECT7 ");
        strcpy(display_line[3], "          ");
        display_changed = true;  
      } break;
  }
}