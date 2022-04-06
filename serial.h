/// Includes
#include "primitives.h"

/// Globals
extern char recieved_message[10];
extern short unsigned int recieve_index;
extern short unsigned int TX_index;
extern bool send_transmission;
extern char transmission[10];
extern bool message_recieved;
extern bool UCA1_connected;

/// Functions
void Init_Serial_UCA0(int, int);
void Init_Serial_UCA1(int, int);