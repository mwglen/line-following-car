/// Functions
void program_start(void);

/// Types
// Program Control
typedef enum {
  STARTUP       = 0,
  MAIN_MENU     = 1,
  DRAW_CIRCLE   = 2,
  DRAW_TRIANGLE = 3,
  DRAW_FIGURE8  = 4,
} Event;

// Triangle
typedef enum {
    DRAW_SIDE1  = 0,
    ROTATE1     = 1,
    DRAW_SIDE2  = 2,
    ROTATE2     = 3,
    DRAW_SIDE3  = 4,
    ROTATE3     = 5,
} TriangleState;

// Figure 8
typedef enum {
    CIRCLE1 = 0,
    CIRCLE2 = 1,
} Figure8State;

/// Defines
//#define CIR_ROT_TIME    (10000);
//#define TRI_SIDE_TIME   (40);
//#define TRI_ROT_TIME    (40);
//#define FIG8_ROT_TIME   (40);

extern const unsigned int CIR_ROT_TIME;
extern const unsigned int TRI_SIDE_TIME;
extern const unsigned int TRI_ROT_TIME;
extern const unsigned int FIG8_ROT_TIME;