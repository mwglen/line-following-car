/// Functions
void program_start(void);

/// Types
// Program Control
typedef enum {
  STARTUP       = 0,
  MAIN_MENU     = 1,
  PROJECT5      = 2,
} Event;

// Project 5
typedef enum {
  CLEAR_TIMERS = 0,
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
} Project5State;


#define TIME_1_SECS (10)
#define TIME_2_SECS (20)
#define TIME_3_SECS (30)