/// Includes
#include "msp430.h"
#include "timersB1.h"
#include "wheels.h"
#include "primitives.h"

/// Functions
void init_timer_B1(void) {
  TB1CTL  = TBSSEL__SMCLK; // SMCLK source
  TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB1CTL |= MC__CONTINOUS; // Continuous up
  TB1CTL |= ID_1; // Divide clock by 2
  TB1EX0  = TBIDEX_7; // Divide clock by an additional 8
  
  // 50ms Timer (Used to sample ADC)
  TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
  TB1CCTL0 |= CCIE; // CCR0 enable interrupt
  
  // 50ms Timer (Used to disable wheels)
  TB1CCR1 = TB1CCR1_INTERVAL; // CCR1
  TB1CCTL1 &= ~CCIE; // CCR0 disable interrupt
  
  TB1CTL &= ~TBIE; // Disable Overflow Interrupt
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  ADCCTL0 |= ADCENC; // Enable Conversions
  ADCCTL0 |= ADCSC;  // Start next sample
  TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1
}

bool ready_to_enable_wheels = false;
#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
  switch(__even_in_range(TB1IV,14)){
    case 0: break; // No interrupt
    
    case 2: // Wheel Renable
      // Add What you need happen in the interrupt
      if (ready_to_enable_wheels) {
        ready_to_enable_wheels = false;
        wheels_enabled = true;
        TB1CCTL1 &= ~CCIE;    // CCR2 disable interrupt
      } else ready_to_enable_wheels = true;
      TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1
      break;
    
    case 4:
      break;
      
    case 14: // overflow
      // Add What you need happen in the interrup
      break;
      
    default: break;
  }
}
