/// Functions
void init_timer_B1(void);

/// Defines
#define TB1CCR0_INTERVAL  (5000) // 8,000,000 / 2 / 8 / (1 / 10msec)
#define TB1CCR1_INTERVAL (50000) // 8,000,000 / 2 / 8 / (1 / 100msec)
#define TB1CCR2_INTERVAL (50000) // 8,000,000 / 2 / 8 / (1 / 100msec)