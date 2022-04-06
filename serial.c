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

   // 9,600 Baud Rate
   // 1. Calculate N = fBRCLK / Baudrate
   //      N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
   // if N > 16 continue with step 3, otherwise with step 2
   // 
   // 2. OS16 = 0, UCBRx = INT(N)
   // continue with step 4
   //
   // 3. OS16 = 1,
   //    UCx = INT(N/16),
   //        = INT(N/16) = 833.333/16 => 52
   //   UCFx = INT([(N/16) – INT(N/16)] × 16)
   //        = ([833.333/16-INT(833.333/16)]*16)
   //        = (52.08333333-52)*16
   //        = 0.083*16 = 1
   //
   // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
   // Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
   //
   // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
   // TX error (%) RX error (%)
   // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos   neg  pos
   // 8000000 9600     1     52    1   0x49 -0.08 0.04 -0.10 0.14
   UCA0BRW = brw; // 9,600 Baud
   // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
   // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
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
      default: break;
   }
}

// Initialize UCA0
void Init_Serial_UCA1(int brw, int mctlw) {
   // Configure UART 0
   UCA1CTLW0 = 0; // Use word register
   UCA1CTLW0 |= UCSWRST; // Set Software reset enable
   UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK

   // 9,600 Baud Rate
   // 1. Calculate N = fBRCLK / Baudrate
   //      N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
   // if N > 16 continue with step 3, otherwise with step 2
   // 
   // 2. OS16 = 0, UCBRx = INT(N)
   // continue with step 4
   //
   // 3. OS16 = 1,
   //    UCx = INT(N/16),
   //        = INT(N/16) = 833.333/16 => 52
   //   UCFx = INT([(N/16) – INT(N/16)] × 16)
   //        = ([833.333/16-INT(833.333/16)]*16)
   //        = (52.08333333-52)*16
   //        = 0.083*16 = 1
   //
   // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
   // Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
   //
   // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
   // TX error (%) RX error (%)
   // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos   neg  pos
   // 8000000 9600     1     52    1   0x49 -0.08 0.04 -0.10 0.14
   UCA1BRW = brw; // 9,600 Baud
   // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
   // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
   UCA1MCTLW = mctlw ;
   UCA1CTLW0 &= ~UCSWRST; // Set Software reset enable
   UCA1IE |= UCRXIE; // Enable RX interrupt

   // Disable TX interrupt
   UCA1IE &= ~UCTXIE;

   // Send null character to set interrupt flag
   // (calls ISR immediately when enabled due to flag)
   UCA1TXBUF = '\0';
}

