/**
 * @file stm32f4xx.h
 * @brief STM32F4xx peripheral definitions
 */

#ifndef STM32F4XX_H
#define STM32F4XX_H

#include <stdint.h>

/* NVIC priority bits */
#define __NVIC_PRIO_BITS 4

/* System Core Clock */
extern uint32_t SystemCoreClock;

/* System Init function */
void SystemInit(void);

/* Memory and peripheral base addresses (STM32F303 layout) */
#define FLASH_BASE            0x40022000UL
#define SRAM_BASE             0x20000000UL
#define PERIPH_BASE           0x40000000UL
#define SRAM_BB_BASE          0x22000000UL
#define PERIPH_BB_BASE        0x42000000UL

/* AHB1 peripherals (STM32F303) */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x1000UL)

/* APB1 peripherals (STM32F303) */
#define APB1PERIPH_BASE       PERIPH_BASE
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800UL)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00UL)

/* WWDG register definitions */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFR;
    volatile uint32_t SR;
} WWDG_TypeDef;

#define WWDG                 ((WWDG_TypeDef *) WWDG_BASE)

/* WWDG Control Register (WWDG_CR) */
#define WWDG_CR_T_Pos                0
#define WWDG_CR_T_Msk                (0x7FUL << WWDG_CR_T_Pos)
#define WWDG_CR_WDGA_Pos             7
#define WWDG_CR_WDGA_Msk             (1UL << WWDG_CR_WDGA_Pos)
#define WWDG_CR_WDGA                 WWDG_CR_WDGA_Msk
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000UL)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800UL)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00UL)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000UL)

/* APB2 peripherals (STM32F303) */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x0000UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400UL)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x3400UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800UL)
#define TIM15_BASE            (APB2PERIPH_BASE + 0x4000UL)
#define TIM16_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM17_BASE            (APB2PERIPH_BASE + 0x4800UL)

/* AHB2 peripherals (STM32F303 GPIO) */
#define AHB2PERIPH_BASE       0x48000000UL
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB2PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x1400UL)

/* AHB3 peripherals (STM32F303) */
#define AHB3PERIPH_BASE       0x50000000UL

/* Core peripherals */
#define SCS_BASE              0xE000E000UL
#define ITM_BASE              0xE0000000UL
#define DWT_BASE              0xE0001000UL
#define TPI_BASE              0xE0040000UL
#define CoreDebug_BASE        0xE000EDF0UL
#define SysTick_BASE          (SCS_BASE + 0x0010UL)
#define NVIC_BASE             (SCS_BASE + 0x0100UL)
#define SCB_BASE              (SCS_BASE + 0x0D00UL)

/* Peripheral declarations */
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOI               ((GPIO_TypeDef *) GPIOI_BASE)
#define SysTick             ((SysTick_Type *) SysTick_BASE)
#define NVIC                ((NVIC_Type *) NVIC_BASE)
#define SCB                 ((SCB_Type *) SCB_BASE)

/* RCC register definitions (STM32F303) */
typedef struct {
    volatile uint32_t CR;         /* 0x00 */
    volatile uint32_t CFGR;       /* 0x04 */
    volatile uint32_t CIR;        /* 0x08 */
    volatile uint32_t APB2RSTR;   /* 0x0C */
    volatile uint32_t APB1RSTR;   /* 0x10 */
    volatile uint32_t AHBENR;     /* 0x14 */
    volatile uint32_t APB2ENR;    /* 0x18 */
    volatile uint32_t APB1ENR;    /* 0x1C */
    volatile uint32_t BDCR;       /* 0x20 */
    volatile uint32_t CSR;        /* 0x24 */
    volatile uint32_t AHBRSTR;    /* 0x28 */
    volatile uint32_t CFGR2;      /* 0x2C */
    volatile uint32_t CFGR3;      /* 0x30 */
} RCC_TypeDef;

/* GPIO register definitions */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

/* SysTick register definitions */
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_Type;

/* NVIC register definitions */
typedef struct {
    volatile uint32_t ISER[8];
    volatile uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    volatile uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];
    volatile uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    volatile uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];
    volatile uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];
    volatile uint32_t RESERVED5[644];
    volatile uint32_t STIR;
} NVIC_Type;

