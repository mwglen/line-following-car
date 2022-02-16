//------------------------------------------------------------------------------
//
//  Description: This file contains functions to initialize ports
//
//  Matthew Glen
//  Feb 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

#include "macros.h"
#include "msp430.h"
#include "functions.h"

//------------------------------------------------------------------------------
//
// Description: This calls the rest of the initialization functions
//
//------------------------------------------------------------------------------
void Init_Ports(void) {
    Init_Port1();
    Init_Port2();
    Init_Port3('a');
    Init_Port4();
    Init_Port5();
    Init_Port6();
}

//------------------------------------------------------------------------------
//
// Description: This function will initialize all pins in port 1
//
//------------------------------------------------------------------------------
void Init_Port1(void) {
    P1OUT = RESET_STATE;      // P1 set Low
    P1DIR = RESET_STATE;      // Set P1 direction to input

    // P1 PIN 0
    P1SEL1 &= ~RED_LED;             // GPIO operation
    P1SEL0 &= ~RED_LED;             // GPIO operation
    P1DIR  |=  RED_LED;             // Output
    P1OUT  &= ~RED_LED;             // Off [Low]  ***OFF IS HIGH***

    // P1 PIN 1
    P1SEL1 |=  A1_SEEED;            // ADC Function
    P1SEL0 |=  A1_SEEED;            // ADC Function

    // P1 PIN 2
    P1SEL1 |=  V_DETECT_L;          // ADC Function
    P1SEL0 |=  V_DETECT_L;          // ADC Function

    // P1 PIN 3
    P1SEL1 |=  V_DETECT_R;          // ADC Function
    P1SEL0 |=  V_DETECT_R;          // ADC Function

    // P1 PIN 4
    P1SEL1 &= ~SMCLK_2476;          // GPIO operation
    P1SEL0 &= ~SMCLK_2476;          // GPIO operation
    P1DIR  &= ~SMCLK_2476;          // Input

    // P1 PIN 5
    P1SEL1 |= V_THUMB;              // ADC Function
    P1SEL0 |= V_THUMB;              // ADC Function

    // P1 PIN 6
    P1SEL1 &= ~UCA0RXD;             // UCA0RXD Function
    P1SEL0 |=  UCA0RXD;             // UCA0RXD Function
        
    // P1 PIN 7
    P1SEL1 &= ~UCA0TXD;             // UCA0TXD Function
    P1SEL0 |=  UCA0TXD;             // UCA0TXD Function
}

//------------------------------------------------------------------------------
//
// Description: This function will initialize all pins in port 2
//
//------------------------------------------------------------------------------
void Init_Port2(void) {
    P2OUT = RESET_STATE;            // P2 set Low
    P2DIR = RESET_STATE;            // Set P2 direction to input

    // P3 PIN 0
    P2SEL1 &= ~RESET_LCD;           // GPIO operation
    P2SEL0 &= ~RESET_LCD;           // GPIO operation
    P2DIR |= RESET_LCD;             // Output
    P2OUT &= ~RESET_LCD;            // Off [Low]

    // P3 PIN 1
    P2SEL1 &= ~L_REVERSE_2476;      // GPIO operation
    P2SEL0 &= ~L_REVERSE_2476;      // GPIO operation
    P2DIR  &= ~L_REVERSE_2476;      // Input

    // P3 PIN 2
    P2SEL1 &= ~P2_2;                // GPIO operation
    P2SEL0 &= ~P2_2;                // GPIO operation
    P2DIR  &= ~P2_2;                // Input

    // P3 PIN 3
    P2SEL1 &= ~SW2;                 // GPIO Operation
    P2SEL0 &= ~SW2;                 // GPIO Operation
    P2OUT  |=  SW2;                 // Configure pullup resistor
    P2DIR  &= ~SW2;                 // Input
    P2REN  |=  SW2;                 // Enable pullup resistor
    //P2IES |=  SW2;                  // P2.0 Hi/Lo edge interrupt
    //P2IFG &= ~SW2;                  // Clear all P2.6 interrupt flags
    //P2IE  |=  SW2;                  // P2.6 interrupt enabled

    // P3 PIN 4
    P2SEL1 &= ~IOT_RUN_CPU;         // GPIO operation
    P2SEL0 &= ~IOT_RUN_CPU;         // GPIO operation
    P2DIR  |=  IOT_RUN_CPU;         // Input
    P2OUT  &= ~IOT_RUN_CPU;         // Off [Low]

    // P3 PIN 5
    P2SEL1 &= ~DAC_ENB;             // GPIO operation
    P2SEL0 &= ~DAC_ENB;             // GPIO operation
    P2DIR  |=  DAC_ENB;             // Output
    P2OUT  |=  DAC_ENB;             // On [High]

    // P3 PIN 6
    P2SEL1 |=  LFXOUT;              // Clock operation
    P2SEL0 &= ~LFXOUT;              // Clock operation

    // P3 PIN 7
    P2SEL1 |=  LFXIN;               // Clock operation
    P2SEL0 &= ~LFXIN;               // Clock operation
}

