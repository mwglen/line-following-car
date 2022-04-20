/// Includes
#include "iot.h"
#include "display_iot.h"
#include "display.h"
#include "text.h"
#include "ring_buffer.h"

/// Globals
unsigned short int display_iot_state = 0;
bool recieved_ip = false;
bool recieved_ssid = false;

/// Functions
void display_iot(void) {
  switch (display_iot_state++) {
    // Get SSID
    case 0: write_buffer(&iot_tx_buffer, "AT+CWJAP?\r\n");
    
    // Get IP Address
    case 1: write_buffer(&iot_tx_buffer, "AT+CIFSR\r\n");
      
    // Enable Multiple Connections
    case 2: write_buffer(&iot_tx_buffer, "AT+CIPMUX=1\r\n");
    
    // Start Server
    case 3: write_buffer(&iot_tx_buffer, "AT+CIPSERVER=1,8480\r\n");
    
    // Display Data
    case 4:
      display_iot_state = 4;
      if (recieved_ip && recieved_ssid) {
        center_cpy(display_line[0], ssid);
        center_cpy(display_line[1], "IP address");
        center_cpy(display_line[2], ip_addr1);
        center_cpy(display_line[3], ip_addr2);
        display_changed = true;
        
        // Reset State
        display_iot_state = 0;
        display_iot_flag = false;
        recieved_ip = false;
        recieved_ssid = false;
      }
  }
}
