/// Includes
#include "primitives.h"

/// Functions
void project_7(void);
void drive_pid(void);
void drive_2_pid(void);
void drive_algorithm(void);

/// Defines
#define base_speed (6000)
#define CENTER_TIME (4)
#define TURN_TIME   (1500)
#define k_p (100) // 40
#define k_d (0)
#define k_i (0)