/* SCB register definitions */
typedef struct {
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR[3];
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t DFSR;
    volatile uint32_t MMFAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
    volatile uint32_t PFR[2];
    volatile uint32_t DFR;
    volatile uint32_t ADR;
    volatile uint32_t MMFR[4];
    volatile uint32_t ISAR[5];
    volatile uint32_t RESERVED0[5];
    volatile uint32_t CPACR;
} SCB_Type;

/* Flash register definitions */
typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
    volatile uint32_t OPTCR1;
} FLASH_TypeDef;

#define FLASH                ((FLASH_TypeDef *) FLASH_BASE)

/* Register bit definitions */
#define RCC_CR_HSEON_Pos             16
#define RCC_CR_HSEON_Msk             (1UL << RCC_CR_HSEON_Pos)
#define RCC_CR_HSEON                 RCC_CR_HSEON_Msk
#define RCC_CR_HSERDY_Pos            17
#define RCC_CR_HSERDY_Msk            (1UL << RCC_CR_HSERDY_Pos)
#define RCC_CR_HSERDY                RCC_CR_HSERDY_Msk
#define RCC_CR_PLLON_Pos             24
#define RCC_CR_PLLON_Msk             (1UL << RCC_CR_PLLON_Pos)
#define RCC_CR_PLLON                 RCC_CR_PLLON_Msk
#define RCC_CR_PLLRDY_Pos            25
#define RCC_CR_PLLRDY_Msk            (1UL << RCC_CR_PLLRDY_Pos)
#define RCC_CR_PLLRDY                RCC_CR_PLLRDY_Msk

#define RCC_PLLCFGR_PLLSRC_Pos       22
#define RCC_PLLCFGR_PLLSRC_Msk       (3UL << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLSRC_HSE       (1UL << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLM_Pos         0
#define RCC_PLLCFGR_PLLM_Msk         (0x3FUL << RCC_PLLCFGR_PLLM_Pos)
#define RCC_PLLCFGR_PLLN_Pos         6
#define RCC_PLLCFGR_PLLN_Msk         (0x1FFUL << RCC_PLLCFGR_PLLN_Pos)
#define RCC_PLLCFGR_PLLP_Pos         16
#define RCC_PLLCFGR_PLLP_Msk         (3UL << RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLQ_Pos         24
#define RCC_PLLCFGR_PLLQ_Msk         (0xFUL << RCC_PLLCFGR_PLLQ_Pos)

#define RCC_CFGR_SW_Pos              0
#define RCC_CFGR_SW_Msk              (3UL << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_PLL              (2UL << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SWS_Pos             2
#define RCC_CFGR_SWS_Msk             (3UL << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS                 RCC_CFGR_SWS_Msk
#define RCC_CFGR_SWS_PLL             (2UL << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_HPRE_Pos            4
#define RCC_CFGR_HPRE_Msk            (0xFUL << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_HPRE_DIV1           (0UL << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_PPRE1_Pos           10
#define RCC_CFGR_PPRE1_Msk           (7UL << RCC_CFGR_PPRE1_Pos)
#define RCC_CFGR_PPRE1_DIV4          (5UL << RCC_CFGR_PPRE1_Pos)
#define RCC_CFGR_PPRE2_Pos           13
#define RCC_CFGR_PPRE2_Msk           (7UL << RCC_CFGR_PPRE2_Pos)
#define RCC_CFGR_PPRE2_DIV2          (4UL << RCC_CFGR_PPRE2_Pos)

/* RCC AHB enable bits (STM32F303) */
#define RCC_AHBENR_IOPAEN_Pos        17
#define RCC_AHBENR_IOPAEN_Msk        (1UL << RCC_AHBENR_IOPAEN_Pos)
#define RCC_AHBENR_IOPBEN_Pos        18
#define RCC_AHBENR_IOPBEN_Msk        (1UL << RCC_AHBENR_IOPBEN_Pos)
#define RCC_AHBENR_IOPCEN_Pos        19
#define RCC_AHBENR_IOPCEN_Msk        (1UL << RCC_AHBENR_IOPCEN_Pos)
#define RCC_AHBENR_IOPDEN_Pos        20
#define RCC_AHBENR_IOPDEN_Msk        (1UL << RCC_AHBENR_IOPDEN_Pos)
#define RCC_AHBENR_IOPEEN_Pos        21
#define RCC_AHBENR_IOPEEN_Msk        (1UL << RCC_AHBENR_IOPEEN_Pos)
#define RCC_AHBENR_IOPFEN_Pos        22
#define RCC_AHBENR_IOPFEN_Msk        (1UL << RCC_AHBENR_IOPFEN_Pos)
#define RCC_AHBENR_IOPEEN            RCC_AHBENR_IOPEEN_Msk

