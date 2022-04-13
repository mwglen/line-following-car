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
      
      // Parse Direction
      switch (cmd[5]) {
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
      for (int i = 6; isdigit(cmd[i]); i++)
          stop_after_time = stop_after_time * 10 + cmd[i] - '0';
      
      // Set flag signaling to stop after time
      stop_after_curr_time = 0;
      stop_after_flag = true;
      
    // Return Error
    } else write_buffer(&pc_tx_buffer, "COMMAND NOT FOUND\r\n");
  
  // Passthrough to iot
  } else write_buffer(&iot_tx_buffer, cmd);
}