//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Jim Carlson
//  Jan 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include  "macros.h"
#include <string.h>

// Global Variables
volatile char slow_input_down;
unsigned char display_mode;
unsigned int test_value;
char chosen_direction;
char change;

//***********************//
typedef enum {
  FWD,
  BWD
} Direction;

void wheels_straight_control(void) {
    switch(operation) {
        case OFF:
            turn wheels off;
            break;
        case ON:
            if (wheel_period >= DEFINED_PERIOD) {
                wheel_period = 0;
                turn both wheels on
            }
            if (wheel_period >= TO_FAST) {
                TURN FAST WHEEL OFF
            }
            break;
        Default: break;
    }
}

void Movement(void) {
    Switch (shapes) {
        case Circle:

            Switch (Circle_State) {
                case IDLE:
                    break;
                case WAIT:
                    Count some time off your mytime value
                    break;
                case START:
                    this is where you place your wheel control
                    break;
                case END:
                    break;
                Default: break;
            }
        break;
        case Figure8:
            Switch (Figure8_State) {
                case IDLE:
                    break;
                case WAIT:
                    Count some time off your mytime vlaue
                    break;
                case START:
                    break;
                case END:
                    break;
                Default: break;
            }
        break;
    }
}
void drive_wheels(Direction left_dir, Direction right_dir) {
  switch(left_dir) {
    case FWD:
      //P6OUT &= ~L_REVERSE;
      P6OUT |= L_FORWARD;
      break;
    case BWD:
      //P6OUT &= ~L_FORWARD;
      //P6OUT |= L_REVERSE;
      break;
  }
  
  switch(right_dir) {
    case FWD:
      //P6OUT &= ~R_REVERSE;
      P6OUT |= R_FORWARD;
      break;
    case BWD:
      //P6OUT &= ~R_FORWARD;
      //P6OUT |= R_REVERSE;
      break;
  }
}

void draw_circle() {
    drive_wheels(FWD, BWD);
    // Wait certain amount of time
    // wait();
    stop_car();
}

void draw_triangle() {
    drive_wheels(FWD, BWD);
    // Wait certain amount of time
    // wait();
    stop_car();
}

void draw_figure8() {
}

enum {IDLE, GO, RUN, STOP} States;
void drive(&Instructions instructions) {
    switch(states) {
        // Wait for buttom press then move to GO
        case IDLE:
            stop_car();
            break;

        // Configure to go
        case GO:
            break;

        // 
        case RUN:
            break;

        // Wait for buttom press then move to GO
        case STOP:
            stop_car();
            break;

        default: break;
    }
}

typedef struct instructions_struct {
    
} Instructions;

void stop_car() {
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;
  //P6OUT &= ~L_REVERSE;
  //P6OUT &= ~R_REVERSE;
}
//**********************************//

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
void main(void){
  unsigned int old_Time_Sequence;
  unsigned int mytime;

  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD

  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = TRUE;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    if (Time_Sequence != old_Time_Sequence) {
        mytime++;
        old_Time_Sequence = Time_Sequence;
        wheel_period++;
    }
    Carlson_StateMachine();            // Run a Time Based State Machine
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
  }
//------------------------------------------------------------------------------
}

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          lcd_BIG_mid();
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        break;
      case 200:                        //
        if(one_time){
          P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;           // Change State of LED 5
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
//          lcd_4line();
          lcd_BIG_bot();
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          P1OUT &= ~RED_LED;           // Change State of LED 4
          P6OUT &= ~GRN_LED;           // Change State of LED 5
          one_time = 0;
        }
        break;                         //
      default: break;
    }
}
