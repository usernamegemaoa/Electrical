#ifndef MICRO_SETUP_H
#define	MICRO_SETUP_H

#include <xc.h>                 // PIC16F917 Configuration Bit Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC1:FOSC0 as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor enabled)
#pragma config IESO = OFF       // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up disabled)

//#pragma config MODE = MM        // External Memory Bus (Microcontroller mode - External bus disabled)

// CONFIG3H
//#pragma config CCP2MX = DEFAULT // ECCP2 MUX bit (ECCP2/P2A is multiplexed with RC1)
//#pragma config ECCPMX = DEFAULT // ECCPx MUX bit (ECCP1 outputs (P1B/P1C) are multiplexed with RE6 and RE5; ECCP3 outputs (P3B/P3C) are multiplexed with RE4 and RE3)

#pragma config DEBUG = ON       // In-Circuit Debugger enabled, RB6/ICSPCLK and RB7/ICSPDAT are dedicated to the debugger

// Register map
//=============
#define	PWR_CONTROL					0x000	// RW	Power & conversion control

#define	STAGE_CAL_EN				0x001	// RW	Ambient compensation control register 0
#define	AMB_COMP_CTRL0				0x002	// RW	Ambient compensation control register 1
#define	AMB_COMP_CTRL1				0x003	// RW	Ambient compensation control register 2
#define	AMB_COMP_CTRL2				0x004	// RW	Ambient compensation control register 3

#define	STAGE_LOW_INT_EN			0x005	// RW	Interrupt enable register 0
#define	STAGE_HIGH_INT_EN			0x006	// RW	Interrupt enable register 1
#define	STAGE_COMPLETE_INT_EN		0x007	// RW	Interrupt enable register 2
#define	STAGE_LOW_LIMIT_INT			0x008	// R	Low limit interrupt status register 0
#define	STAGE_HIGH_LIMIT_INT		0x009	// R	High limit interrupt status register 1
#define	STAGE_COMPLETE_LIMIT_INT	0x00A	// R	Interrupt status register 2

#define	CDC_RESULT_S0				0x00B	// R	CDC stage 0 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S1				0x00C	// R	CDC stage 1 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S2				0x00D	// R	CDC stage 2 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S3				0x00E	// R	CDC stage 3 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S4				0x00F	// R	CDC stage 4 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S5				0x010	// R	CDC stage 5 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S6				0x011	// R	CDC stage 6 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S7				0x012	// R	CDC stage 7 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S8				0x013	// R	CDC stage 8 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S9				0x014	// R	CDC stage 9 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S10				0x015	// R	CDC stage 10 result (uncompensated) actually located in SRAM
#define	CDC_RESULT_S11				0x016	// R	CDC stage 11 result (uncompensated) actually located in SRAM

#define	DEV_ID						0x017	// R	I.D. Register
#define ENABLE_WRITE                0xE000  // W    Enable Word to Write
#define ENABLE_READ                 0xE400  // R    Enable Word to Read

#define	THRES_STAT_REG0				0x040	// R	Current threshold status register 0
#define	THRES_STAT_REG1				0x041	// R	Current threshold status register 1
#define	PROX_STAT_REG				0x042	// R	Current proximity status register 2

extern unsigned int CDC_RESULT_REGISTER[12] = {CDC_RESULT_S0, CDC_RESULT_S1, CDC_RESULT_S2, CDC_RESULT_S3, CDC_RESULT_S4,
CDC_RESULT_S5, CDC_RESULT_S6, CDC_RESULT_S7, CDC_RESULT_S8, CDC_RESULT_S9, CDC_RESULT_S10, CDC_RESULT_S11};

extern unsigned int CDC_RESULT[12] = 0; // Result from the CDC 
extern unsigned int AD7147Registers[23] = 0;
// Ram map - these registers are defined as we go along
//=====================================================
#define STAGE0_CONNECTION       	0x80
#define STAGE1_CONNECTION           0x88
#define STAGE2_CONNECTION           0x90
#define STAGE3_CONNECTION           0x98
#define STAGE4_CONNECTION           0xA0
#define STAGE5_CONNECTION           0xA8
#define STAGE6_CONNECTION           0xB0
#define STAGE7_CONNECTION           0xB8
#define STAGE8_CONNECTION           0xC0
#define STAGE9_CONNECTION           0xC8
#define STAGE10_CONNECTION          0xD0
#define STAGE11_CONNECTION          0xD8

#define STAGE0                  	0xE0
#define STAGE0_AMBIENT              0xF1
#define STAGE0_MAX_AVG              0xF9
#define STAGE0_UPP_THRES            0xFA
#define STAGE0_MIN_AVG              0x100
#define STAGE0_LWR_THRES            0x101

#define STAGE1                      0x104
#define STAGE1_AMBIENT              0x115
#define STAGE1_MAX_AVG              0x11D
#define STAGE1_UPP_THRES            0x11E
#define STAGE1_MIN_AVG              0x124
#define STAGE1_LWR_THRES            0x125