#define GPIO_MODER_MODER0_Pos        0
#define GPIO_MODER_MODER0_Msk        (3UL << GPIO_MODER_MODER0_Pos)
#define GPIO_MODER_MODER0_0          (1UL << GPIO_MODER_MODER0_Pos)
#define GPIO_MODER_MODER1_Pos        2
#define GPIO_MODER_MODER1_Msk        (3UL << GPIO_MODER_MODER1_Pos)
#define GPIO_MODER_MODER1_0          (1UL << GPIO_MODER_MODER1_Pos)
#define GPIO_MODER_MODER2_Pos        4
#define GPIO_MODER_MODER2_Msk        (3UL << GPIO_MODER_MODER2_Pos)
#define GPIO_MODER_MODER2_0          (1UL << GPIO_MODER_MODER2_Pos)
#define GPIO_MODER_MODER3_Pos        6
#define GPIO_MODER_MODER3_Msk        (3UL << GPIO_MODER_MODER3_Pos)
#define GPIO_MODER_MODER3_0          (1UL << GPIO_MODER_MODER3_Pos)
#define GPIO_MODER_MODER4_Pos        8
#define GPIO_MODER_MODER4_Msk        (3UL << GPIO_MODER_MODER4_Pos)
#define GPIO_MODER_MODER4_0          (1UL << GPIO_MODER_MODER4_Pos)
#define GPIO_MODER_MODER5_Pos        10
#define GPIO_MODER_MODER5_Msk        (3UL << GPIO_MODER_MODER5_Pos)
#define GPIO_MODER_MODER5_0          (1UL << GPIO_MODER_MODER5_Pos)
#define GPIO_MODER_MODER6_Pos        12
#define GPIO_MODER_MODER6_Msk        (3UL << GPIO_MODER_MODER6_Pos)
#define GPIO_MODER_MODER6_0          (1UL << GPIO_MODER_MODER6_Pos)
#define GPIO_MODER_MODER7_Pos        14
#define GPIO_MODER_MODER7_Msk        (3UL << GPIO_MODER_MODER7_Pos)
#define GPIO_MODER_MODER7_0          (1UL << GPIO_MODER_MODER7_Pos)
#define GPIO_MODER_MODER8_Pos        16
#define GPIO_MODER_MODER8_Msk        (3UL << GPIO_MODER_MODER8_Pos)
#define GPIO_MODER_MODER8_0          (1UL << GPIO_MODER_MODER8_Pos)
#define GPIO_MODER_MODER9_Pos        18
#define GPIO_MODER_MODER9_Msk        (3UL << GPIO_MODER_MODER9_Pos)
#define GPIO_MODER_MODER9_0          (1UL << GPIO_MODER_MODER9_Pos)
#define GPIO_MODER_MODER10_Pos       20
#define GPIO_MODER_MODER10_Msk       (3UL << GPIO_MODER_MODER10_Pos)
#define GPIO_MODER_MODER10_0         (1UL << GPIO_MODER_MODER10_Pos)
#define GPIO_MODER_MODER11_Pos       22
#define GPIO_MODER_MODER11_Msk       (3UL << GPIO_MODER_MODER11_Pos)
#define GPIO_MODER_MODER11_0         (1UL << GPIO_MODER_MODER11_Pos)
#define GPIO_MODER_MODER12_Pos       24
#define GPIO_MODER_MODER12_Msk       (3UL << GPIO_MODER_MODER12_Pos)
#define GPIO_MODER_MODER12_0         (1UL << GPIO_MODER_MODER12_Pos)
#define GPIO_MODER_MODER13_Pos       26
#define GPIO_MODER_MODER13_Msk       (3UL << GPIO_MODER_MODER13_Pos)
#define GPIO_MODER_MODER13_0         (1UL << GPIO_MODER_MODER13_Pos)
#define GPIO_MODER_MODER14_Pos       28
#define GPIO_MODER_MODER14_Msk       (3UL << GPIO_MODER_MODER14_Pos)
#define GPIO_MODER_MODER14_0         (1UL << GPIO_MODER_MODER14_Pos)
#define GPIO_MODER_MODER15_Pos       30
#define GPIO_MODER_MODER15_Msk       (3UL << GPIO_MODER_MODER15_Pos)
#define GPIO_MODER_MODER15_0         (1UL << GPIO_MODER_MODER15_Pos)

