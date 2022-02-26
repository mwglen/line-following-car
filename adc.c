/// Includes
#include "adc.h"
#include "msp430.h"
#include "ports.h"

/// Global Variables
short int ADC_CHANNEL;

/// Functions
void init_adc(void) {
  //------------------------------------------------------------------------------
  // V_DETECT_L (0x04) // Pin 2 A2
  // V_DETECT_R (0x08) // Pin 3 A3
  // V_THUMB (0x20) // Pin 5 A5
  //------------------------------------------------------------------------------
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
  ADCMCTL0 |= ADCINCH_5;        // V_THUMB (0x20) Pin 5 A5
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
    case 0x00:                      // Channel A2 Interrupt
      // Read in Thumb Wheel Value (in ADMEM0)
      ADCMCTL0 &= ~ADCINCH_5;       // Disable Last channel A5
      ADCMCTL0 |= ADCINCH_3;        // Enable Next channel A3
      ADC_Left_Detect = ADCMEM0;    // Move result into Global
      ADC_Left_Detect =             // Divide the result by 4
        ADC_Left_Detect >> 2;
      ADCCTL0 |= ADCENC; // Enable Conversions
      ADCCTL0 |= ADCSC; // Start next sample
      break;
      
      // Channels Right_detect, left_detect, V_thumb
      
    case 0x01:
      // Replicate for each channel being read
      // Read in Thumb Wheel Value (in ADMEM0)
      ADCMCTL0 &= ~ADCINCH_5;       // Disable Last channel A5
      ADCMCTL0 |= ADCINCH_3;        // Enable Next channel A3
      ADC_Left_Detect = ADCMEM0;    // Move result into Global
      ADC_Left_Detect =             // Divide the result by 4
        ADC_Left_Detect >> 2;
      ADCCTL0 |= ADCENC; // Enable Conversions
      ADCCTL0 |= ADCSC; // Start next sample
      break;
      
    case 0x02:
      // Read in Thumb Wheel Value (in ADMEM0)
      ADCMCTL0 &= ~ADCINCH_5;       // Disable Last channel A5
      ADCMCTL0 |= ADCINCH_3;        // Enable Next channel A3
      ADC_Left_Detect = ADCMEM0;    // Move result into Global
      ADC_Left_Detect =             // Divide the result by 4
        ADC_Left_Detect >> 2;
      ADCCTL0 |= ADCENC; // Enable Conversions
      ADCCTL0 |= ADCSC; // Start next sample
      ADC_Channel=0;
      just_read_in_values = 1;
      break;
      
    default:
      break;
    }
    break;
    
  default:
    break;
  }
}

//------------------------------------------------------------------------------
//******************************************************************************
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//------------------------------------------------------------------------------
void hex_to_bcd(int hex_value){
  int value = 0;
  adc_char[0] = '0';
  adc_char[1] = '0';
  adc_char[2] = '0';
  adc_char[3] = '0';
  while (hex_value > 999){
    hex_value = hex_value - 1000;
    value = value + 1;
    adc_char[0] = 0x30 + value;
  }
  value = 0;
  while (hex_value > 99){
    hex_value = hex_value - 100;
    value = value + 1;
    adc_char[1] = 0x30 + value;
  }
  value = 0;
  while (hex_value > 9){
    hex_value = hex_value - 10;
    value = value + 1;
    adc_char[2] = 0x30 + value;
  }
  adc_char[3] = 0x30 + hex_value;
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