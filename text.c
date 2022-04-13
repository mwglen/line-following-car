/// Includes
#include "string.h"
#include "text.h"

/// Functions
void center_cpy(char *dst, char *src) {
  if (strlen(src) < 10) {
    int padding_width = 5 - strlen(src)/2;
    
    strcpy(dst, "");
    for (int i = 0; i < padding_width; i++)
      strncat(dst, " ", 1);
    strncat(dst, src, strlen(src));
    for (int i = 0; i < padding_width; i++)
      strncat(dst, " ", 1);
  } else strncpy(dst, src, 10);
}

bool starts_with(char *src1, char *src2) {
  if (strncmp(src1, src2, strlen(src2)) == 0) return true;
  else return false;
}
/*
// A simple atoi() function
int atoi(char* str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return res;
}
*/
 