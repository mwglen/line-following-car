#include  "macros.h"
#include  "project4.h"
#include  "msp430.h"
#include <string.h>

//*************************//
//***** MAIN FUNCTION *****//
//*************************//
// State Declaration
Project4State PROJECT4_STATE = 0;

// State Machine
void project_04() {
  switch (PROJECT4_STATE) {
    case (STOP_CAR1):
      stop_car();
      strcpy(display_line[0], "          ");
      strcpy(display_line[1], " Awaiting ");
      strcpy(display_line[2], "  Input 1 ");
      strcpy(display_line[3], "          ");
      display_changed = TRUE;
      PROJECT4_STATE++;
      break;

  case (WAIT_FOR_PRESS1):
    if (wait_for_button() == COMPLETED) {
      /* strcpy(display_line[0], "          "); */
      /* strcpy(display_line[1], " Drawing  "); */
      /* strcpy(display_line[2], " Circle   "); */
      /* strcpy(display_line[3], "          "); */
      /* display_changed = TRUE; */
      PROJECT4_STATE++;
    }
    break;

  case (DRAW_CIRCLE):
    set_wheels(FWD, FWD);
    /* if (draw_circle() == COMPLETED) { */
    /*   strcpy(display_line[0], "          "); */
    /*   strcpy(display_line[1], " Awaiting "); */
    /*   strcpy(display_line[2], "  Input 2 "); */
    /*   strcpy(display_line[3], "          "); */
    /*   display_changed = TRUE; */
    /*   PROJECT4_STATE++; */
    /* } */
    break;

  case (WAIT_FOR_PRESS2):
    if (wait_for_button() == COMPLETED) {
      strcpy(display_line[0], "          ");
      strcpy(display_line[1], " Drawing  ");
      strcpy(display_line[2], " Triangle ");
      strcpy(display_line[3], "          ");
      display_changed = TRUE;
      PROJECT4_STATE++;
    }
    break;

  case (DRAW_TRIANGLE):
    if (draw_triangle() == COMPLETED) {
      strcpy(display_line[0], "          ");
      strcpy(display_line[1], " Awaiting ");
      strcpy(display_line[2], "  Input 3 ");
      strcpy(display_line[3], "          ");
      display_changed = TRUE;
      PROJECT4_STATE++;
    }
    break;

  case (WAIT_FOR_PRESS3):
    if (wait_for_button() == COMPLETED) {
      strcpy(display_line[0], "          ");
      strcpy(display_line[1], " Drawing  ");
      strcpy(display_line[2], " Figure8  ");
      strcpy(display_line[3], "          ");
      display_changed = TRUE;
      PROJECT4_STATE++;
    }
    break;

  case (DRAW_FIGURE8):
    if (draw_figure8() == COMPLETED)
      PROJECT4_STATE = 0;
    break;
  }
}

//*************************//
//***** WHEEL CONTROL *****//
//*************************//
unsigned int WHEEL_TIME = 0;

// Stop Car
void stop_car(void) {
  set_wheels(STP, STP);
}

// Set wheels
void set_wheels(
  WheelDirection left_dir,
  WheelDirection right_dir
){
  switch (left_dir) {
    case STP:
      P6OUT &= ~L_FORWARD;
      //P6OUT &= ~L_REVERSE;
      break;

    case FWD:
      if (WHEEL_TIME > 20) {
        P6OUT |=  L_FORWARD;
        //P6OUT &= L_REVERSE;
      } else {
        P6OUT &= ~L_FORWARD;
        //P6OUT &= ~L_REVERSE;
      }
      if (WHEEL_TIME > 40) {
        WHEEL_TIME = 0;
      }
      break;

    case BWD:
      P6OUT &= ~L_FORWARD;
      //P6OUT |=  L_REVERSE;
      break;
  }
  switch (right_dir) {
    case STP:
      P6OUT &= ~R_FORWARD;
      //P6OUT &= ~R_REVERSE;
    break;
      
    case FWD:
      P6OUT |=  R_FORWARD;
      //P6OUT &= ~R_REVERSE;
      break;
              
    case BWD:
      P6OUT &= ~R_FORWARD;
      //P6OUT |=  R_REVERSE;
      break;
  }
  WHEEL_TIME++;
}

//*************************//
//**** BUTTON FUNCTION ****//
//*************************//
Progress wait_for_button() {
  if (! (P2IN & SW2)) return COMPLETED;
  else return IN_PROGRESS;
}

