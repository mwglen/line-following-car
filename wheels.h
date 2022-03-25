/// Includes
#include "primitives.h"

/// Functions
void init_wheels(void);
void wheels_process(void);
void stop_wheels(void);
void fwd_left(void);
void bwd_left(void);
void fwd_right(void);
void bwd_right(void);
extern long int LEFT_SPEED;
extern long int RIGHT_SPEED;

/// Defines
#define WHEEL_OFF                 (0)
#define WHEEL_PERIOD          (50000)

/// Globals
extern bool wheels_enabled;