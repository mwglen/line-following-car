// ACLK, MCLK, SMCLK, MODOSC
// Maybe set L_REVERSE_2355 to GPIO input
// VLOCLK is 10kHz (can be used in a sleep mode)
// Usually XT1 Clock source is used for ACLK due to its precision
// 3.4 A side - 2355 (Two soldering irons to solder remove frrom solder)
// choose divider between SMCLK and MCLK to dtermine speed of code
void wait(int ms) {

}

// Registers CSCTL0 through CSCTL8 control the clock system
typedef enum {AM, LPM0, LPM3, LPM4, LPM3_5, LPM4_5} PowerMode;
void change_power_mode(new_mode) {
    switch new_mode {
        case AM:
            ACLKREQEN = 0;
            MCLKREQEN = 0;
            SMCLKREQEN = 0;
            SMCLKOFF = 0;
            break;

        case LPM0:
            ACLKREQEN = 0;
            MCLKREQEN = 0;
            SMCLKREQEN = 0;
            SMCLKOFF = 0;
            break;

        case LPM3:
            ACLKREQEN = 0;
            MCLKREQEN = 0;
            SMCLKREQEN = 0;
            SMCLKOFF = 0;
            break;

        case LPM4:
            ACLKREQEN = 0;
            MCLKREQEN = 0;
            SMCLKREQEN = 0;
            SMCLKOFF = 0;
            break;

        case LPM3_5:
            ACLKREQEN = 0;
            MCLKREQEN = 0;
            SMCLKREQEN = 0;
            SMCLKOFF = 0;
            break;

        case LPM4_5:
            ACLKREQEN = 0;
            MCLKREQEN = 0;
            SMCLKREQEN = 0;
            SMCLKOFF = 0;
            break;
    }
}


// Digitally Controlled Oscillator
// Controlled by DCORSEL, DCO, and MOD bits
// Can be stabilized byt the FLL to a multiple frequency of FLLREFCLK/n
// The FLL accepts different reference sources selected by the SELREF bits
// Reference sources include XT1CLK and REFOCLK
// The value of n is defined by the FLLREFDIV bits (n=1, 32, 64, 128, 256, or 512)
void setup_dco() {


}



////////// INTERUPTS ////////////////
// Interrupt vectors are located at 0FFFFh to 0FF80h, for a maximum of 64 interrupts
// A vector is programed by the user and points to the start location of the corresponding ISR
// Interrupts have priority with the highest address getting the most priority
// Three types of interrupts: System Reset, Nonmaskable, Maskable

// Switches on our board are attached to port 4.1 and port 2.3
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
    // Switch 1
    if (P4IFG & SW1) {
        P4IFG &= ~SW1; // IFG SW2 Cleared
        P6OUT &= ~LCD_BACKLIGHT;
    }
}

#pragma vector = {Vector}
__interrupt void switchP4_interrupt(void) {
    // Switch 2
    if (P2IFG & SW2) {
        P2IFG &= ~SW2; // IFG SW2 Cleared
        P6OUT |= LCD_BACKLIGHT;
    }
}



