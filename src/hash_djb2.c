#include <stdio.h>
#include <stdint.h>

#define hash_type uint32_t

// Hashes a string so that it can be matched in constant time
uint32_t hash(unsigned char *str) {
  uint32_t hash = 5381;
  int c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

int main() {
  char *commands[] = {
    "^^",
    "^S",
    "^F",
    "^U",
    "^Stop",
    "^TurnR",
    "^TurnL",
    "^MoveF",
    "^MoveB",
    "^Exit",
    "^Circle",
    "^Inc"
  };

  for (int i = 0; i < (sizeof(commands)/sizeof(commands[0])); i++) {
    printf("%s:\t", commands[i]);
    printf("%lx\n", hash(commands[i]));
  }

  return 0;
}
