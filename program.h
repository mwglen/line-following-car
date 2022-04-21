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
#define TIME_50_MS   (50  / TB0CCR0_SR)
#define TIME_100_MS  (100 / TB0CCR0_SR)
#define TIME_150_MS  (150 / TB0CCR0_SR)
#define TIME_200_MS  (200 / TB0CCR0_SR)
#define TIME_1_SECS (1000 / TB0CCR0_SR)
#define TIME_2_SECS (2000 / TB0CCR0_SR)
#define TIME_3_SECS (3000 / TB0CCR0_SR)
#define TIME_4_SECS (4000 / TB0CCR0_SR)