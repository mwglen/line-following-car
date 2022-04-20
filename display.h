/// Includes
#include <stdbool.h>
#include "display_iot.h"

/// Functions
void display_process(void);
void init_display(void);

/// Global Variables
//extern bool DISPLAY_CHANGED;
//extern bool UPDATE_DISPLAY;
extern bool display_iot_flag;
extern bool timer_enable;

// External Variables
extern char display_line[4][11];
extern char *display[4];
extern void Display_Update(char, char, char, char);

extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int  update_display_count;