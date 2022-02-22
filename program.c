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

// Circle
unsigned int CIR_TIME = 0;

// Triangle
TriangleState TRI_STATE = 0;
unsigned int TRI_TIME = 0;
unsigned int TRI_NUM = 0;

// Figure 8
Figure8State FIG8_STATE = 0;
unsigned int FIG8_TIME = 0;
unsigned int FIG8_NUM = 0;

const unsigned int CIR_ROT_TIME = 3800;
const unsigned int TRI_SIDE_TIME = 300;
const unsigned int TRI_ROT_TIME45 = 150;
const unsigned int FIG8_ROT_TIME = 800;

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
      set_wheels_dir(0, 0);
      switch (CURR_SELECTION) {
        case MAIN_MENU:
          if (get_sw2()) {
            CURR_SELECTION = DRAW_CIRCLE;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], "  CIRCLE  ");
            display_changed = TRUE;  
          } else get_sw1();
          break;
          
        case DRAW_CIRCLE:
          if (get_sw2()) {
            CURR_SELECTION = DRAW_TRIANGLE;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], " TRIANGLE ");
            display_changed = TRUE; 
          } else if (get_sw1()) {
            CURR_EVENT = DRAW_CIRCLE;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "  DRAWING ");
            strcpy(display_line[2], "  CIRCLE  ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;  
          } 
          break;
          
        case DRAW_TRIANGLE:
          if (get_sw2()) {
            CURR_SELECTION = DRAW_FIGURE8;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], " FIGURE 8 ");
            display_changed = TRUE; 
          } else if (get_sw1()) {
            CURR_EVENT = DRAW_TRIANGLE;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "  DRAWING ");
            strcpy(display_line[2], " Triangle ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE; 
          }
          break;
          
        case DRAW_FIGURE8:
          if (get_sw2()) {
            CURR_SELECTION = DRAW_CIRCLE;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], "  CIRCLE  ");
            display_changed = TRUE;  
          } else if (get_sw1()) {
            CURR_EVENT = DRAW_FIGURE8;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "  DRAWING ");
            strcpy(display_line[2], " Figure 8 ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE; 
          }
          break;
      }
      break;
      
    case DRAW_CIRCLE:
      if (CIR_TIME >= CIR_ROT_TIME) {
        CIR_TIME = 0;
        CURR_EVENT = MAIN_MENU;
      } else {
        CIR_TIME++;
        set_wheels_dir(25, 100);
      }
      break;
      
    case DRAW_TRIANGLE:
      
      switch (TRI_STATE) {
        case DRAW_SIDE1:
          if (TRI_TIME >= TRI_SIDE_TIME) {
            TRI_TIME = 0;
            TRI_STATE++;
          } else {
            TRI_TIME++;
            set_wheels_dir(50, 50);
          }
          break;
          
        case ROTATE1:
          if (TRI_TIME >= TRI_ROT_TIME45*2) {
            TRI_TIME = 0;
            TRI_STATE++;
          } else {
            TRI_TIME++;
            set_wheels_dir(100, 0);
          }
          break;
          
        case DRAW_SIDE2:
          if (TRI_TIME >= TRI_SIDE_TIME) {
            TRI_TIME = 0;
            TRI_STATE++;
          } else {
            TRI_TIME++;
            set_wheels_dir(50, 50);
          }
          break;
          
        case ROTATE2:
          if (TRI_TIME >= TRI_ROT_TIME45*2) { 
            TRI_TIME = 0;
            TRI_STATE++;
          } else {
            TRI_TIME++;
            set_wheels_dir(100, 0);
          }
          break;
          
        case DRAW_SIDE3:
          if (TRI_TIME >= TRI_SIDE_TIME * 1.414) {
            TRI_TIME = 0;
            TRI_NUM++;
            if (TRI_NUM == 2) {
              TRI_NUM = 0;
              CURR_EVENT = MAIN_MENU;
              TRI_STATE = 0;
            } else {
              TRI_STATE++;
            }
          } else {
            TRI_TIME++;
            set_wheels_dir(50, 50);
          }
          break;
          
        case ROTATE3:
          if (TRI_TIME >= TRI_ROT_TIME45 * 3) { 
            TRI_TIME = 0;
            TRI_STATE = 0;
          } else {
            TRI_TIME++;
            set_wheels_dir(100, 0);
          }
          break;
          
      }
      break;
          
    case DRAW_FIGURE8:
      switch (FIG8_STATE) {
        case CIRCLE1:
          if (FIG8_TIME >= FIG8_ROT_TIME) {
            FIG8_TIME = 0;
            FIG8_STATE++;
          } else {
            FIG8_TIME++;
            set_wheels_dir(10, 100);
          }
          break;
        case CIRCLE2: 
          if (FIG8_TIME >= FIG8_ROT_TIME) {
            FIG8_NUM++;
            FIG8_TIME = 0;
            FIG8_STATE = 0;
            if (FIG8_NUM == 2) {
              FIG8_NUM = 0;
              CURR_EVENT = MAIN_MENU;
            }
          } else {
            FIG8_TIME++;
            set_wheels_dir(100, 10);
          }
          break;
      }
      break;
  }
}