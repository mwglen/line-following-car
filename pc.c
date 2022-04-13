/// Includes
#include "msp430.h"
#include "timersB0.h"
#include "iot.h"
#include "pc.h"
#include "text.h"
#include "ring_buffer.h"
#include "commands.h"
#include <string.h>
#include <stdbool.h>

/// Globals
bool pc_connected = true;
char pc_trans[RING_MSG_LENGTH] = "";
char cmd[RING_MSG_LENGTH] = "";
unsigned int pc_count = 0;

// A buffer to hold transmissions to send
RingBuffer pc_tx_buffer = {
  .write_idx = 0,
  .read_idx = 0,
  .curr_size = 0
};

// A buffer to hold recieved transmissions
RingBuffer pc_rx_buffer = {
  .write_idx = 0,
  .read_idx = 0,
  .curr_size = 0
};

/// Functions
// Transmit message to computer
void transmit_pc(char *message) {
  if (pc_connected) {
    strncpy(pc_trans, message, RING_MSG_LENGTH - 1);
    UCA1IE |= UCTXIE;
  }
}

char pc_tx_msg[RING_MSG_LENGTH] = "";
char pc_rx_msg[RING_MSG_LENGTH] = "";
void pc_process(void) {
  if (pc_process_flag) {
    // Update Flag
    pc_process_flag = false;
    
    // Recieve if Needed
    if (pc_rx_buffer.curr_size != 0) {
      read_buffer(&pc_rx_buffer, pc_rx_msg);
      run_cmd(pc_rx_msg);
    }
    
    // Transmit if Needed
    else if (pc_tx_buffer.curr_size != 0) {
      read_buffer(&pc_tx_buffer, pc_tx_msg);
      transmit_pc(pc_tx_msg);
    }
  }
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

unsigned int pc_tx_idx = 0;
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
   char temp_char;
   switch(__even_in_range(UCA1IV,0x08)){
      case 0: // Vector 0 - no interrupt
         break;
      case 2: // Vector 2 – RXIFG
         pc_connected = true;
         temp_char = UCA1RXBUF;
         
         // Ignore null character
         if (temp_char == '\0') break;
         
         // Append character to string
         if (pc_count < RING_MSG_LENGTH-2) {
           strncat(cmd, &temp_char, 1);
           pc_count++;
         }
         
         // Echo character
         UCA1TXBUF = temp_char;
           
         // Send commands to buffer
         if (temp_char == '\n') {
           write_buffer(&pc_rx_buffer, cmd);
           strcpy(cmd, "");
           pc_count = 0;
         } break;

      case 4: // Vector 4 – TXIFG
        temp_char = pc_trans[pc_tx_idx++];
        if (temp_char == '\0') {
          UCA1IE &= ~UCTXIE;
          pc_tx_idx = 0;
        } else UCA1TXBUF = temp_char;
        break;
      default: break;
   }
}