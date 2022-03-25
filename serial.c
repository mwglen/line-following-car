/// Includes
#include "serial.h"
#include "msp430.h"

/// Globals
char recieved_message[20];
short unsigned int recieved_index = 0;
short unsigned int TX_Index = 0;
bool send_transmission = false;

/// Functions
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
   unsigned int temp;
   switch(__even_in_range(UCA0IV,0x08)){
      case 0: // Vector 0 - no interrupt
         break;
      case 2: // Vector 2 – RXIFG
         // Recieved character in RXBUF
         temp_char = RXBUF;
         if (temp_char == '\0');
         else recieved_message[recieve_index] = temp_char;
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
               TXBUF = transmission[TX_index];
               TX_index++;
               break;

            case 10: 
               // Send New Line
               TXBUF = '\n';
               TX_index++;
               break;

            case 11: 
               // Send Carriage Return
               TXBUF = '\r';
               TX_index = 0;

               // Disable transmit interrupt
               UCA0IFT &= ~UCTXIFG
               break;
         } break;
      default: break;
   }
}

// Initialize UCA0
void Init_Serial_UCA0(int mctlw, int num2) {
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
   UCA0BRW = 52; // 9,600 Baud
   // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
   // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
   UCA0MCTLW = 0x4911 ;
   UCA0CTLW0 &= ~UCSWRST; // Set Software reset enable
   UCA0IE |= UCRXIE; // Enable RX interrupt

   // Disable TX interrupt
   UCA0IFG &= ~UCTXIFG;

   // Send null character to set interrupt flag
   // (calls ISR immediately when enabled due to flag)
   UCA0TXBUF = '\0';
}

