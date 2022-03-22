/// Includes
#include "primitives.h"

/// Functions
void project_7(void);
bool calibrate(void);

/// Defines
//#define base_speed (WHEEL_PERIOD / 2)
#define base_speed (0)
#define k_p_left  ((WHEEL_PERIOD - base_speed) / left_target_value)
#define k_p_right ((WHEEL_PERIOD - base_speed) / right_target_value)