#include "wheels.h"
#include "msp430.h"
#include "ports.h"

/// Functions
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 R_FORWARD
// TB3.2 P6.1 L_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_REVERSE
//------------------------------------------------------------------------------
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

// set the power delivered to each wheel
void set_wheels(
  unsigned int left_forward_speed, 
  unsigned int left_reverse_speed,
  unsigned int right_forward_speed, 
  unsigned int right_reverse_speed
){
  LEFT_FORWARD_SPEED = left_forward_speed;
  LEFT_REVERSE_SPEED = left_reverse_speed;
  RIGHT_FORWARD_SPEED = right_forward_speed;
  RIGHT_REVERSE_SPEED = right_reverse_speed;
}

// Movement Functions for Project 5
void stop_wheels()  { set_wheels(0, 0, 0, 0); }
void fwd_left() { LEFT_FORWARD_SPEED = WHEEL_PERIOD/2; }
void bwd_left() { LEFT_REVERSE_SPEED = WHEEL_PERIOD/2; }
void fwd_right() { RIGHT_FORWARD_SPEED = WHEEL_PERIOD/2; }
void bwd_right() { RIGHT_REVERSE_SPEED = WHEEL_PERIOD/2; }