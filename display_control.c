/// Includes
#include "display_control.h"
#include "display.h"
#include <string.h>

/// Globals
char display_lines[][4] = {"", "", "", ""};
int  display_count[4] = {0, 0, 0, 0};

/// Functions
void display_full() {
  for (i = 0; i < 4; i++) {

    // We must cycle through the chars of strings 
    // with lengths greater than 10
    if (length(lines[i]) > 10) {
      display_count[i] = (display_count[i] + 1) % length(lines[i]);
      strcpy(display_line[i], memcpy(lines[i] + display_count[i], 10);
    } else {
      // Display String
      strcpy(display_line[i], memcpy(lines[i], 10));
    }

  }
}

void set_display(char[] string, int line) {
  strcpy(lines[line], string);
}