//------------------------------------------------------------------------------
//
// Description: This function will initialize all pins in port 3
//
//------------------------------------------------------------------------------
void Init_Port3(char msclk) {
    P3OUT = RESET_STATE;            // P3 set Low
    P3DIR = RESET_STATE;            // Set P3 direction to input
    
    // P3 PIN 0
    P3SEL1 &= ~TEST_PROBE;          // GPIO operation
    P3SEL0 &= ~TEST_PROBE;          // GPIO operation
    P3DIR  |=  TEST_PROBE;          // Output
    P3OUT  &= ~TEST_PROBE;          // Off [Low]

    // P3 PIN 1
    P3SEL1 &= ~DAC_CNTL1;           // GPIO operation
    P3SEL0 &= ~DAC_CNTL1;           // GPIO operation
    P3DIR  |=  DAC_CNTL1;           // Output
    P3OUT  &= ~DAC_CNTL1;           // Off [Low]

    // P3 PIN 2
    P3SEL1 |=  OA2N;                // SMCLK operation
    P3SEL0 |=  OA2N;                // SMCLK operation

    // P3 PIN 3
    P3SEL1 &= ~LCD_BACKLITE;        // GPIO operation
    P3SEL0 &= ~LCD_BACKLITE;        // GPIO operation
    P3DIR  |=  LCD_BACKLITE;        // Output
    P3OUT  |=  LCD_BACKLITE;        // On [High]

    // P3 PIN 4
    P3SEL1 &= ~SMCLK_2355;          // SMCLK operation
    P3SEL0 |=  SMCLK_2355;          // SMCLK operation
    P3DIR  |=  SMCLK_2355;          // Direction must be High

    // P3 PIN 5
    P3SEL1 &= ~DAC_CNTL;            // GPIO operation
    P3SEL0 &= ~DAC_CNTL;            // GPIO operation
    P3DIR  |=  DAC_CNTL;            // Output
    P3OUT  &= ~DAC_CNTL;            // Off [Low]

    // P3 PIN 6
    P3SEL1 &= ~IOT_LINK_CPU;        // GPIO operation
    P3SEL0 &= ~IOT_LINK_CPU;        // GPIO operation
    P3DIR  &= ~IOT_LINK_CPU;        // Input

    // P3 PIN 7
    P3SEL1 &= ~IOT_EN_CPU;          // GPIO opertaion
    P3SEL0 &= ~IOT_EN_CPU;          // GPIO operation
    P3DIR  |=  IOT_EN_CPU;          // Output
    P3OUT  &= ~IOT_EN_CPU;          // Off [Low]
}

//------------------------------------------------------------------------------
//
// Description: This function will initialize all pins in port 4
//
//------------------------------------------------------------------------------
void Init_Port4(void) {
    P4OUT = RESET_STATE;            // P4 set Low
    P4DIR = RESET_STATE;            // Output

    // P4 PIN 0
    P4SEL1 &= ~P4_0;                // GPIO operation
    P4SEL0 &= ~P4_0;                // GPIO operation
    P4DIR  &= ~P4_0;                // Input
    
    // P4 PIN 1
    P4SEL1 &= ~SW1;                 // GPIO operation
    P4SEL0 &= ~SW1;                 // GPIO operation
    P4OUT  |=  SW1;                 // Configure pullup resistor
    P4DIR  &= ~SW1;                 // Input
    P4REN  |=  SW1;                 // Enable pullup resistor
    // P4IES |= SW1;                  // P4.2 Hi/Lo edge interrupt
    // P4IFG &= ~SW1;                 // Clear all P2.6 interrupt flags
    // P4IE |= SW1;                   // P4.2 interrupt enabled
    
    // P4 PIN 2
    P4SEL1 &= ~UCA1TXD;             // UART operation
    P4SEL0 |=  UCA1TXD;             // UART operation
    
    // P4 PIN 3
    P4SEL1 &= ~UCA1RXD;             // UART operation
    P4SEL0 |=  UCA1RXD;             // UART operation
    
    // P4 PIN 4
    P4SEL1 &= ~UCB1_CS_LCD;         // GPIO operation
    P4SEL0 &= ~UCB1_CS_LCD;         // GPIO operation
    P4DIR  |=  UCB1_CS_LCD;         // Output
    P4OUT  |=  UCB1_CS_LCD;         // Off [High]
    
    // P4 PIN 5
    P4SEL1 &= ~UCB1CLK;             // UCB1CLK SPI BUS operation
    P4SEL0 |=  UCB1CLK;             // UCB1CLK SPI BUS operation
    
    // P4 PIN 6
    P4SEL1 &= ~UCB1SIMO;            // UCB1SIMO SPI BUS operation
    P4SEL0 |=  UCB1SIMO;            // UCB1SIMO SPI BUS operation
    
    // P4 PIN 7
    P4SEL1 &= ~UCB1SOMI;            // UCB1SOMI SPI BUS operation
    P4SEL0 |=  UCB1SOMI;            // UCB1SOMI SPI BUS operation
}

