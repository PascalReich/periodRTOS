/**
 * File Name: init.c
 * Author: Scott Pavetti
 * Date: 2-Sept 2024
 * Carnegie Mellon University
 * Course# 17638 Engineeing Embedded Systems
 * 
 * 
 * Description: Sample code scaffolding for 'hello led' blinky application.
 *
 * Usage: This demo code is intended for the STM Discovery with an STM32F303VTC chip.
 * All ISRs except ResetHandler call into the Default_Handler(). 
 *
 */
 

void Reset_Handler(void);
void DefaultHandler(void);
void TIM2_Handler (void) __attribute__ ((weak));
void SysTick_Handler (void) __attribute__ ((weak));
void NMI_Handler (void) __attribute__ ((weak));
void PendSV_Handler (void) __attribute__ ((weak));
void HardFaultHandler(void);

// Symbols are declared in the linker script
extern unsigned char  _sidata;
extern unsigned char  _sdata;
extern unsigned char  _edata;
extern unsigned char  __bss_start__;
extern unsigned char  __bss_end__;

// Vectors is placed at the beginning of flash by the linker
// Using the __attribute((section(".vectors"))) we can instruct
// the linker what section to place this array into.
const void * Vectors[] __attribute__((section(".isr_vector"))) ={
	(void *)0x2000a000, 	/* Top of stack */ 
	Reset_Handler,   	/* Reset Handler */
    NMI_Handler,		/* NMI */
	HardFaultHandler,		/* Hard Fault */
	0,	                /* Reserved */
	0,                   	/* Reserved */
	0,                  	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	DefaultHandler,   	/* SVC */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	PendSV_Handler,   	/* PendSV */
	SysTick_Handler,   	/* SysTick */		
	/* External interrupt handlers follow */
	DefaultHandler, 	/* WWDG */
	DefaultHandler, 	/* PVD */
	DefaultHandler, 	/* RTC */
	DefaultHandler, 	/* FLASH */
	DefaultHandler, 	/* RCC */
	DefaultHandler, 	/* EXTI0_1 */
	DefaultHandler, 	/* EXTI2_3 */
	DefaultHandler, 	/* EXTI4_15 */
	DefaultHandler, 	/* TSC */
	DefaultHandler, 	/* DMA_CH1 */
	DefaultHandler, 	/* DMA_CH2_3 */
	DefaultHandler, 	/* DMA_CH4_5 */
	DefaultHandler,		/* ADC_COMP */
	DefaultHandler,  	/* TIM1_BRK_UP_TRG_COM */
	DefaultHandler, 	/* TIM1_CC */
	TIM2_Handler, 	    /* TIM2 */
	DefaultHandler, 	/* TIM3 */
	DefaultHandler, 	/* TIM6_DAC */
	DefaultHandler, 	/* RESERVED */
	DefaultHandler, 	/* TIM14 */
	DefaultHandler, 	/* TIM15 */
	DefaultHandler,		/* TIM16 */
	DefaultHandler, 	/* TIM17 */
	DefaultHandler, 	/* I2C1 */
	DefaultHandler, 	/* I2C2 */
	DefaultHandler, 	/* SPI1 */
	DefaultHandler, 	/* SPI2 */
	DefaultHandler, 	/* USART1 */
	DefaultHandler, 	/* USART2 */
	DefaultHandler, 	/* RESERVED */
	DefaultHandler, 	/* CEC */
	DefaultHandler 		/* RESERVED */
};
/**
 * ResetHandler()
 * Set up as the function called when the processor resets.
 * Uses a couple of symbols required to be in the linker file
 * that represent the start and end addresses that are used
 * in the copying and zeroing sequence.
 * 
 * 1. Get the addresses from our linker symbols
 * 2. Copy contents of flash to sram
 * 3. Zero the .bss section
 * 4. Call main()
 */
void Reset_Handler()
{
	// do global/static data initialization
	unsigned char *src;
	unsigned char *dest;
	unsigned len;
	src= &_sidata;
	dest= &_sdata;
	len= &_edata-&_sdata;
	//Copy to sram
	//memcpy(dest, src, len);
	while (len--) *(dest++) = *(src++);
	
	// zero out the uninitialized global/static variable locations
	dest = &__bss_start__;
	len = &__bss_end__ - &__bss_start__;
	//Zero bss section
	//memset(dest, 0, len);
	while (len--) *(dest++) = 0;

	//Jump to application
	asm ("bl main");
}

/**
 * DefaultHandler()
 *
 * Simple while(1) trap.
 *
 */
void DefaultHandler()
{
	while(1);
}

void NMI_Handler() {
	while(1);
}

void PendSV_Handler() {
	while(1);
}


void HardFaultHandler() {
	while(1);
}