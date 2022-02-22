#include "wheels.h"
#include "msp430.h"
#include "ports.h"

/// Global Variables
unsigned int LEFT_TIME = 0;
unsigned int RIGHT_TIME = 0;
unsigned int LEFT_WHEEL_POWER = 0;
unsigned int RIGHT_WHEEL_POWER = 0;
const unsigned int MAX_POWER = 100;
const int WHEEL_CORRECTION_FACTOR = 0;

/// Functions
// set the power delivered to each wheel
void set_wheels_dir(unsigned int left_power, unsigned int right_power) {
  LEFT_WHEEL_POWER = left_power;
  RIGHT_WHEEL_POWER = right_power;  
}

// drive the car
void drive_car(void){
  // calculate target times
  unsigned int targ_left_time = 0;
  targ_left_time = LEFT_WHEEL_POWER;
  unsigned int targ_right_time = 0; 
  if (RIGHT_WHEEL_POWER > WHEEL_CORRECTION_FACTOR) {
    targ_right_time = RIGHT_WHEEL_POWER - WHEEL_CORRECTION_FACTOR;
  } else { targ_right_time = 0; }
  
  // drive left wheel
  if (LEFT_TIME > MAX_POWER) LEFT_TIME = 0;
  if (LEFT_TIME < targ_left_time)
    P6OUT |=  L_FORWARD;
  else if (LEFT_TIME <= MAX_POWER-LEFT_WHEEL_POWER)
    P6OUT &= ~L_FORWARD;
  LEFT_TIME++;
  
  // drive right wheel
  if (RIGHT_TIME > MAX_POWER) RIGHT_TIME = 0;
  if (RIGHT_TIME < targ_right_time)
    P6OUT |=  R_FORWARD;
  else if (RIGHT_TIME <= MAX_POWER-RIGHT_WHEEL_POWER) 
    P6OUT &= ~R_FORWARD;
  RIGHT_TIME++;
}