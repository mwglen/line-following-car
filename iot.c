/// Includes
#include "iot.h"
#include "primitives.h"
#include "msp430.h"
#include "display.h"
#include "computer.h"
#include <string.h>
#include <stdio.h>

/// Globals
char iot_trans[20];
char iot_res[100] = "";
short unsigned int iot_tx_idx = 0;
bool iot_send_nl = false;

// Display Strings
char ssid[20] = "";
char ip_addr[20] = "";
char ip_addr1[10] = "";
char ip_addr2[10] = "";

/// Functions
// Transmit message to iot
void transmit_iot(char *message) {
  strcpy(iot_trans, message);
  UCA1IE |= UCTXIE;
}

bool starts_with(char *src1, char *src2) {
  if (strncmp(src1, src2, strlen(src2)) == 0) return true;
  else return false;
}

void parse_iot_res(void) {
  // +CWJAP:"my_ssid"
  if (starts_with(iot_res, "+CWJAP:\"")) {
    strcpy(ssid, "");
    char *c = iot_res + 8;
    while (c[0] != '"') strncat(ssid, c++, 1);
    transmit_pc(ssid);
  }
  
  // +CIFSR:APIP,"192.168.4.1"
  if (starts_with(iot_res, "+CIFSR:APIP,\"")) {
    strcpy(ip_addr1, "");
    strcpy(ip_addr2, "");
    
    // Add the first IP Mask
    char *c = iot_res + 13;
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
    transmit_pc(ip_addr);
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
         UCA1TXBUF = temp_char;
         
         // Read IOT Responses
         if (temp_char == '+') {
           strcpy(iot_res, "+");
         } else if (temp_char == '\n') {
           parse_iot_res();
           strcpy(iot_res,"");
         } else {
           strncat(iot_res, &temp_char, 1);
         };
         break;

      case 4: // Vector 4 – TXIFG
        if (iot_send_nl) {
          UCA0TXBUF = '\n';
          iot_send_nl = false;
          UCA0IE &= ~UCTXIE;
          iot_tx_idx = 0;
        } else if (iot_trans[iot_tx_idx] == '\0') {
          UCA0TXBUF = '\r';
          iot_send_nl = true;
        } else {
          UCA0TXBUF = iot_trans[iot_tx_idx];
          iot_tx_idx++;
        } break;
        
      default: break;
   }
}

// Initialize UCA0
void init_iot(int brw, int mctlw) {
   // Configure UART 0
   UCA0CTLW0 = 0; // Use word register
   UCA0CTLW0 |= UCSWRST; // Set Software reset enable
   UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK

   UCA0BRW = brw; // 9,600 Baud
   UCA0MCTLW = mctlw ;
   UCA0CTLW0 &= ~UCSWRST; // Set Software reset enable
   UCA0IE |= UCRXIE; // Enable RX interrupt

   // Disable TX interrupt
   UCA0IE &= ~UCTXIE;

   // Send null character to set interrupt flag
   // (calls ISR immediately when enabled due to flag)
   UCA0TXBUF = '\0';
}



void center_cpy(char *dst, char *src) {
  if (strlen(src) < 10) {
    sprintf(dst, "---%*s%*s---\n", 
      5+(int)strlen(src)/2, src, 5-(int)strlen(src)/2,"");
  } else strncpy(dst, src, 10);
}

void display_iot(void) {
  // Update Info
  iot_transmit("AT+CWJAP?\r\n");
  iot_transmit("AT+CIFSR\r\n");
  
  // Display Info
  center_cpy(display_line[0], ssid);
  center_cpy(display_line[1], "IP address");
  center_cpy(display_line[2], ip_addr1);
  center_cpy(display_line[3], ip_addr2);
  display_changed = true;
}