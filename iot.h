/// Includes
#include <stdbool.h>
#include "ring_buffer.h"

/// Globals
extern char iot_trans[RING_MSG_LENGTH];
extern bool message_recieved;

// Network Strings
extern char ssid[20];
extern char ip_addr[20];
extern char ip_addr1[10];
extern char ip_addr2[10];
extern RingBuffer iot_tx_buffer;
extern RingBuffer iot_tx_buffer;

/// Functions
void init_iot(int, int);
void transmit_iot(char *);
void display_iot(void);
void iot_process(void);