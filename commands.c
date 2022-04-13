#include "commands.h"
#include "pc.h"
#include "iot.h"
#include "text.h"
#include "ring_buffer.h"
#include "timersB0.h"
#include "stdbool.h"
#include "display.h"

// Parse and run commands
void run_cmd(char cmd[RING_MSG_LENGTH]) {
  if (cmd[0] == '^') {
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
      
    // Return Error
    } else {
      write_buffer(&pc_tx_buffer, "COMMAND NOT FOUND");
    }
  } else {
    write_buffer(&iot_tx_buffer, cmd);
  }
}