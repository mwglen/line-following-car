#include "primitives.h"
#include "program.h"
#include "msp430.h"
#include "switches.h"
#include "display.h"
#include "wheels.h"
#include "timersB0.h"
#include "ports.h"
#include "adc.h"
#include <string.h>

/// Global Variables
// Main Function
unsigned int STARTUP_TIME = 0;
Event CURR_EVENT = STARTUP;
Event CURR_SELECTION = MAIN_MENU;

// Programs
ProjectState PROJECT5_STATE = SETUP;
ProjectState PROJECT6_STATE = STEP0;

#define BLACK_LINE_VAL (300)

// Program 6
//char ir_status[10]  = false ? "EMITTER ON" : "  IR OFF  ";
char program_status[] = " Program6 ";
char ir_status[]    = "  IR OFF  ";
char left_ir_str[]  = " L:  xxxx ";
char right_ir_str[] = " R:  xxxx ";
char ADC_CHAR[4];
int LEFT_IR_VALUE;
int RIGHT_IR_VALUE;

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
            CURR_SELECTION = PROJECT6;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], " COMPLETE ");
            strcpy(display_line[3], " PROJECT6 ");
            display_changed = TRUE; 
          } else if (get_sw1()) {
            CURR_EVENT = PROJECT5;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "PREFORMING");
            strcpy(display_line[2], " PROJECT5 ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;  
          } break;
          
        case PROJECT6:
          if (get_sw2()) {
            CURR_SELECTION = PROJECT5;
            strcpy(display_line[0], "MAIN  MENU");
            strcpy(display_line[1], "----------");
            strcpy(display_line[2], " COMPLETE ");
            strcpy(display_line[3], " PROJECT5 ");
            display_changed = TRUE; 
          } else if (get_sw1()) {
            CURR_EVENT = PROJECT6;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "PREFORMING");
            strcpy(display_line[2], " PROJECT6 ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;  
          } break;
        
      } break;
      
  case PROJECT5:
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
    } break;
    
  case PROJECT6:
    
    // Monitor ADC    
    // Fill in right IR sensor values
    hex_to_bcd(LEFT_IR_VALUE);
    for (int i = 0; i < 4; i++)
      left_ir_str[i+5] = ADC_CHAR[i];
    
    // Fill in left IR sensor values
    hex_to_bcd(RIGHT_IR_VALUE);
    for (int i = 0; i < 4; i++)
      right_ir_str[i+5] = ADC_CHAR[i];
     
    // Monitor ADC Values
    strcpy(display_line[0], program_status);
    strcpy(display_line[1], ir_status);
    strcpy(display_line[2], left_ir_str);
    strcpy(display_line[3], right_ir_str);
    display_changed = true;
    
    switch (PROJECT6_STATE) {
      // Wait for SW1
      case STEP0:
        if (get_sw1()) {
          // Turn on Emitter
          strcpy(ir_status, "EMITTER ON");
          P6OUT |= IR_EMITTER; // On [High]
          PROGRAM_COUNT = 0;
          PROJECT6_STATE++;
        } break;
        
      // Wait 1 Second
      case STEP1:
        if (PROGRAM_COUNT >= TIME_1_SECS) {
          PROGRAM_COUNT = 0;
          PROJECT6_STATE++;
        } break;
      
      // Move forward until line is detected
      case STEP2:
        fwd_left();
        fwd_right();
        if (LEFT_IR_VALUE > BLACK_LINE_VAL || RIGHT_IR_VALUE > BLACK_LINE_VAL) {
          stop_wheels();
          PROGRAM_COUNT = 0;
          PROJECT6_STATE++;
        } break;
        
      // Stop for 4 Seconds
      case STEP3:
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "BLACK LINE");
        strcpy(display_line[2], " DETECTED ");
        strcpy(display_line[3], "          ");
        if (PROGRAM_COUNT >= TIME_4_SECS) {
          PROGRAM_COUNT = 0;
          PROJECT6_STATE++;
        } break;
        
      // Move Forward until center of car is on line
      case STEP4:
        fwd_left();
        fwd_right();
        if (PROGRAM_COUNT >= TIME_100_MS) {
          PROGRAM_COUNT = 0;
          PROJECT6_STATE++;
        } break;
        
      // Wait to avoid running FWD directly after REV
      case STEP5:
        stop_wheels();
        if (PROGRAM_COUNT >= TIME_100_MS) {
          PROGRAM_COUNT = 0;
          PROJECT6_STATE++;
        } break;
        
      // Rotate until lined up
      case STEP6:
        fwd_left();
        bwd_right();
        if (LEFT_IR_VALUE > 300) {
          stop_wheels();
          PROJECT6_STATE = STEP0;
          CURR_EVENT = MAIN_MENU;
        } break;
    } break;
  }
}