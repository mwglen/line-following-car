/// Includes
#include "msp430.h"
#include "timersB1.h"

/// Functions
void init_timer_B1(void) {
  TB1CTL  = TBSSEL__SMCLK; // SMCLK source
  TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB1CTL |= MC__CONTINOUS; // Continuous up
  TB1CTL |= ID_1; // Divide clock by 2
  TB1EX0  = TBIDEX_7; // Divide clock by an additional 8
  
  // 50ms Timer (Used to sample ADC)
  //TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
  //TB1CCTL0 |= CCIE; // CCR0 enable interrupt
  
  TB1CTL &= ~TBIE; // Disable Overflow Interrupt
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}