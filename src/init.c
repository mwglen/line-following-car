/// Includes
#include "primitives.h"
#include "msp430.h"
#include "init.h"
#include "system.h"
#include "display.h"

/// Functions
void init_conditions(void){

  int i;
  for(i=0;i<11;i++){
    display_line[0][i] = 0;
    display_line[1][i] = 0;
    display_line[2][i] = 0;
    display_line[3][i] = 0;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  display_changed = 1; 

  // Interrupts are disabled by default, enable them.
  enable_interrupts();
}

