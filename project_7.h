/// Includes
#include <stdbool.h>

/// Functions
void project_7(void);
bool calibrate(void);
void monitor_ir_sensors(void);

/// Globals
extern int max_left_white;
extern int max_right_white;
extern int max_left_black;
extern int max_right_black;

/// Defines
//#define base_speed (WHEEL_PERIOD / 2)
#define base_speed (0)
#define k_p_left  ((WHEEL_PERIOD - base_speed) / left_target_value)
#define k_p_right ((WHEEL_PERIOD - base_speed) / right_target_value)