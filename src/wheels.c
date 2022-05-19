/// Includes
#include "wheels.h"
#include "msp430.h"
#include "ports.h"
#include "timersB0.h"
#include "stdlib.h"
#include <stdbool.h>

/// Functions
void init_wheels(void) {
  TB3CTL = TBSSEL__SMCLK;   // SMCLK
  TB3CTL |= MC__UP;         // Up Mode
  TB3CTL |= TBCLR;          // Clear TAR
  TB3CCR0 = WHEEL_PERIOD;   // PWM Period
  
  TB3CCTL1 = OUTMOD_7;                  // CCR1 reset
  RIGHT_FORWARD_SPEED = WHEEL_OFF;      // P6.0 Right Forward PWM duty cycle
  
  TB3CCTL2 = OUTMOD_7;                  // CCR2 reset/set
  LEFT_FORWARD_SPEED  = WHEEL_OFF;      // P6.1 Left Forward PWM duty cycle
  
  TB3CCTL3 = OUTMOD_7;      // CCR3 reset/set
  RIGHT_REVERSE_SPEED = WHEEL_OFF;  // P6.2 Right Reverse PWM duty cycle
  
  TB3CCTL4 = OUTMOD_7;      // CCR4 reset/set
  LEFT_REVERSE_SPEED  = WHEEL_OFF;  // P6.3 Left Reverse PWM duty cycle
}

// Movement Functions
void stop_wheels()  { LEFT_SPEED = 0; RIGHT_SPEED = 0;}
void fwd_left() { LEFT_SPEED = WHEEL_PERIOD/4; }
void bwd_left() { LEFT_SPEED = -WHEEL_PERIOD/2; }
void fwd_right() { RIGHT_SPEED = WHEEL_PERIOD/4; }
void bwd_right() { RIGHT_SPEED = -WHEEL_PERIOD/2; }

// Check wheels to make sure they are safe
void check_wheels() {
  // Make sure that the wheels are safe to drive and then drive 
  if ((LEFT_FORWARD_SPEED && LEFT_REVERSE_SPEED)
    || (RIGHT_FORWARD_SPEED && RIGHT_REVERSE_SPEED)) {
    stop_wheels();
    P1OUT &= ~RED_LED; // Turn on Red LED
    while (true) {}    // Halt Program
  }
}

// Indirect Wheel Speeds
#define LFS (LEFT_FORWARD_SPEED)
#define RFS (RIGHT_FORWARD_SPEED)
#define LRS (LEFT_REVERSE_SPEED)
#define RRS (RIGHT_REVERSE_SPEED)

// Wheel Speed 
long int LEFT_SPEED = 0;
long int RIGHT_SPEED = 0;
long int prev_ls = 0;
long int prev_rs = 0;

#define LS (LEFT_SPEED)
#define RS (RIGHT_SPEED)

void wheels_process() {
  if (wheels_process_flag) {    
    // Reset Flag
    wheels_process_flag = false;

    // Update Right Wheel
    if (RS != prev_rs) {
      if (((RFS > 0) && (RS < 0)) || ((RRS > 0) && (RS > 0))) {
        RFS = 0; RRS = 0;
      } else {
        RFS = RS > 0 ? abs(RS) : 0; 
        RRS = RS < 0 ? abs(RS) : 0; 
        prev_rs = RS;
      }
    }
    
    // Update Left Wheel
    if (LS != prev_ls) {
      if (((LFS > 0) && (LS < 0)) || ((LRS > 0) && (LS > 0))) {
        LFS = 0; LRS = 0;
      } else {
        LFS = LS > 0 ? abs(LS) : 0; 
        LRS = LS < 0 ? abs(LS) : 0; 
        prev_ls = LS;
      }
    }
  }
}