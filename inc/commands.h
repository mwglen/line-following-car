/// Includes
#include "ring_buffer.h"
#include <stdbool.h>

/// Globals
extern bool circle_cmd_recieved;
extern bool exit_cmd_recieved;
extern bool inc_cmd_recieved;
extern bool cmd_recieved;

/// Functions
void run_cmd(char msg[RING_MSG_LENGTH]);
