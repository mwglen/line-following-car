/// Includes
#include "primitives.h"

/// Functions
void init_timer_B0(void);
void reset_timers(void);
bool wait(void);

/// Defines
#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 2 / 8 / (1 / 50ms)
#define TB0CCR1_INTERVAL   (500) // 8,000,000 / 2 / 8 / (1 / 1ms)
#define TB0CCR2_INTERVAL (50000) // 8,000,000 / 2 / 8 / (1 / 100ms)
#define TIME_1_MS       (1)
#define TIME_100_MS   (100)
#define TIME_150_MS   (150)
#define TIME_1_SECS  (1000)
#define TIME_2_SECS  (2000)
#define TIME_3_SECS  (3000)
#define TIME_4_SECS  (4000)
#define CIRCLE_TIME (20000)

extern long unsigned int PROGRAM_COUNT;
extern long unsigned int PROGRAM_COUNT2;
extern short unsigned int DISPLAY_COUNT;