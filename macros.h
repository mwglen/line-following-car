// Base Types
typedef enum {
  false = 0,
  true  = 1,
} bool;

// Global Variables
extern char display_line[4][11];
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern unsigned int MY_TIME;
extern bool SW1_PRESSED;
extern bool SW2_PRESSED;

extern unsigned int sw1_position;
extern unsigned int sw2_position;

#define ALWAYS          (1)
#define RESET_STATE     (0)
#define TRUE            (0x01)

// Switches
#define DEBOUNCE_TIME (1500)
#define DEBOUNCE_RESTART (0x00)
#define PRESSED (0)
#define RELEASED (1)

// Port 1 Pins
#define RED_LED         (0x01)      // 0 RED LED 0
#define A1_SEEED        (0x02)      // 1 A1_SEEED
#define V_DETECT_L      (0x04)      // 2 V_DETECT_L
#define V_DETECT_R      (0x08)      // 3 V_DETECT_R
#define SMCLK_2476      (0x10)      // 4 SMCLK_2476
#define V_THUMB         (0x20)      // 5 V_THUMB
#define UCA0RXD         (0x40)      // 6 Back Channel UCA0RXD
#define UCA0TXD         (0x80)      // 7 Back Channel UCA0XTD

// Port 2 Pins
#define RESET_LCD       (0x01)      // 0 RESET LCD
#define L_REVERSE_2476  (0x02)      // 1 L_REVERSE FOR MSP430F2476
#define P2_2            (0x04)      // 2 P2_2 
#define SW2             (0x08)      // 3 SW2
#define IOT_RUN_CPU     (0x10)      // 4 IOT_RUN_CPU
#define DAC_ENB         (0x20)      // 5 DAC_ENB
#define LFXOUT          (0x40)      // 6 LFXOUTR
#define LFXIN           (0x80)      // 7 LFXINR

// Port 3 Pins
#define TEST_PROBE      (0x01)      // 0 TEST PROBE
#define DAC_CNTL1       (0x02)      // 1 DAC_CNTL1
#define OA2N            (0x04)      // 2 Photodiode Circuit
#define LCD_BACKLITE    (0x08)      // 3 LCD_BACKLITE
#define SMCLK_2355      (0x10)      // 4 SMCLK
#define DAC_CNTL        (0x20)      // 5 DAC signal from Processor
#define IOT_LINK_CPU    (0x40)      // 6 IOT_LINK_CPU
#define IOT_EN_CPU      (0x80)      // 7 IOT_EN_CPU

// Port 4 Pins
#define P4_0            (0x01)      // 0 P4_0
#define SW1             (0x02)      // 1 SW1
#define UCA1RXD         (0x04)      // 2 Back Channel UCA1RXD
#define UCA1TXD         (0x08)      // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD     (0x10)      // 4 Chip Select
#define UCB1CLK         (0x20)      // 5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO        (0x40)      // 6 UCB1SIMO
#define UCB1SOMI        (0x80)      // 7 UCB1SOMI

// Port 5 Pins
#define CHECK_BAT       (0x01)      // 0 CHECK_BAT
#define V_BAT           (0x02)      // 1 V_BAT
#define V_DAC           (0x04)      // 2 V_DAC
#define V_3_3           (0x08)      // 3 V_3_3
#define IOT_BOOT_CPU    (0x10)      // 4 IOT_BOOT_CPU

// Port 6 Pins
#define R_FORWARD       (0x01)      // 0 R_FORWARD
#define L_FORWARD       (0x02)      // 1 L_FORWARD
#define R_REVERSE       (0x04)      // 2 R_REVERSE
#define L_REVERSE       (0x08)      // 3 L_REVERSE
#define IR_SENSOR       (0x10)      // 4 IR_SENSOR
#define P6_5            (0x20)      // 5 P6_5
#define GRN_LED         (0x40)      // 6 GRN_LED

