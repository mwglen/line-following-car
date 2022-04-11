/// Includes
#include "wheels.h"
#include "primitives.h"
#include "msp430.h"
#include "ports.h"
#include "timersB0.h"

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

// movement functions for project 5
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