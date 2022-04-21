/// Includes
#include <stdbool.h>

/// Functions
void init_timer_B0(void);
void clear_program_timer(void);

/// Defines
// CHANGE ME
#define TB0CCR0_SR  (50) // Every  50ms
#define TB0CCR1_SR (100) // Every 100ms
#define TB0CCR2_SR (100) // Every 100ms

// 2500 = 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR0_INTERVAL (TB0CCR0_SR * 500U)
#define TB0CCR1_INTERVAL (TB0CCR1_SR * 500U)
#define TB0CCR2_INTERVAL (TB0CCR2_SR * 500U)

extern long unsigned int PROGRAM_COUNT;
extern long unsigned int TASK_COUNT;
//extern short unsigned int DISPLAY_COUNT;
extern bool wheels_process_flag;
extern bool iot_process_flag;
extern bool pc_process_flag;
extern bool adc_process_flag;
extern long int LEFT_SPEED;
extern long int RIGHT_SPEED;