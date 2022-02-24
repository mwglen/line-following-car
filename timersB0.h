/// Functions
void init_timer_B0(void);

/// Defines
#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 2 / 8 / (1 / 50msec)
#define TB0CCR1_INTERVAL (2500)  // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR2_INTERVAL (2500)  // 8,000,000 / 2 / 8 / (1 / 5msec)