#define STAGE2                      0x128
#define STAGE2_AMBIENT              0x139
#define STAGE2_MAX_AVG              0x141
#define STAGE2_UPP_THRES            0x142
#define STAGE2_MIN_AVG              0x148
#define STAGE2_LWR_THRES            0x149

#define STAGE3                      0x14C
#define STAGE3_AMBIENT              0x15D
#define STAGE3_MAX_AVG              0x165
#define STAGE3_UPP_THRES            0x166
#define STAGE3_MIN_AVG              0x16C
#define STAGE3_LWR_THRES            0x16D

#define STAGE4                      0x170
#define STAGE4_AMBIENT              0x181
#define STAGE4_MAX_AVG              0x189
#define STAGE4_UPP_THRES            0x18A
#define STAGE4_MIN_AVG              0x190
#define STAGE4_LWR_THRES            0x191

#define STAGE5                      0x194
#define STAGE5_AMBIENT              0x1A5
#define STAGE5_MAX_AVG              0x1AD
#define STAGE5_UPP_THRES            0x1AE
#define STAGE5_MIN_AVG              0x1B4
#define STAGE5_LWR_THRES            0x1B5

#define STAGE6                      0x1B8
#define STAGE6_AMBIENT              0x1C9
#define STAGE6_MAX_AVG              0x1D1
#define STAGE6_UPP_THRES            0x1D2
#define STAGE6_MIN_AVG              0x1D8
#define STAGE6_LWR_THRES            0x1D9

#define STAGE7                      0x1DC
#define STAGE7_AMBIENT              0x1ED
#define STAGE7_MAX_AVG              0x1F5
#define STAGE7_UPP_THRES            0x1F6
#define STAGE7_MIN_AVG              0x1FC
#define STAGE7_LWR_THRES            0x1FD

#define STAGE8                      0x200
#define STAGE8_AMBIENT              0x211
#define STAGE8_MAX_AVG              0x219
#define STAGE8_UPP_THRES            0x21A
#define STAGE8_MIN_AVG              0x220
#define STAGE8_LWR_THRES            0x221

#define STAGE9                      0x224
#define STAGE9_AMBIENT              0x234
#define STAGE9_MAX_AVG          	0x23D
#define STAGE9_UPP_THRES            0x23E
#define STAGE9_MIN_AVG              0x244
#define STAGE9_LWR_THRES            0x245

#define STAGE10                     0x248
#define STAGE10_AMBIENT             0x259
#define STAGE10_MAX_AVG             0x261
#define STAGE10_UPP_THRES           0x262
#define STAGE10_MIN_AVG             0x268
#define STAGE10_LWR_THRES           0x269

#define STAGE11                     0x26C
#define STAGE11_AMBIENT             0x27D
#define STAGE11_MAX_AVG             0x285
#define STAGE11_UPP_THRES           0x286
#define STAGE11_MIN_AVG             0x28C
#define STAGE11_LWR_THRES           0x28D

#define LED0 PORTDbits.RD0        // Defined Pin RD0 to variable LED0
#define LED1 PORTDbits.RD1        // Defined Pin RD1 to variable LED1
#define LED2 PORTDbits.RD2        // Defined Pin RD2 to variable LED2
#define LED3 PORTDbits.RD3        // Defined Pin RD3 to variable LED3
#define LED4 PORTDbits.RD4        // Defined Pin RD4 to variable LED4
#define LED5 PORTDbits.RD5        // Defined Pin RD5 to variable LED5
#define LED6 PORTDbits.RD6        // Defined Pin RD6 to variable LED6
#define LED7 PORTDbits.RD7        // Defined Pin RD7 to variable LED7

#define CS_AD7147 PORTCbits.RC2   // Defined Pin RE0 to variable CS_AD7147

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) 

#define LED_ON (1)               // DIGITAL: Assigning variable LED_ON to the value: 1
#define LED_OFF (0)              // DIGITAL: Assigning variable LED_OFF to the value: 0

#define DUTY_ON (1)               // DIGITAL: Assigning variable DUTY_ON to the value: 1
#define DUTY_OFF (0)              // DIGITAL: Assigning variable DUTY_OFF to the value: 0

#define HALL_ON (1)               // DIGITAL: Assigning variable HALL_ON to the value: 1
#define HALL_OFF (0)              // DIGITAL: Assigning variable HALL_OFF to the value: 0

#define TX PORTCbits.RC6         // Defined Pin RC6 to variable TX
#define RX PORTCbits.RC7         // Defined Pin RC7 to variable RX

#define _XTAL_FREQ 10000000       // 10 MHz External Crystal 

void setup_mcu(void);

void setup_port(void);

void init_uart(void);

void init_pin(void);

void delay_ms(int n);

#endif	/* MICRO_SETUP_H */