#define GPIO_ODR_OD0_Pos             0
#define GPIO_ODR_OD0_Msk             (1UL << GPIO_ODR_OD0_Pos)
#define GPIO_ODR_OD1_Pos             1
#define GPIO_ODR_OD1_Msk             (1UL << GPIO_ODR_OD1_Pos)
#define GPIO_ODR_OD2_Pos             2
#define GPIO_ODR_OD2_Msk             (1UL << GPIO_ODR_OD2_Pos)
#define GPIO_ODR_OD3_Pos             3
#define GPIO_ODR_OD3_Msk             (1UL << GPIO_ODR_OD3_Pos)
#define GPIO_ODR_OD4_Pos             4
#define GPIO_ODR_OD4_Msk             (1UL << GPIO_ODR_OD4_Pos)
#define GPIO_ODR_OD5_Pos             5
#define GPIO_ODR_OD5_Msk             (1UL << GPIO_ODR_OD5_Pos)
#define GPIO_ODR_OD6_Pos             6
#define GPIO_ODR_OD6_Msk             (1UL << GPIO_ODR_OD6_Pos)
#define GPIO_ODR_OD7_Pos             7
#define GPIO_ODR_OD7_Msk             (1UL << GPIO_ODR_OD7_Pos)
#define GPIO_ODR_OD8_Pos             8
#define GPIO_ODR_OD8_Msk             (1UL << GPIO_ODR_OD8_Pos)
#define GPIO_ODR_OD9_Pos             9
#define GPIO_ODR_OD9_Msk             (1UL << GPIO_ODR_OD9_Pos)
#define GPIO_ODR_OD10_Pos            10
#define GPIO_ODR_OD10_Msk            (1UL << GPIO_ODR_OD10_Pos)
#define GPIO_ODR_OD11_Pos            11
#define GPIO_ODR_OD11_Msk            (1UL << GPIO_ODR_OD11_Pos)
#define GPIO_ODR_OD12_Pos            12
#define GPIO_ODR_OD12_Msk            (1UL << GPIO_ODR_OD12_Pos)
#define GPIO_ODR_OD13_Pos            13
#define GPIO_ODR_OD13_Msk            (1UL << GPIO_ODR_OD13_Pos)
#define GPIO_ODR_OD14_Pos            14
#define GPIO_ODR_OD14_Msk            (1UL << GPIO_ODR_OD14_Pos)
#define GPIO_ODR_OD15_Pos            15
#define GPIO_ODR_OD15_Msk            (1UL << GPIO_ODR_OD15_Pos)

/* GPIO ODR bit definitions (convenience macros) */
#define GPIO_ODR_OD0                 GPIO_ODR_OD0_Msk
#define GPIO_ODR_OD1                 GPIO_ODR_OD1_Msk
#define GPIO_ODR_OD2                 GPIO_ODR_OD2_Msk
#define GPIO_ODR_OD3                 GPIO_ODR_OD3_Msk
#define GPIO_ODR_OD4                 GPIO_ODR_OD4_Msk
#define GPIO_ODR_OD5                 GPIO_ODR_OD5_Msk
#define GPIO_ODR_OD6                 GPIO_ODR_OD6_Msk
#define GPIO_ODR_OD7                 GPIO_ODR_OD7_Msk
#define GPIO_ODR_OD8                 GPIO_ODR_OD8_Msk
#define GPIO_ODR_OD9                 GPIO_ODR_OD9_Msk
#define GPIO_ODR_OD10                GPIO_ODR_OD10_Msk
#define GPIO_ODR_OD11                GPIO_ODR_OD11_Msk
#define GPIO_ODR_OD12                GPIO_ODR_OD12_Msk
#define GPIO_ODR_OD13                GPIO_ODR_OD13_Msk
#define GPIO_ODR_OD14                GPIO_ODR_OD14_Msk
#define GPIO_ODR_OD15                GPIO_ODR_OD15_Msk

