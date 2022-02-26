#include "primitives.h"

/// Functions
void init_timer_B0(void);
void clear_program_timer(void);

/// Defines
#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 2 / 8 / (1 / 50msec)
#define TB0CCR1_INTERVAL (50000)  // 8,000,000 / 2 / 8 / (1 / 100msec)
#define TB0CCR2_INTERVAL (50000)  // 8,000,000 / 2 / 8 / (1 / 100msec)

extern short unsigned int PROGRAM_COUNT;
extern short unsigned int DISPLAY_COUNT;