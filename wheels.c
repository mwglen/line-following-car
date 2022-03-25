/// Includes
#include "wheels.h"
#include "msp430.h"
#include "ports.h"
#include "timersB0.h"
#include "primitives.h"
#include <stdlib.h>

#define LFS (LEFT_FORWARD_SPEED)
#define RFS (RIGHT_FORWARD_SPEED)
#define LRS (LEFT_REVERSE_SPEED)
#define RRS (RIGHT_REVERSE_SPEED)

#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED  (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED  (TB3CCR4)

long int LEFT_SPEED = 0;
long int RIGHT_SPEED = 0;
#define LS  (LEFT_SPEED)
#define RS  (RIGHT_SPEED)

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


unsigned int PREV_LEFT_SPEED;
unsigned int PREV_RIGHT_SPEED;
bool wheels_enabled = true;
void wheels_process(void) {
  
  // Update left wheel
  if (wheels_enabled && (LEFT_SPEED != PREV_LEFT_SPEED)) {
    if (((LFS > 0) && (LS < 0)) || ((LRS > 0) && (LS > 0))) {
      wheels_enabled = false;
      TB1CCTL1 |= CCIE;    // CCR2 enable interrupt
    } else {
      LFS = LS > 0 ? abs(LS) : 0; 
      LRS = LS < 0 ? abs(LS) : 0; 
    }    
  }
  
  // Update right wheel
  if (wheels_enabled && (RIGHT_SPEED != PREV_RIGHT_SPEED)) {
    if (((RFS > 0) && (RS < 0)) || ((RRS > 0) && (RS > 0))) {
      wheels_enabled = false;
      TB1CCTL1 |= CCIE;    // CCR2 enable interrupt
    } else {
      RFS = RS > 0 ? abs(RS) : 0; 
      RRS = RS < 0 ? abs(RS) : 0; 
    }
  }
  
  // Update previous speeds
  PREV_LEFT_SPEED = LEFT_SPEED;
  PREV_RIGHT_SPEED = RIGHT_SPEED;
}