//*************************//
//**** CIRCLE FUNCTION ****//
//*************************//
// Global Variables
CircleState CIRCLE_STATE = 0;
int CIRCLE_TIME = 0;
int SIDE_COUNT = 0;

// Parameters
const int NUM_SIDES = 10;
const int CIR_FWD_TIME  = 10;
const int CIR_ROT_TIME  = 10;
Progress draw_circle(void) {
  CIRCLE_TIME++;
  switch (CIRCLE_STATE) {
    case (MOVE_FORWARD):
      set_wheels(FWD, FWD);
      if (CIRCLE_TIME >= CIR_FWD_TIME) {
        CIRCLE_TIME = 0;
        CIRCLE_STATE++;
      }
      return IN_PROGRESS;

  case (ROTATE):
    set_wheels(FWD, STP);
    if (CIRCLE_TIME >= CIR_ROT_TIME) {
      CIRCLE_TIME = 0;
      CIRCLE_STATE = (SIDE_COUNT < NUM_SIDES) ? MOVE_FORWARD: STOP;
      SIDE_COUNT++;
  }
  return IN_PROGRESS;

  case (STOP):
    stop_car();
    CIRCLE_STATE = 0;
    CIRCLE_TIME = 0;
    SIDE_COUNT = 0;
    return COMPLETED;

  default:
    return COMPLETED;
  }
}

//*************************//
//**** FIGURE8 FUNCTION ***//
//*************************//
Figure8_State FIGURE8_STATE = 0;
Progress draw_figure8() {
  switch (FIGURE8_STATE) {
    case (DRAW_CIRCLE1):
      if (draw_circle() == COMPLETED)
        FIGURE8_STATE = DRAW_CIRCLE2;
        return IN_PROGRESS;
        
    case (DRAW_CIRCLE2):
      return draw_circle();
        
    default:
      return COMPLETED;
  }
}

//*************************//
//*** TRIANGLE FUNCTION ***//
//*************************//
Triangle_State TRIANGLE_STATE = 0;
int TRIANGLE_TIME = 0;

// Parameters
const int TRI_FWD_TIME  = 10;
const int TRI_ROT_TIME  = 10;

Progress draw_triangle() {
  switch (TRIANGLE_STATE) {
    case (DRAW_SIDE1):
      set_wheels(FWD, FWD);
      if (TRIANGLE_TIME >= TRI_FWD_TIME) {
        TRIANGLE_TIME = 0;
        TRIANGLE_STATE++;
      }
      return IN_PROGRESS;

    case (ROTATE1):
      set_wheels(FWD, STP);
      if (TRIANGLE_TIME >= TRI_ROT_TIME) {
        TRIANGLE_TIME = 0;
        TRIANGLE_STATE++;
      }
      return IN_PROGRESS;

    case (DRAW_SIDE2):
      set_wheels(FWD, FWD);
      if (TRIANGLE_TIME >= TRI_FWD_TIME) {
        TRIANGLE_TIME = 0;
        TRIANGLE_STATE++;
      }
      return IN_PROGRESS;

    case (ROTATE2):
      set_wheels(FWD, STP);
      if (TRIANGLE_TIME >= TRI_ROT_TIME) {
        TRIANGLE_TIME = 0;
        TRIANGLE_STATE++;
      }
      return IN_PROGRESS;

  case (DRAW_SIDE3):
    set_wheels(FWD, FWD);
    if (TRIANGLE_TIME >= TRI_FWD_TIME) {
      TRIANGLE_TIME = 0;
      TRIANGLE_STATE++;
    }
    return IN_PROGRESS;

  case (ROTATE3): 
    set_wheels(FWD, STP);
    if (TRIANGLE_TIME >= TRI_ROT_TIME) {
      TRIANGLE_TIME = 0;
      TRIANGLE_STATE++;
      return COMPLETED;
    }
    return IN_PROGRESS;

    default: 
    return COMPLETED;
  }
}

Progress triangle_draw_side() {
  set_wheels(FWD, FWD);
  if (CIRCLE_TIME >= CIR_FWD_TIME) {
    CIRCLE_TIME = 0;
    CIRCLE_STATE++;
  }
  return IN_PROGRESS;
}
Progress triangle_rotate() {
  set_wheels(FWD, FWD);
  if (CIRCLE_TIME >= CIR_FWD_TIME) {
    CIRCLE_TIME = 0;
    CIRCLE_STATE++;
  }
  return IN_PROGRESS;
}
