#include "msp430.h"
#include "functions.h"

//------------------------------------------------------------------------------
// Description: This function calls the individual Switch Functions
//------------------------------------------------------------------------------
void Switches_Process(void){
  Switch1_Process();
  Switch2_Process();
}

// Globals
int count_debounce_SW1 = 0;
int count_debounce_SW2 = 0;
bool okay_to_look_at_switch1 = true;
bool okay_to_look_at_switch2 = true;
unsigned int sw1_position = RELEASED;
unsigned int sw2_position = RELEASED;


//------------------------------------------------------------------------------
// Switch 1 Configurations
//------------------------------------------------------------------------------
bool SW1_PRESSED = false;
void Switch1_Process(void){
  if (okay_to_look_at_switch1 && sw1_position){
    if (!(P4IN & SW1)){
      sw1_position = PRESSED;
      okay_to_look_at_switch1 = false;
      count_debounce_SW1 = DEBOUNCE_RESTART;
      // do what you want with button press
      SW1_PRESSED = true;
    }
  }
  if (count_debounce_SW1 <= DEBOUNCE_TIME){
    count_debounce_SW1++;
  } else {
    okay_to_look_at_switch1 = true;
      if (P4IN & SW1){
      sw1_position = RELEASED;
    }
  }
}

//------------------------------------------------------------------------------
// Switch 2 Configurations
//------------------------------------------------------------------------------
bool SW2_PRESSED = false;
void Switch2_Process(void){
  if (okay_to_look_at_switch2 && sw2_position){
    if (!(P2IN & SW2)){
      sw2_position = PRESSED;
      okay_to_look_at_switch2 = false;
      count_debounce_SW2 = DEBOUNCE_RESTART;
      // do what you want with button press
      SW2_PRESSED = true;
    }
  }
  if (count_debounce_SW2 <= DEBOUNCE_TIME){
    count_debounce_SW2++;
  } else {
    okay_to_look_at_switch2 = true;
    if (P2IN & SW2){
      sw2_position = RELEASED;
    }
  }
}


bool get_sw1(void) {
  if (SW1_PRESSED) {
    SW1_PRESSED = false;
    return true;
  } else return false;
}


bool get_sw2(void) {
  if (SW2_PRESSED) {
    SW2_PRESSED = false;
    return true;
  } else return false;
}