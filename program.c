#include "program.h"
#include "display.h"
#include <string.h>

// Events
#include "main_menu.h"
#include "project_5.h"
#include "project_6.h"
#include "project_7.h"

/// Global Variables
// Main Function
unsigned int STARTUP_TIME = 0;
Event CURR_EVENT = STARTUP;

/// Functions
// start the main program
void program_start(void) {
  switch (CURR_EVENT) {
    case STARTUP:
      STARTUP_TIME++;
      if (STARTUP_TIME > 400) {
        CURR_EVENT = MAIN_MENU;
        strcpy(display_line[0], "MAIN  MENU");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "<- RUN    ");
        strcpy(display_line[3], "   NEXT ->");
        display_changed = TRUE;
        STARTUP_TIME = 0;
      }
      break;
      
    case MAIN_MENU: main_menu(); break;
    case PROJECT5: project_5(); break;
    case PROJECT6: project_6(); break;
    case PROJECT7: project_7(); break;
  }
}