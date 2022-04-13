#include "stdbool.h"
#include "ring_buffer.h"

/// Globals
extern bool pc_connected;
extern RingBuffer pc_tx_buffer;
extern RingBuffer pc_rx_buffer;

/// Functions
void init_pc(int, int);
void transmit_pc(char *);
void pc_process(void);