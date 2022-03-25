/// Includes
#include "primitives.h"

/// Globals
extern char recieved_message[20];
extern short unsigned int recieved_index;
extern short unsigned int TX_index;
extern bool send_transmission;
extern char transmission[10];
extern bool message_recieved;

/// Functions
void Init_Serial_UCA0(int, int);
