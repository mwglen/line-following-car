/// Functions
void program_start(void);

/// Types
// Program Control
typedef enum {
  STARTUP       = 0,
  MAIN_MENU     = 1,
  PROJECT5      = 2,
  PROJECT6      = 3,
  PROJECT7      = 4
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
  STEP9 = 10,
} ProjectState;

/// Defines
#define TIME_50_MS   (1)
#define TIME_100_MS  (2)
#define TIME_150_MS   (3)
#define TIME_1_SECS (20)
#define TIME_2_SECS (40)
#define TIME_3_SECS (60)
#define TIME_4_SECS (80)