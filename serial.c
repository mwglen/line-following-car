/// Includes
#include "serial.h"
#include "msp430.h"

/// Globals
char recieved_message[10] = "No Message";
char transmission[10];
short unsigned int recieve_index = 0;
short unsigned int TX_index = 0;
bool send_transmission = false;
bool message_recieved = false;

// Project 9
bool UCA1_connected = false;

// Common Baud Rates:
//      460,800: (1, 0x4A11)
//      115,200: (4, 0x5551)
//      9,600:   (, 0x)
/// Functions
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
   char temp_char;
   switch(__even_in_range(UCA0IV,0x08)){
      case 0: // Vector 0 - no interrupt
         break;
      case 2: // Vector 2 – RXIFG
         // Recieved character in RXBUF
         temp_char = UCA0RXBUF;
         if (temp_char == '\0') break;
         else if (temp_char == '\n') break;
         else if (temp_char == '\r') break;
         else {
           recieved_message[recieve_index] = temp_char;
           recieve_index++;
         }
         if (recieve_index == 10) {
           message_recieved = true;
           recieve_index = 0;
         }
         break;

      case 4: // Vector 4 – TXIFG
         switch(TX_index) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
               UCA0TXBUF = transmission[TX_index];
               TX_index++;
               break;

            case 10: 
               // Send New Line
               UCA0TXBUF = '\r';
               TX_index++;
               break;

            case 11: 
               // Send Carriage Return
               UCA0TXBUF = '\n';
               TX_index = 0;

               // Disable transmit interrupt
               UCA0IE &= ~UCTXIE;
               break;
         } break;
      default: break;
   }
}

// Initialize UCA0
void Init_Serial_UCA0(int brw, int mctlw) {
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

/// Functions
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
   char temp_char;
   switch(__even_in_range(UCA1IV,0x08)){
      case 0: // Vector 0 - no interrupt
         break;
      case 2: // Vector 2 – RXIFG
         // Recieved character in RXBUF
         temp_char = UCA1RXBUF;
         UCA1_connected = true;
         if (temp_char == '\0') break;
         else if (temp_char == '\n') break;
         else if (temp_char == '\r') break;
         else {
           recieved_message[recieve_index] = temp_char;
           recieve_index++;
         }
         if (recieve_index == 10) {
           message_recieved = true;
           recieve_index = 0;
         }
         break;

      case 4: // Vector 4 – TXIFG
        if (UCA1_connected) {
          switch(TX_index) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
               UCA1TXBUF = transmission[TX_index];
               TX_index++;
               break;

            case 10: 
               // Send New Line
               UCA1TXBUF = '\r';
               TX_index++;
               break;

            case 11: 
               // Send Carriage Return
               UCA1TXBUF = '\n';
               TX_index = 0;

               // Disable transmit interrupt
               UCA1IE &= ~UCTXIE;
               break;
          } break;
        }
      default: break;
   }
}

// Initialize UCA0
void Init_Serial_UCA1(int brw, int mctlw) {
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

