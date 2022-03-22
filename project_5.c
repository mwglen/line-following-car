#include "program.h"
#include "project_5.h"
#include "display.h"
#include "wheels.h"
#include "timersB0.h"
#include <string.h>

ProjectState PROJECT5_STATE = SETUP;

void project_5(void) {
 switch (PROJECT5_STATE) {
    case SETUP:
      PROGRAM_COUNT = 0;
      PROJECT5_STATE++;
      strcpy(display_line[0], " PROJECT5 ");
      strcpy(display_line[1], "----------");
      strcpy(display_line[2], "DRIVE  FWD");
      strcpy(display_line[3], "  1 SECS  ");
      display_changed = TRUE; 
      break;
      
    // Travel forwards 1 sec
    case STEP0:
      fwd_left();
      fwd_right();
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "   WAIT   ");
        strcpy(display_line[3], "  1 SECS  ");
        display_changed = TRUE; 
      } break;
      
    // Pause 1 sec
    case STEP1:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "DRIVE  REV");
        strcpy(display_line[3], "  2 SECS  ");
        display_changed = TRUE; 
      } break;
    
    // Travel reverse 2 sec
    case STEP2:
      bwd_left();
      bwd_right();
      if (PROGRAM_COUNT >= TIME_2_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "   WAIT   ");
        strcpy(display_line[3], "  1 SECS  ");
        display_changed = TRUE; 
      } break;
      
    // Pause 1 sec
    case STEP3:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "DRIVE  FWD");
        strcpy(display_line[3], "  1 SECS  ");
        display_changed = TRUE; 
      } break;
    
    // Travel forwards 1 sec
    case STEP4:
      fwd_left();
      fwd_right();
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "   WAIT   ");
        strcpy(display_line[3], "  1 SECS  ");
        display_changed = TRUE;
      } break;
      
    // Pause 1 sec
    case STEP5:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_1_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], " SPIN CW ");
        strcpy(display_line[3], "  3 SECS  ");
        display_changed = TRUE; 
      } break;
      
    // Spin CW for 3 secs
    case STEP6:
      fwd_left();
      bwd_right();
      if (PROGRAM_COUNT >= TIME_3_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "   WAIT   ");
        strcpy(display_line[3], "  2 SECS  ");
        display_changed = TRUE; 
      } break;
      
    // Pause 2 secs
    case STEP7:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_2_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], " SPIN CCW ");
        strcpy(display_line[3], "  3 SECS  ");
        display_changed = TRUE; 
      } break;
      
    // Spin CCW for 3 secs
    case STEP8:
      bwd_left();
      fwd_right();
      if (PROGRAM_COUNT >= TIME_3_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
        strcpy(display_line[0], " PROJECT5 ");
        strcpy(display_line[1], "----------");
        strcpy(display_line[2], "   WAIT   ");
        strcpy(display_line[3], "  1 SECS  ");
        display_changed = TRUE; 
      } break;
      
    // Pause 2 secs
    case STEP9:
      stop_wheels();
      if (PROGRAM_COUNT >= TIME_2_SECS) {
        PROGRAM_COUNT = 0;
        PROJECT5_STATE++;
      } break;
  }
}