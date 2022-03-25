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
      strcpy(display_line[2], " NEXT --->");
      strcpy(display_line[3], "<--- RUN  ");
      display_changed = TRUE;
      if (get_sw2()) CURR_SELECTION = PROJECT5;
      else get_sw1();
      break;
      
    case PROJECT5:
      strcpy(display_line[2], " COMPLETE ");
      strcpy(display_line[3], " PROJECT5 ");
      display_changed = TRUE; 
      if (get_sw2()) CURR_SELECTION = PROJECT6;
      else if (get_sw1()) CURR_EVENT = PROJECT5;
      break;
      
    case PROJECT6:
      strcpy(display_line[2], " COMPLETE ");
      strcpy(display_line[3], " PROJECT6 ");
      display_changed = TRUE; 
      if (get_sw2()) CURR_SELECTION = CALIBRATE;
      else if (get_sw1()) CURR_EVENT = PROJECT6;  
      break;
      
    case CALIBRATE:
      strcpy(display_line[2], "CALIBRATE:");
      strcpy(display_line[3], "IR SENSORS");
      display_changed = TRUE;
      if (get_sw2()) CURR_SELECTION = PROJECT7;
      else if (get_sw1()) CURR_EVENT = CALIBRATE;  
      break;
      
    case PROJECT7:
      strcpy(display_line[2], " COMPLETE ");
      strcpy(display_line[3], " PROJECT7 ");
      display_changed = TRUE; 
      if (get_sw2()) CURR_SELECTION = PROJECT5;
      else if (get_sw1()) CURR_EVENT = PROJECT7;  
      break;
  }
}