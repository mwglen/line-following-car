/// Includes
#include "adc.h"
#include "msp430.h"
#include "ports.h"
#include "primitives.h"

/// Global Variables
short int ADC_CHANNEL;
int THUMB_VALUE;
bool NEW_ADC_VALUES;
int LEFT_IR_VALUE;
int RIGHT_IR_VALUE;

/// Functions
void init_adc(void) {
  // ADCCTL0 Register
  ADCCTL0 = 0;                  // Reset
  ADCCTL0 |= ADCSHT_2;          // 16 ADC clocks
  ADCCTL0 |= ADCMSC;            // MSC
  ADCCTL0 |= ADCON;             // ADC ON
  
  // ADCCTL1 Register
  ADCCTL2 = 0;                  // Reset
  ADCCTL1 |= ADCSHS_0;          // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP;            // ADC sample-and-hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH;          // ADC invert signal sample-and-hold.
  ADCCTL1 |= ADCDIV_0;          // ADC clock divider - 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0;         // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0;       // ADC conversion sequence 00b = Single-channel single-conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process
  
  // ADCCTL2 Register
  ADCCTL2 = 0;                  // Reset
  ADCCTL2 |= ADCPDIV0;          // ADC pre-divider 00b = Pre-divide by 1
  ADCCTL2 |= ADCRES_2;          // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
  ADCCTL2 &= ~ADCDF;            // ADC data read-back format 0b = Binary unsigned.
  ADCCTL2 &= ~ADCSR;            // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
  
  // ADCMCTL0 Register
  ADCMCTL0 |= ADCSREF_0;        // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
  ADCMCTL0 |= ADCINCH_2;        // LEFT_IR (0x20) Pin 2 A2
  ADCIE |= ADCIE0;              // Enable ADC conv complete interrupt
  ADCCTL0 |= ADCENC;            // ADC enable conversion.
  ADCCTL0 |= ADCSC;             // ADC start conversion.
}

// Enable On 5ms Timer
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
  case ADCIV_NONE:
    break;
    
  case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
    // before its previous conversion result was read.
    break;
    
  case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
    break;
    
  case ADCIV_ADCHIIFG: // Window comparator interrupt flags
    break;
    
  case ADCIV_ADCLOIFG: // Window comparator interrupt flag
    break;
    
  case ADCIV_ADCINIFG: // Window comparator interrupt flag
    break;
    
  case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
    ADCCTL0 &= ~ADCENC; // Disable ENC bit.
    
    switch (ADC_CHANNEL++){
    case 0x01:                   // Channel A2 (Left IR)
      ADCMCTL0 &= ~ADCINCH_2;    // Disable Last channel A2
      ADCMCTL0 |= ADCINCH_3;     // Enable Next channel A3
      LEFT_IR_VALUE = ADCMEM0;   // Move result into Global
      LEFT_IR_VALUE =            // Divide the result by 4
        LEFT_IR_VALUE >> 2;
      ADCCTL0 |= ADCENC;         // Enable Conversions
      ADCCTL0 |= ADCSC;          // Start next sample
      break;
      
    case 0x02:                  // Channel A3 (Right IR)
      ADCMCTL0 &= ~ADCINCH_3;   // Disable Last channel A3
      ADCMCTL0 |= ADCINCH_5;    // Enable Next channel A5
      RIGHT_IR_VALUE = ADCMEM0; // Move result into Global
      RIGHT_IR_VALUE =          // Divide the result by 4
        RIGHT_IR_VALUE >> 2;
      ADCCTL0 |= ADCENC;        // Enable Conversions
      ADCCTL0 |= ADCSC;         // Start next sample
      break;
      
    case 0x03:
      ADCMCTL0 &= ~ADCINCH_5;   // Disable Last channel A5
      ADCMCTL0 |= ADCINCH_2;    // Enable Next channel A3
      THUMB_VALUE = ADCMEM0;    // Move result into Global
      THUMB_VALUE =             // Divide the result by 4
        THUMB_VALUE >> 2;
      ADCCTL0 &= ~ADCENC;        // Enable Conversions
      ADCCTL0 &= ~ADCSC;         // Start next sample
      ADC_CHANNEL = 0;
      NEW_ADC_VALUES = true;
      break;
      
    default:
      break;
    }
    break;
    
  default:
    break;
  }
}

// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
void hex_to_bcd(int hex_value){
  int value = 0;
  ADC_CHAR[0] = '0';
  ADC_CHAR[1] = '0';
  ADC_CHAR[2] = '0';
  ADC_CHAR[3] = '0';
  while (hex_value > 999){
    hex_value = hex_value - 1000;
    value = value + 1;
    ADC_CHAR[0] = 0x30 + value;
  }
  value = 0;
  while (hex_value > 99){
    hex_value = hex_value - 100;
    value = value + 1;
    ADC_CHAR[1] = 0x30 + value;
  }
  value = 0;
  while (hex_value > 9){
    hex_value = hex_value - 10;
    value = value + 1;
    ADC_CHAR[2] = 0x30 + value;
  }
  ADC_CHAR[3] = 0x30 + hex_value;
}

void init_ref(void){
  // Configure reference module
  PMMCTL0_H = PMMPW_H; // Unlock the PMM registers
  PMMCTL2 = INTREFEN; // Enable internal reference
  PMMCTL2 |= REFVSEL_2; // Select 2.5V reference
  while(!(PMMCTL2 & REFGENRDY)); // Poll till internal reference settles
  // Using a while statement is not usually 
  // recommended without an exit strategy.
  // This while statement is the suggested 
  // operation to allow the reference to settle.
}