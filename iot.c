/// Includes
#include "iot.h"
#include "msp430.h"
#include "display.h"
#include "pc.h"
#include "text.h"
#include "ring_buffer.h"
#include "timersB0.h"
#include "commands.h"
#include <stdlib.h>
#include <string.h>

/// Globals
char iot_trans[RING_MSG_LENGTH];
char iot_res[RING_MSG_LENGTH] = "";
short unsigned int iot_tx_idx = 0;
bool display_iot_now = false;

// Network Strings
char ssid[20] = "";
char ip_addr[20] = "";
char ip_addr1[10] = "";
char ip_addr2[10] = "";

// TCP Strings
char tcp_msg[10] = "";
unsigned int tcp_msg_src;
unsigned int tcp_num_chr;

// A buffer to hold recieved iot messages
RingBuffer iot_rx_buffer = {
  .write_idx = 0,
  .read_idx = 0,
  .curr_size = 0
};

// A buffer to hold iot transmissions
RingBuffer iot_tx_buffer = {
  .write_idx = 0,
  .read_idx = 0,
  .curr_size = 0
};

/// Functions
// Transmit message to iot
void transmit_iot(char *message) {
  strcpy(iot_trans, message);
  if (iot_trans[iot_tx_idx] != '\0') {
    UCA0TXBUF = iot_trans[iot_tx_idx++];
    UCA0IE |= UCTXIE;
  }
}

void parse_iot_res(char *res) {
  // +CWJAP:"my_ssid"
  if (starts_with(res, "+CWJAP:\"")) {
    strcpy(ssid, "");
    char *c = res + 8;
    while (c[0] != '"') strncat(ssid, c++, 1);
    recieved_ssid = true;
  }
  
  // +IPD,0,13:Hello World/r/n
  else if (starts_with(res, "+IPD,")) {
    char msg_src_str[10] = "";
    char num_chr_str[10] = "";
    char cmd[10] = "";
    strcpy(tcp_msg, "");

    char *c = res + 5;
    while (*c != ',') strncat(msg_src_str, c++, 1); c++; // Source of Message
    while (*c != ':') strncat(num_chr_str, c++, 1); c++; // Number of Characters
    tcp_msg_src = atoi(msg_src_str);
    tcp_num_chr = atoi(num_chr_str);
    char *k = c; // Save current index
    while (c != k + tcp_num_chr) strncat(cmd, c++, 1);
    run_cmd(cmd);
    strcpy(cmd, "");
  }
  
  // +CIFSR:APIP,"192.168.4.1"
  else if (starts_with(res, "+CIFSR:STAIP,\"")) {
    strcpy(ip_addr1, "");
    strcpy(ip_addr2, "");
    
    // Add the first IP Mask
    char *c = res + 14;
    while (c[0] != '.') strncat(ip_addr1, c++, 1);
    
    // Add the Period
    strncat(ip_addr1, c++, 1);
    
    // Add the second IP Mask
    while (c[0] != '.') strncat(ip_addr1, c++, 1);
      
    // Add the third IP Mask
    while (c[0] != '.') strncat(ip_addr2, c++, 1);
      
    // Add the Period
    strncat(ip_addr2, c++, 1);
    
    // Add the fourth IP Mask
    while (c[0] != '"') strncat(ip_addr2, c++, 1);
    
    strcpy(ip_addr, ip_addr1);
    strcat(ip_addr, ip_addr2);
    recieved_ip = true;
    
  } else if (starts_with(res, "WIFI GOT IP")) {
    display_iot_flag = true;
  }
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
   char temp_char;
   switch(__even_in_range(UCA0IV,0x08)){
      case 0: // Vector 0 - no interrupt
         break;
      case 2: // Vector 2 – RXIFG
         // Recieved character in RXBUF
         temp_char = UCA0RXBUF;
         
         // Ignore null characters (just in case)
         if (temp_char == '\0') break;
         
         // Echo character to pc
         UCA1TXBUF = temp_char;
         
         // Ignore null characters (just in case)
         if (temp_char == '\0') break;
         
         // Add Character to Response
         strncat(iot_res, &temp_char, 1);

         // Read IOT Responses
         if (temp_char == '\n') {
           write_buffer(&iot_rx_buffer, iot_res);
           strcpy(iot_res, "");
         } break;

      case 4: // Vector 4 – TXIFG
        temp_char = iot_trans[iot_tx_idx++];
        if (temp_char == '\0') {
          UCA0IE &= ~UCTXIE;
          iot_tx_idx = 0;
        } else UCA0TXBUF = temp_char;
        break;
        
      default: break;
   }   
}

// Initialize UCA0
void init_iot(int brw, int mctlw) {
   // Configure UART 0
   UCA0CTLW0 = 0;               // Use word register
   UCA0CTLW0 |= UCSWRST;        // Set Software reset enable
   UCA0CTLW0 |= UCSSEL__SMCLK;  // Set SMCLK as fBRCLK
   
   UCA0BRW = brw;
   UCA0MCTLW = mctlw ;
   UCA0CTLW0 &= ~UCSWRST;       // Set Software reset enable
   UCA0IE |= UCRXIE;            // Enable RX interrupt

   // Disable TX interrupt
   UCA0IE &= ~UCTXIE;

   // Send null character to set interrupt flag
   // (calls ISR immediately when enabled due to flag)
   UCA0TXBUF = '\0';
}

char iot_tx_msg[RING_MSG_LENGTH] = "";
char iot_rx_msg[RING_MSG_LENGTH] = "";

void iot_process(void) {
  if (iot_process_flag) {
    // Update Flag
    iot_process_flag = false;
    
    // Recieve if Needed
    if (iot_rx_buffer.curr_size != 0) {
      read_buffer(&iot_rx_buffer, iot_rx_msg);
      parse_iot_res(iot_rx_msg);
    }
    
    // Transmit if Needed
    else if (iot_tx_buffer.curr_size != 0) {
      read_buffer(&iot_tx_buffer, iot_tx_msg);
      transmit_iot(iot_tx_msg);
    }
    
    // Display IOT Info if Needed
    if (display_iot_flag) display_iot();
  }
}