#define SysTick_CTRL_ENABLE_Pos      0
#define SysTick_CTRL_ENABLE_Msk      (1UL << SysTick_CTRL_ENABLE_Pos)
#define SysTick_CTRL_TICKINT_Pos     1
#define SysTick_CTRL_TICKINT_Msk     (1UL << SysTick_CTRL_TICKINT_Pos)
#define SysTick_CTRL_CLKSOURCE_Pos   2
#define SysTick_CTRL_CLKSOURCE_Msk   (1UL << SysTick_CTRL_CLKSOURCE_Pos)

#define FLASH_ACR_LATENCY_Pos        0
#define FLASH_ACR_LATENCY_Msk        (0xFUL << FLASH_ACR_LATENCY_Pos)
#define FLASH_ACR_LATENCY_5WS        (5UL << FLASH_ACR_LATENCY_Pos)
#define FLASH_ACR_PRFTEN_Pos         8
#define FLASH_ACR_PRFTEN_Msk         (1UL << FLASH_ACR_PRFTEN_Pos)
#define FLASH_ACR_PRFTEN             FLASH_ACR_PRFTEN_Msk
#define FLASH_ACR_ICEN_Pos           9
#define FLASH_ACR_ICEN_Msk           (1UL << FLASH_ACR_ICEN_Pos)
#define FLASH_ACR_ICEN               FLASH_ACR_ICEN_Msk
#define FLASH_ACR_DCEN_Pos           10
#define FLASH_ACR_DCEN_Msk           (1UL << FLASH_ACR_DCEN_Pos)
#define FLASH_ACR_DCEN               FLASH_ACR_DCEN_Msk

/* IRQn_Type definition (STM32F303) */
typedef enum {
    NonMaskableInt_IRQn = -14,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn = -11,
    UsageFault_IRQn = -10,
    SVCall_IRQn = -5,
    DebugMonitor_IRQn = -4,
    PendSV_IRQn = -2,
    SysTick_IRQn = -1,
    WWDG_IRQn = 0,
    PVD_IRQn = 1,
    TAMP_STAMP_IRQn = 2,
    RTC_WKUP_IRQn = 3,
    FLASH_IRQn = 4,
    RCC_IRQn = 5,
    EXTI0_IRQn = 6,
    EXTI1_IRQn = 7,
    EXTI2_TS_IRQn = 8,
    EXTI3_IRQn = 9,
    EXTI4_IRQn = 10,
    DMA1_Channel1_IRQn = 11,
    DMA1_Channel2_IRQn = 12,
    DMA1_Channel3_IRQn = 13,
    DMA1_Channel4_IRQn = 14,
    DMA1_Channel5_IRQn = 15,
    DMA1_Channel6_IRQn = 16,
    DMA1_Channel7_IRQn = 17,
    ADC1_2_IRQn = 18,
    USB_HP_CAN_TX_IRQn = 19,
    USB_LP_CAN_RX0_IRQn = 20,
    CAN_RX1_IRQn = 21,
    CAN_SCE_IRQn = 22,
    EXTI9_5_IRQn = 23,
    TIM1_BRK_TIM15_IRQn = 24,
    TIM1_UP_TIM16_IRQn = 25,
    TIM1_TRG_COM_TIM17_IRQn = 26,
    TIM1_CC_IRQn = 27,
    TIM2_IRQn = 28,
    TIM3_IRQn = 29,
    TIM4_IRQn = 30,
    I2C1_EV_IRQn = 31,
    I2C1_ER_IRQn = 32,
    I2C2_EV_IRQn = 33,
    I2C2_ER_IRQn = 34,
    SPI1_IRQn = 35,
    SPI2_IRQn = 36,
    USART1_IRQn = 37,
    USART2_IRQn = 38,
    USART3_IRQn = 39,
    EXTI15_10_IRQn = 40,
    RTC_Alarm_IRQn = 41,
    USBWakeUp_IRQn = 42,
    TIM8_BRK_IRQn = 43,
    TIM8_UP_IRQn = 44,
    TIM8_TRG_COM_IRQn = 45,
    TIM8_CC_IRQn = 46,
    ADC3_IRQn = 47,
    FPU_IRQn = 81
} IRQn_Type;

/* NVIC functions */
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
void NVIC_EnableIRQ(IRQn_Type IRQn);
void NVIC_DisableIRQ(IRQn_Type IRQn);

#endif /* STM32F4XX_H */
