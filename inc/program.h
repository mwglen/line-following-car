/// Functions
void run_program(void);

/// Types
// Project States
typedef enum {
  WAIT_FOR_WIFI = 0,
  CALIBRATE     = 1,
  WAIT_FOR_CMD  = 2,
  WAIT_FOR_CIRC = 3,
  INTERCEPTING  = 4,
  CIRCLING      = 5,
  EXITING       = 6,
  FINISHED      = 7
} ProgramState;

// Program States
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
#define TIME_5_SECS (5000 / TB0CCR0_SR)