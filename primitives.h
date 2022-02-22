#pragma once

/// Types
typedef enum {
  false = 0,
  true  = 1,
} bool;


/// Global Variables
extern char display_line[4][11];
extern char *display[4];
extern void Display_Update(char, char, char, char);

/// Defines
#define ALWAYS          (1)
#define TRUE            (0x01)
#define RESET_STATE     (0)