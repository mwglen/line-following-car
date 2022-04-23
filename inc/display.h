/// Includes
#include "primitives.h"

/// Functions
void display_process(void);
void init_display(void);

/// Global Variables
//extern bool DISPLAY_CHANGED;
//extern bool UPDATE_DISPLAY;

extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int  update_display_count;