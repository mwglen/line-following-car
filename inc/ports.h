/// Functions
void init_ports(void);
void init_port1(void);
void init_port2(void);
void init_port3(char smclk);
void init_port4(void);
void init_port5(void);
void init_port6(void);

/// Defines
// States
#define RESET_STATE     (0)

// SMCLK
#define USE_GPIO   (0x00)
#define USE_SMCLK  (0x01)

// Port 1 Pins
#define RED_LED         (0x01)      // 0 RED LED 0
#define A1_SEEED        (0x02)      // 1 A1_SEEED
#define LEFT_IR         (0x04)      // 2 L_IR_SENSOR
#define RIGHT_IR        (0x08)      // 3 R_IR_SENSOR
#define SMCLK_2476      (0x10)      // 4 SMCLK_2476
#define THUMB           (0x20)      // 5 THUMB
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
#define UCB1CLK         (0x20)      // 5 SPI mode - clock output?UCB1CLK
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
#define IR_EMITTER      (0x10)      // 4 IR_EMITTER
#define P6_5            (0x20)      // 5 P6_5
#define GRN_LED         (0x40)      // 6 GRN_LED