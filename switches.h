#include "primitives.h"

/// Functions
void switches_process(void);
void switch1_process(void);
void switch2_process(void);
bool get_sw1(void);
bool get_sw2(void);

/// Global Variables
extern bool SW1_PRESSED;
extern bool SW2_PRESSED;
extern unsigned int sw1_position;
extern unsigned int sw2_position;
extern bool send_transmission;
extern char line_to_display [10];
extern long int buad_rate;

/// Defines
#define DEBOUNCE_TIME (10000)
#define DEBOUNCE_RESTART (0x00)
#define PRESSED (0)
#define RELEASED (1)
