/// Functions
void init_wheels(void);
void stop_wheels(void);
void fwd_left(void);
void bwd_left(void);
void fwd_right(void);
void bwd_right(void);
void check_wheels(void);

/// Defines
#define WHEEL_OFF                 (0)
#define WHEEL_PERIOD          (50000)
#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED  (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED  (TB3CCR4)
