/// Includes
#include "msp430.h"
#include "primitives.h"
#include "timersB0.h"
#include "iot.h"
#include <string.h>

/// Globals
bool pc_connected = true;
char pc_trans[20] = "";

// Store Commands
char cmd[10] = "";

/// Functions
// Transmit message to computer
void transmit_pc(char *message) {
  if (pc_connected) {
    strcpy(pc_trans, message);
    UCA1IE |= UCTXIE;
  }
}

void run_cmd(void) {
   if (strcmp(cmd, "^^") == 0) {
     transmit_pc("PC READY");
   } else if (strcmp(cmd, "^F") == 0) {
     init_iot(4, 0x5551); //115200 
     transmit_pc("FAST BUAD");
   } else if (strcmp(cmd, "^S") == 0) {
     init_iot(52, 0x4911); //9600 
     transmit_pc("SLOW BUAD");
   }
   strcpy(cmd, "");
}

// Initialize UCA0
void init_pc(int brw, int mctlw) {
   // Configure UART 0
   UCA1CTLW0 = 0; // Use word register
   UCA1CTLW0 |= UCSWRST; // Set Software reset enable
   UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK

   UCA1BRW = brw;
   UCA1MCTLW = mctlw ;
   UCA1CTLW0 &= ~UCSWRST; // Set Software reset enable
   UCA1IE |= UCRXIE; // Enable RX interrupt

   // Disable TX interrupt
   UCA1IE &= ~UCTXIE;

   // Send null character to set interrupt flag
   // (calls ISR immediately when enabled due to flag)
   UCA1TXBUF = '\0';
}

//const unsigned long hash(const char *str) {
//  unsigned long hash = 5381;
//  int c;
//  while ((c = *str++))
//    hash = ((hash << 5) + hash) + c;
//  return hash;
//}

bool send_nl = false;
short unsigned int pc_tx_idx = 0;
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
   char temp_char;
   switch(__even_in_range(UCA1IV,0x08)){
      case 0: // Vector 0 - no interrupt
         break;
      case 2: // Vector 2 – RXIFG
         pc_connected = true;
         temp_char = UCA1RXBUF;
         
         // Run command
         if ((cmd[0] == '^') && (temp_char == '\r')) run_cmd();
         
         // Add character to string
         else if (cmd[0] == '^') strncat(cmd, &temp_char, 1);
         
         // Start capturing command
         else if (temp_char == '^') strcpy(cmd, "^");
         
         // Passthrough to IOT
         else UCA0TXBUF = temp_char;
         break;

      case 4: // Vector 4 – TXIFG
        if (send_nl) {
          UCA1TXBUF = '\n';
          send_nl = false;
          UCA1IE &= ~UCTXIE;
          pc_tx_idx = 0;
        } else if (pc_trans[pc_tx_idx] == '\0') {
          UCA1TXBUF = '\r';
          send_nl = true;
        } else {
          UCA1TXBUF = pc_trans[pc_tx_idx];
          pc_tx_idx++;
        } break;
        
      default: break;
   }
}