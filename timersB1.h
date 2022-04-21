/// Functions
void init_timer_B1(void);

/// Defines
// CHANGE ME
#define TB1CCR0_SR   (5) // Every 5ms
#define TB1CCR1_SR (100) // Every 100ms
#define TB1CCR2_SR (100) // Every 100ms

// 2500 = 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB1CCR0_INTERVAL (TB1CCR0_SR * 500)
#define TB1CCR1_INTERVAL (TB1CCR1_SR * 500)
#define TB1CCR2_INTERVAL (TB1CCR2_SR * 500)