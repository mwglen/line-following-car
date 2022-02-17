#include  "macros.h"
#include  "project4.h"
#include  "msp430.h"
#include <string.h>

//************************//
//***** RUN FUNCTION *****//
//************************//
void run(void) {
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
      }
      break;
      
    case MAIN_MENU:
      switch (CURR_SELECTION) {
        case MAIN_MENU:
          if (sw2()) {
            CURR_SELECTION = DRAW_CIRCLE;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], "  CIRCLE  ");
            display_changed = TRUE; 
          }
          break;
          
        case DRAW_CIRCLE:
          if (sw2()) {
            CURR_SELECTION = DRAW_TRIANGLE;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], " TRIANGLE ");
            display_changed = TRUE; 
          } else if (sw1()) {
            CURR_EVENT = DRAW_CIRCLE;
          } 
          break;
        case DRAW_TRIANGLE:
          if (sw2()) {
            CURR_SELECTION = DRAW_FIGURE8;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], " FIGURE 8 ");
            display_changed = TRUE; 
          } else if (sw1()) {
            CURR_EVENT = DRAW_TRIANGLE;
          }
          break;
        case DRAW_FIGURE8:
          if (sw2()) {
            CURR_SELECTION = DRAW_CIRCLE;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], "   DRAW   ");
            strcpy(display_line[3], "  CIRCLE  ");
            display_changed = TRUE; 
          } else if (sw1()) {
            CURR_EVENT = DRAW_FIGURE8;
          }
          break;
      }
      break;
      
    case DRAW_CIRCLE:
      if (CIR_TIME >= CIR_ROT_TIME) {
        CIR_TIME = 0;
        CURR_EVENT = MAIN_MENU;
      } else set_wheels(25, 75);
      break;
      
    case DRAW_TRIANGLE:
      switch (TRI_STATE) {
        case DRAW_SIDE1:
          if (TRI_TIME >= TRI_SIDE_TIME) {
            TRI_TIME = 0;
            TRI_STATE++;
          } else set_wheels(50, 50);
          break;
        case ROTATE1:
          if (TRI_TIME >= TRI_ROT_TIME) {
            TRI_TIME = 0;
            TRI_STATE++;
          } else set_wheels(100, 0);
          break;
        case DRAW_SIDE2:
          if (TRI_TIME >= TRI_SIDE_TIME) {
            TRI_TIME = 0;
            TRI_STATE++;
          } else set_wheels(50, 50);
          break;
        case ROTATE2:
          if (TRI_TIME >= TRI_ROT_TIME) { 
            TRI_TIME = 0;
            TRI_STATE++;
          } else set_wheels(100, 0);
          break;
        case DRAW_SIDE3:
          if (TRI_TIME >= TRI_SIDE_TIME) {
            TRI_TIME = 0;
            TRI_STATE = 0;
            CURR_EVENT = MAIN_MENU;
          } else set_wheels(50, 50);
          break;
      }
          
    case DRAW_FIGURE8:
      switch (FIG8_STATE) {
        case CIRCLE1:
          if (FIG8_TIME >= FIG8_ROT_TIME) {
            FIG8_TIME = 0;
            FIG8_STATE++;
          } else set_wheels(25, 75);
          break;
        case CIRCLE2: 
          if (FIG8_TIME >= FIG8_ROT_TIME) {
            FIG8_TIME = 0;
            FIG8_STATE = 0;
            CURR_EVENT = MAIN_MENU;
          } else set_wheels(75, 25);
          break;
      }
  }
}

//*************************//
//***** WHEEL CONTROL *****//
//*************************//
// drive car
void set_wheels(unsigned int left_power, unsigned int right_power) {
  LEFT_WHEEL_POWER = left_power;
  RIGHT_WHEEL_POWER = right_power;  
}

void drive(void){
  // drive left wheel
  if (LEFT_TIME > MAX_POWER) LEFT_TIME = 0;
  if (LEFT_TIME < LEFT_WHEEL_POWER)
    P6OUT |=  L_FORWARD;
  else if (LEFT_TIME <= MAX_POWER-LEFT_WHEEL_POWER)
    P6OUT &= ~L_FORWARD;
  LEFT_TIME++;
  
  // drive right wheel
  if (RIGHT_TIME > MAX_POWER) RIGHT_TIME = 0;
  if (RIGHT_TIME < RIGHT_WHEEL_POWER)
    P6OUT |=  R_FORWARD;
  else if (RIGHT_TIME <= MAX_POWER-RIGHT_WHEEL_POWER) 
    P6OUT &= ~R_FORWARD;
  RIGHT_TIME++;
}