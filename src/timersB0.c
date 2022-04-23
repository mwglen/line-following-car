/// Includes
#include "msp430.h"
#include "timersB0.h"
#include "wheels.h"
#include "display.h"
#include "ports.h"
#include "switches.h"
#include "program.h"
#include "iot.h"
#include "text.h"
#include "adc.h"
#include <string.h>
#include <stdbool.h>

/// Globals
// Flags
bool iot_process_flag = false;
bool wheels_process_flag  = false;
bool pc_process_flag = false;
bool adc_process_flag = false;

// Timer
unsigned int timer_count = 0;
bool timer_enable = false;
bool timer_updated = false;

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

unsigned int IOT_EN_COUNT  = 0;
unsigned int DISPLAY_COUNT = 0;
long unsigned int PROGRAM_COUNT = 0;
long unsigned int TASK_COUNT = 0;
unsigned int WHEEL_COUNT = 0;
unsigned int PC_COUNT = 0;
unsigned int IOT_COUNT = 0;
unsigned int ADC_COUNT = 0;
unsigned int DISPLAY_TIMER_COUNT = 0;

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  // Update Program Count
  PROGRAM_COUNT++;
  TASK_COUNT++;
  
  // Set Display Process Flag
  if (++DISPLAY_COUNT == TIME_200_MS) {
    update_display = 1;
    DISPLAY_COUNT = 0;
  }
  
  // Set IOT EN Process Flag
  if (++IOT_EN_COUNT == TIME_200_MS) {
    P3OUT |= IOT_EN_CPU; 
    IOT_EN_COUNT = 0;
  }
  
  // Set Wheel Process Flag
  if (++WHEEL_COUNT == TIME_50_MS) {
    wheels_process_flag = true; 
    WHEEL_COUNT = 0;
  } 
  
  // Set IOT Process Flag  
  if (++IOT_COUNT   == TIME_50_MS) {
    iot_process_flag = true; 
    IOT_COUNT = 0;
  }
  
  // Set PC Process Flag
  if (++PC_COUNT   == TIME_50_MS) {
    pc_process_flag = true; 
    PC_COUNT = 0;
  }   

  // Set ADC Process Flag
  if (++ADC_COUNT == TIME_50_MS) {
    adc_process_flag = true;
    ADC_COUNT = 0;
  } 
  
  if (++DISPLAY_TIMER_COUNT == TIME_1_SECS) {
    DISPLAY_TIMER_COUNT = 0;
    if (timer_enable) {
      // Increment Timer
      timer_count++;
      timer_updated = true;
    }
  }
  
  TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
}

unsigned short int DEBOUNCE_COUNT = 0;
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void) {
  switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    
    case 2: break; // Unused CCR
    
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