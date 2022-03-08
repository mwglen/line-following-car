/// Functions
void program_start(void);

/// Types
// Program Control
typedef enum {
  STARTUP       = 0,
  MAIN_MENU     = 1,
  PROJECT5      = 2,
  PROJECT6      = 3
} Event;

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
  STEP9 = 10,
} ProjectState;

/// Defines
#define TIME_100_MS  (1)
#define TIME_1_SECS (10)
#define TIME_2_SECS (20)
#define TIME_3_SECS (30)
#define TIME_4_SECS (40)