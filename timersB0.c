/// Includes
#include "msp430.h"
#include "timersB0.h"
#include "wheels.h"

/// Functions
void init_timer_B0(void) {
  TB0CTL = TBSSEL__SMCLK; // SMCLK source
  TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS; // Continuous up
  TB0CTL |= ID_1; // Divide clock by 2
  TB0EX0 = TBIDEX_7; // Divide clock by an additional 8
  
  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt
  
  // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
  // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
  
  // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
  // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
  
  TB0CTL &= ~TBIE; // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  // Add What you need happen in the interrupt
  TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    
    case 2: // CCR1 not used
      // Add What you need happen in the interrupt
      TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
      break;
    
    case 4: // CCR2 not used
      // Add What you need happen in the interrupt
      TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
      break;
    
    case 14: // overflow
      // Add What you need happen in the interrup
      break;
      
    default: break;
  }
}