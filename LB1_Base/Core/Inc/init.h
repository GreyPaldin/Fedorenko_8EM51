#ifndef INIT_H
#define INIT_H

#include <stdint.h>

// ==================== БАЗОВЫЕ АДРЕСА ====================
#define PERIPH_BASE       0x40000000U
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000U)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x20000U)

// ==================== АДРЕСА РЕГИСТРОВ ====================
#define RCC_APB2ENR      (*(volatile uint32_t*)(0x40021000U + 0x18U))

#define GPIOA_CRL        (*(volatile uint32_t*)(0x40010800U + 0x00U))
#define GPIOA_IDR        (*(volatile uint32_t*)(0x40010800U + 0x08U))
#define GPIOA_ODR        (*(volatile uint32_t*)(0x40010800U + 0x0CU))
#define GPIOA_BSRR       (*(volatile uint32_t*)(0x40010800U + 0x10U))

#define GPIOC_CRH        (*(volatile uint32_t*)(0x40011000U + 0x04U))
#define GPIOC_IDR        (*(volatile uint32_t*)(0x40011000U + 0x08U))
#define GPIOC_ODR        (*(volatile uint32_t*)(0x40011000U + 0x0CU))

// ==================== БИТОВЫЕ МАСКИ ДЛЯ PA2 (LED1)====================
#define RCC_GPIOA_EN     0x00000004U  
#define RCC_GPIOC_EN     0x00000010U  

#define GPIOA_MODE_PA2_OUT       0x00000300U  
#define GPIOA_MODE_PA2_MASK      ~(0x00000F00U)

#define GPIOA_BSRR_PA2_SET       0x00000004U  
#define GPIOA_BSRR_PA2_RESET     0x00040000U  

// ==================== БИТОВЫЕ МАСКИ ДЛЯ PA5 (LED2) ====================
#define GPIOA_MODE_PA5_OUT       0x00300000U 
#define GPIOA_MODE_PA5_MASK      ~(0x00F00000U)

#define GPIOA_BSRR_PA5_SET       0x00000020U  
#define GPIOA_BSRR_PA5_RESET     0x00200000U  

// ==================== БИТОВЫЕ МАСКИ ДЛЯ PA6 (LED3) ====================
#define GPIOA_MODE_PA6_OUT       0x03000000U  
#define GPIOA_MODE_PA6_MASK      ~(0x0F000000U)

#define GPIOA_BSRR_PA6_SET       0x00000040U  
#define GPIOA_BSRR_PA6_RESET     0x00400000U   

// ==================== БИТОВЫЕ МАСКИ ДЛЯ PA3 (LED4) ====================
#define GPIOA_MODE_PA3_OUT       0x00003000U  
#define GPIOA_MODE_PA3_MASK      ~(0x0000F000U)

#define GPIOA_BSRR_PA3_SET       0x00000008U  
#define GPIOA_BSRR_PA3_RESET     0x00080000U  

// ==================== БИТОВЫЕ МАСКИ ДЛЯ PA0 (Кнопка 1) ====================
#define GPIOA_MODE_PA0_INPUT     0x00000008U  
#define GPIOA_MODE_PA0_OUTPUT    0x00000003U  
#define GPIOA_MODE_PA0_MASK      ~(0x0000000FU)

#define GPIOA_PULLUP_PA0         0x00000001U  
#define GPIOA_IDR_PA0            0x00000001U  

// ==================== БИТОВЫЕ МАСКИ ДЛЯ PC14 (Кнопка 2) ====================
#define GPIOC_MODE_PC14_INPUT    0x08000000U  
#define GPIOC_MODE_PC14_MASK     ~(0x0F000000U)

#define GPIOC_PULLUP_PC14        0x00004000U  
#define GPIOC_IDR_PC14           0x00004000U 

// ==================== УТИЛИТЫ ====================
#define READ_BIT(REG, BIT)       ((REG) & (BIT))
#define SET_BIT(REG, BIT)        ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)      ((REG) &= ~(BIT))

// ==================== ФУНКЦИИ ====================
void GPIO_Init(void);
void delay(uint32_t count);
void toggle_PA0_mode(void);

// Глобальная переменная
extern uint8_t pa0_mode;

#endif