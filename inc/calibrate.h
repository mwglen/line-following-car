#include <stdbool.h>

/// Types
// Calibration States
typedef enum {
  PLACE_ON_W     = 0,
  GETTING_W      = 1,
  GETTING_B      = 2,
  DISPLAY_VALUES = 3
} CalibrationState;

/// Globals
extern int max_left_white;
extern int max_right_white;
extern int max_left_black;
extern int max_right_black;

/// Functions
bool calibrate(void);