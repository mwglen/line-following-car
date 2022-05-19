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

/// Globals
bool circle_cmd_recieved = false;
bool exit_cmd_recieved = false;
bool inc_cmd_recieved = false;
bool cmd_recieved = false;

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
    // Announce that a command was read
    cmd_recieved = true;
    
    // Add Characters for Command
    int i = 0;
    while ((cmd[i+1] != '\0') && (i < 6)) {
      display_line[3][i] = cmd[i+1]; 
      i++;
    }
    while (i < 6) display_line[3][i++] = ' ';
    display_changed = true;
    
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
        LEFT_SPEED  =  WHEEL_PERIOD/2;
        RIGHT_SPEED =  WHEEL_PERIOD/2;
        break; 
      case 0xe8d42ffcUL: // ^MoveB
        LEFT_SPEED  = -WHEEL_PERIOD/2;
        RIGHT_SPEED = -WHEEL_PERIOD/2;
        break;
        
      //--------------------------------------
      //      Other Commands
      //--------------------------------------
      case 0x7c91e03dUL: // ^Inc
        inc_cmd_recieved = true;
        break;
        
      //--------------------------------------
      //      Circle Movement Commands
      //--------------------------------------
      case 0xeb97d1f5UL: // ^Circle
        circle_cmd_recieved = true;
        break;
      case 0xecbe21dUL: // ^Exit
         exit_cmd_recieved = true;
        break;
        
      default: write_buffer(&pc_tx_buffer, "COMMAND NOT FOUND\r\n");
    } 
 
  // Passthrough to iot
  } else write_buffer(&iot_tx_buffer, cmd);
}