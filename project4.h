typedef enum {
    IN_PROGRESS,
    COMPLETED,
} Progress;

typedef enum {
    STOP_CAR1       = 0,
    WAIT_FOR_PRESS1 = 1, 
    DRAW_CIRCLE     = 2,
    WAIT_FOR_PRESS2 = 3, 
    DRAW_TRIANGLE   = 4,
    WAIT_FOR_PRESS3 = 5, 
    DRAW_FIGURE8    = 6,
} Project4State;
void project_04(void);

//*************************//
//***** WHEEL CONTROL *****//
//*************************//
typedef enum {
  STP,
  FWD,
  BWD,
} WheelDirection;
void stop_car(void);
void set_wheels(WheelDirection, WheelDirection);

//*************************//
//**** BUTTON FUNCTION ****//
//*************************//
Progress wait_for_button(void);

//*************************//
//**** CIRCLE FUNCTION ****//
//*************************//
typedef enum {
    MOVE_FORWARD = 0,
    ROTATE       = 1,
    STOP         = 2,
} CircleState;
Progress draw_circle(void);

//*************************//
//**** FIGURE8 FUNCTION ***//
//*************************//
typedef enum {
    DRAW_CIRCLE1 = 0,
    DRAW_CIRCLE2 = 1,
} Figure8_State;
Progress draw_figure8(void);

//*************************//
//*** TRIANGLE FUNCTION ***//
//*************************//
typedef enum {
    DRAW_SIDE1  = 0,
    ROTATE1     = 1,
    DRAW_SIDE2  = 2,
    ROTATE2     = 3,
    DRAW_SIDE3  = 4,
    ROTATE3     = 5,
} Triangle_State;
Progress draw_triangle(void);
Progress triangle_draw_side(void);
Progress triangle_rotate(void);