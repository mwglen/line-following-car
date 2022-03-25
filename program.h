/// Functions
void program_start(void);

/// Types
// Program Control
typedef enum {
  STARTUP       = 0,
  MAIN_MENU     = 1,
  PROJECT5      = 2,
  PROJECT6      = 3,
  CALIBRATE     = 4,
  PROJECT7      = 5
} Event;
extern Event CURR_EVENT;

// Project States
typedef enum {
  SETUP = 0,
  STEP0 = 1,
  STEP1 = 2,
  STEP2 = 3,
  STEP3 = 4,
  STEP4 = 5,
  STEP5 = 6,
  STEP6 = 7,
  STEP7 = 8,
  STEP8 = 9,
  STEP9 = 10
} ProjectState;