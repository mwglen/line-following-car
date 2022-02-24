#include "primitives.h"
#include "program.h"
#include "msp430.h"
#include "switches.h"
#include "display.h"
#include "wheels.h"
#include <string.h>

/// Global Variables
// Main Function
unsigned int STARTUP_TIME = 0;
Event CURR_EVENT = STARTUP;
Event CURR_SELECTION = MAIN_MENU;

// Project 5
Project5State PROJECT5_STATE = STEP0;

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
      
    case MAIN_MENU:
      stop_wheels();
      switch (CURR_SELECTION) {
        case MAIN_MENU:
          if (get_sw2()) {
            CURR_SELECTION = PROJECT5;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], " COMPLETE ");
            strcpy(display_line[3], " PROJECT5 ");
            display_changed = TRUE;  
          } else get_sw1();
          break;
          
        case PROJECT5:
          if (get_sw2()) {
            CURR_SELECTION = PROJECT5;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], " COMPLETE ");
            strcpy(display_line[3], " PROJECT5 ");
            display_changed = TRUE; 
          } else if (get_sw1()) {
            CURR_EVENT = PROJECT5;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "PREFORMING");
            strcpy(display_line[2], " PROJECT5 ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;  
          } break;
      } break;
      
  case PROJECT5:
    switch (PROJECT5_STATE) {
      // Travel forwards 1 sec
      case STEP0:
        fwd_left();
        fwd_right();
        break;
        
      // Pause 1 sec
      case STEP1:
        stop_wheels();
        break;
      
      // Travel reverse 2 sec
      case STEP2:
        bwd_left();
        bwd_right();
        break;
        
      // Pause 1 sec
      case STEP3:
        stop_wheels();
        break;
      
      // Travel forwards 1 sec
      case STEP4:
        fwd_left();
        fwd_right();
        break;
        
      // Pause 1 sec
      case STEP5:
        stop_wheels();
        break;
        
      // Spin CW for 3 secs
      case STEP6:
        fwd_left();
        bwd_right();
        break;
        
      // Pause 2 secs
      case STEP7:
        stop_wheels();
        break;
        
      // Spin CCW for 3 secs
      case STEP8:
        bwd_left();
        fwd_right();
        break;
        
      // Pause 2 secs
      case STEP9:
        stop_wheels();
        break;
    }
    break;
  }
}