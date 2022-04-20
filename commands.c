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
  
  // Parse commands
  if (cmd[0] == '^') {
    // Print command to screen
    strncpy(display_line[1], cmd, 10);
    lcd_BIG_mid();
    
    // Run command
    switch (hash(cmd)) {
      // ^^: Test Response
      case 0x5976c1UL:
        write_buffer(&pc_tx_buffer, "PC READY\r\n");
        break;
        
      // ^S: Change IOT to Slow Mode
      case 0x5976b6UL: // ^S           
        init_iot(52, 0x4911); //9600 
        write_buffer(&pc_tx_buffer, "SLOW BUAD\r\n");
        break;

      // ^F: Change IOT to Fast Mode:
      case 0x5976a9UL:
        init_iot(4, 0x5551); //115200 
        write_buffer(&pc_tx_buffer, "FAST BUAD\r\n");
        break;
       
      // ^U: Update Display with IOT info:
      case 0x5976b8UL:
        display_iot_flag = true;
        break;
      
      //--------------------------------------
      //      Direct Movement Commands
      //--------------------------------------
      case 0xed37f29UL: // ^Stop
        LEFT_SPEED  = 0;
        RIGHT_SPEED = 0;
        break;
      case 0xe95615feUL: // ^TurnR
        LEFT_SPEED  =  WHEEL_PERIOD/8;
        RIGHT_SPEED = -WHEEL_PERIOD/8;
        break;
      case 0xe95615f8UL: // ^TurnL
        LEFT_SPEED  = -WHEEL_PERIOD/8;
        RIGHT_SPEED =  WHEEL_PERIOD/8;
        break;   
      case 0xe8d43000UL: // ^MoveF
        LEFT_SPEED  =  WHEEL_PERIOD/4;
        RIGHT_SPEED =  WHEEL_PERIOD/4;
        break; 
      case 0xe8d42ffcUL: // ^MoveB
        LEFT_SPEED  = -WHEEL_PERIOD/4;
        RIGHT_SPEED = -WHEEL_PERIOD/4;
        break;
        
      //case 0x0: //^Intercept
      //  display_line[0]
      //  break;
        
      //case 0x0: //^Exit
      //  (display_line[0]);
      //  break;
        
      default: write_buffer(&pc_tx_buffer, "COMMAND NOT FOUND\r\n");
    } 
 
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