//------------------------------------------------------------------------------
//
// Description: This function will initialize all pins in port 5
//
//------------------------------------------------------------------------------
void Init_Port5(void) {
    P5OUT = RESET_STATE;            // P4 set Low
    P5DIR = RESET_STATE;            // Set P4 direction to output

    // P5 PIN 0
    P5SEL1 |= CHECK_BAT;            // Analog Function
    P5SEL0 |= CHECK_BAT;            // Analog Function
    
    // P5 PIN 1
    P5SEL1 |= V_BAT;                // Analog Function
    P5SEL0 |= V_BAT;                // Analog Function
    
    // P5 PIN 2
    P5SEL1 |=  V_DAC;               // Analog Function
    P5SEL0 |=  V_DAC;               // Analog Function
    
    // P5 PIN 3
    P5SEL1 |=  V_3_3;               // Analog Function
    P5SEL0 |=  V_3_3;               // Analog Function
    
    // P5 PIN 4
    P5SEL1 &= ~IOT_BOOT_CPU;        // GPIO operation
    P5SEL0 &= ~IOT_BOOT_CPU;        // GPIO operation
    P6DIR  |=  IOT_BOOT_CPU;        // Output
    P4OUT  &= ~IOT_BOOT_CPU;        // Off [Low]
}

//------------------------------------------------------------------------------
//
// Description: This function will initialize all pins in port 6
//
//------------------------------------------------------------------------------
void Init_Port6(void) {
    P6OUT = RESET_STATE;            // P4 set Low
    P6DIR = RESET_STATE;            // Set P4 direction to output
    
    // P6 PIN 0
    P6SEL1 &= ~R_FORWARD;           // GPIO operation
    P6SEL0 &= ~R_FORWARD;           // GPIO operation
    P6DIR  |=  R_FORWARD;           // Output
    P6OUT  &= ~R_FORWARD;           // Off [Low]
    
    // P6 PIN 1
    P6SEL1 &= ~L_FORWARD;           // GPIO operation
    P6SEL0 &= ~L_FORWARD;           // GPIO operation
    P6DIR  |=  L_FORWARD;           // Output
    P6OUT  &= ~L_FORWARD;           // Off [Low]

    // P6 PIN 2
    P6SEL1 &= ~R_REVERSE;           // GPIO operation
    P6SEL0 &= ~R_REVERSE;           // GPIO operation
    P6DIR  |=  R_REVERSE;           // Output
    P6OUT  &= ~R_REVERSE;           // Off [Low]
    
    // P6 PIN 3
    P6SEL1 &= ~L_REVERSE_2355;      // GPIO operation
    P6SEL0 &= ~L_REVERSE_2355;      // GPIO operation
    P6DIR  |=  L_REVERSE_2355;      // Output
    P6OUT  &= ~L_REVERSE_2355;      // Off [Low]
    
    // P6 PIN 4
    P6SEL1 &= ~IR_SENSOR;           // GPIO operation
    P6SEL0 &= ~IR_SENSOR;           // GPIO operation
    P6DIR  |=  R_REVERSE;           // Input
    
    // P6 PIN 5
    P6SEL1 &= ~P6_5;                // GPIO operation
    P6SEL0 &= ~P6_5;                // GPIO operation
    P6DIR  |=  P6_5;                // Input
    
    // P6 PIN 6
    P6SEL1 &= ~GRN_LED;             // GPIO operation
    P6SEL0 &= ~GRN_LED;             // GPIO operation
    P6DIR  |=  GRN_LED;             // Output
    P6OUT  |=  GRN_LED;             // Off [High]
}
