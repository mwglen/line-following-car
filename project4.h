typedef enum {
  STARTUP = 0,
  MAIN_MENU = 1,
  DRAW_CIRCLE = 2,
  DRAW_TRIANGLE = 3,
  DRAW_FIGURE8 = 4,
} Event;
Event CURR_EVENT = 0;
Event CURR_SELECTION = 1;

// Startup Timer
unsigned int STARTUP_TIME = 0;

// Circle
unsigned int CIR_TIME = 0;
const unsigned int CIR_ROT_TIME = 100000;

// Triangle
typedef enum {
    DRAW_SIDE1  = 0,
    ROTATE1     = 1,
    DRAW_SIDE2  = 2,
    ROTATE2     = 3,
    DRAW_SIDE3  = 4,
} Triangle_State;
Triangle_State TRI_STATE = 0;
unsigned int TRI_TIME = 0;
const unsigned int TRI_SIDE_TIME = 40;
const unsigned int TRI_ROT_TIME = 40;

// Figure 8
typedef enum {
    CIRCLE1 = 0,
    CIRCLE2 = 1,
} Figure8_State;
Figure8_State FIG8_STATE = 0;
unsigned int FIG8_TIME = 0;
const unsigned int FIG8_ROT_TIME = 40;

// Wheel Control
unsigned int LEFT_TIME = 0;
unsigned int RIGHT_TIME = 0;
unsigned int LEFT_WHEEL_POWER = 0;
unsigned int RIGHT_WHEEL_POWER = 0;
const unsigned int MAX_POWER = 100;
void set_wheels(unsigned int, unsigned int);
void drive(void);
