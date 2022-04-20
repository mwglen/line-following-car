/// Includes
#include "msp430.h"
#include "timersB0.h"
#include "wheels.h"
#include "display.h"
#include "ports.h"
#include "switches.h"
#include "iot.h"
#include <stdbool.h>

/// Globals
// Flags
bool iot_process_flag = false;
bool wheels_process_flag  = false;
bool pc_process_flag = false;
bool adc_process_flag = false;

/// Functions
void init_timer_B0(void) {
  TB0CTL = TBSSEL__SMCLK; // SMCLK source
  TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS; // Continuous up
  TB0CTL |= ID_1; // Divide clock by 2
  TB0EX0 = TBIDEX_7; // Divide clock by an additional 8
  
  // 50ms Timer (Used to update display)
  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt
  
  // 100ms Timer (Counts every 1s for program use)
  //TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
  //TB0CCTL1 |= CCIE; // CCR1 enable interrupt
  
  // 100ms Timer (Debounce Timer)
  TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
  TB0CCTL2 |= CCIE; // CCR2 enable interrupt
  
  TB0CTL &= ~TBIE; // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

// 50 ms timer
short unsigned int IOT_EN_COUNT  = 0;
//extern short unsigned int DISPLAY_COUNT = 0;
short unsigned int DISPLAY_COUNT = 0;
long  unsigned int PROGRAM_COUNT = 0;
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  // Update Counts
  PROGRAM_COUNT++;
  DISPLAY_COUNT++;
  IOT_EN_COUNT++;
  
  // Set Flags
  if (++DISPLAY_COUNT ==  4) {update_display = 1;  DISPLAY_COUNT = 0;}
  //if (++IOT_EN_COUNT  == 10) {P3OUT |= IOT_EN_CPU; IOT_EN_COUNT  = 0;}
  wheels_process_flag = true;   
  //iot_process_flag    = true;
  //pc_process_flag     = true;
  adc_process_flag    = true;
  
  TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
}

unsigned short int DEBOUNCE_COUNT = 0;
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    
    case 2:
      break;
    
    case 4: // Debounce Timer
      TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
      if (DEBOUNCE_COUNT == 5) {
        DEBOUNCE_COUNT = 0;
        TB0CCTL2 &= ~CCIE;    // CCR2 disable interrupt
        P4IFG &= ~SW1;        // Clear Interrupt Flags
        P2IFG &= ~SW2;        // Clear Interrupt Flags
        P4IE |= SW1;          // P4.2 interrupt enabled
        P2IE |= SW2;          // P4.2 interrupt enabled
      } else DEBOUNCE_COUNT++;
      break;
      
    case 14: // overflow
      // Add What you need happen in the interrup
      break;
      
    default: break;
  }
}