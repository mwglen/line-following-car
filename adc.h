#include <stdbool.h>

/// Globals
extern char ADC_CHAR[4];
extern int LEFT_IR_VALUE;
extern int RIGHT_IR_VALUE;
extern bool NEW_ADC_VALUES;

/// Functions
void init_adc(void);
void hex_to_bcd(int);
void init_ref(void);