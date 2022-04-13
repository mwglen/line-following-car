#pragma once

/// Defines
#define	RING_MSG_LENGTH 20
#define RING_BUF_LENGTH 20

/// Typedefs
typedef char RingMessage[RING_MSG_LENGTH];
typedef struct RingBuffStruct {
  RingMessage msg[RING_BUF_LENGTH];
  unsigned int write_idx;
  unsigned int read_idx;
  unsigned int curr_size;
} RingBuffer;

/// Functions
void write_buffer(RingBuffer *, char *);
void read_buffer(RingBuffer *, char *);
