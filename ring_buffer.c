/// Includes
#include "ring_buffer.h"
#include <string.h>
#include <stdlib.h>

void write_buffer(RingBuffer *buf, char *msg) {
  if (buf->curr_size == RING_BUF_LENGTH) return;
  
  // Write to buffer
  strncpy(buf->msg[buf->write_idx++], msg, RING_MSG_LENGTH - 1);
  
  // Update curr_size
  buf->curr_size++;
  
  // Update write_idx
  if (buf->write_idx >= RING_BUF_LENGTH-1)
    buf->write_idx = 0;
}

void read_buffer(RingBuffer *buf, char *msg) {
  if (buf->curr_size == 0) return;
  
  // Read buffer
  strncpy(msg, buf->msg[buf->read_idx++], RING_MSG_LENGTH - 1);
  
  // Update curr_size
  buf->curr_size--;
  
  // Update read_idx
  if (buf->read_idx >= RING_BUF_LENGTH-1)
    buf->read_idx = 0;
}