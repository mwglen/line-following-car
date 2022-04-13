/// Includes
#include "commands.h"
#include "pc.h"
#include "iot.h"
#include "text.h"
#include "ring_buffer.h"
#include "timersB0.h"
#include "wheels.h"
#include "display.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

extern void lcd_BIG_mid(void);

// A buffer to hold movement commands
// Strings are in format [FBLR][0..9]*
RingBuffer mv_cmd_buffer = {
  .write_idx = 0,
  .read_idx = 0,
  .curr_size = 0
};

/// Functions
// Parse and run commands
void run_cmd(char cmd[RING_MSG_LENGTH]) {
  if (cmd[0] == '^') {
    
    center_cpy(display_line[1], cmd);
    lcd_BIG_mid();
    
    // Test Response 
    if (starts_with(cmd, "^^")) {
      write_buffer(&pc_tx_buffer, "PC READY\r\n");
     
    // Change IOT to Fast Mode  
    } else if (starts_with(cmd, "^F")) {
      init_iot(4, 0x5551); //115200 
      write_buffer(&pc_tx_buffer, "FAST BUAD\r\n");
     
    // Change IOT to Slow Mode
    } else if (starts_with(cmd, "^S")) {
      init_iot(52, 0x4911); //9600 
      write_buffer(&pc_tx_buffer, "SLOW BUAD\r\n");
     
    // Update Display with IOT info
    } else if (starts_with(cmd, "^U")) {
      display_iot_flag = true;
     
    // TCP Commands
    } else if (starts_with(cmd, "^0000")) {
      // Add movement commands to buffer
      char *ptr = cmd + 5;
      while (*ptr != '\r') {
        // Create a string to hold command
        char new_mv_cmd[RING_MSG_LENGTH] = "";
        
        // Copy direction for command
        strncat(new_mv_cmd, ptr++, 1);
        
        // Copy digits for command
        while (isdigit(*ptr))
          strncat(new_mv_cmd, ptr++, 1);
        
        // Write command to buffer
        write_buffer(&mv_cmd_buffer, new_mv_cmd);
      }
      
      
    // Return Error
    } else write_buffer(&pc_tx_buffer, "COMMAND NOT FOUND\r\n");
  
  // Passthrough to iot
  } else write_buffer(&iot_tx_buffer, cmd);
}

// Reads a move command from buffer and runs the command
void run_move_cmd(void) {
  if (!stop_after_flag && mv_cmd_buffer.curr_size != 0) {
    // Get movement command
    char curr_mv_cmd[RING_MSG_LENGTH];
    read_buffer(&mv_cmd_buffer, curr_mv_cmd);
    
    // Parse Direction
    switch (curr_mv_cmd[0]) {
      case 'F':
        LEFT_SPEED  =  WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/4;
        break;
        
      case 'B':
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED = -WHEEL_PERIOD/4;
        break;
        
      case 'R':
        LEFT_SPEED  =  WHEEL_PERIOD/4;
        RIGHT_SPEED = -WHEEL_PERIOD/4;
        break;
          
      case 'L':
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/4;
        break;
    }
              
    // Determine time to stop after
    stop_after_time = 0;
    for (int i = 1; isdigit(curr_mv_cmd[i]); i++)
        stop_after_time = stop_after_time * 10 + curr_mv_cmd[i] - '0';
    
    // Set flag signaling to stop after time
    stop_after_curr_time = 0;
    stop_after_flag = true;
  }
}