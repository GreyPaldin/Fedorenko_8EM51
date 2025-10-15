#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include "stm32f1xx.h"  // Добавьте этот include

// ЗАКОММЕНТИРУЙТЕ эти строки - они уже есть в CMSIS
// #define GPIOA_BASE          (0x40010800UL)

// Используйте готовые определения из CMSIS
#define GPIOA_CRL           (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_CRH           (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_IDR           (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
#define GPIOA_ODR           (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_BSRR          (*(volatile uint32_t *)(GPIOA_BASE + 0x10))
#define GPIOA_BRR           (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

// Адреса для тактирования (RCC) - используйте готовые из CMSIS
#define RCC_APB2ENR         (*(volatile uint32_t *)(RCC_BASE + 0x18))

// Остальные макросы остаются без изменений...
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (BIT))

#define LED2_PIN             (5)
#define LED2_ON()           SET_BIT(GPIOA_BSRR, (1 << LED2_PIN))
#define LED2_OFF()          SET_BIT(GPIOA_BSRR, (1 << (LED2_PIN + 16)))
#define LED2_TOGGLE()       TOGGLE_BIT(GPIOA_ODR, (1 << LED2_PIN))

#define BUTTON2_PIN         (1)
#define BUTTON2_READ()      (!(READ_BIT(GPIOA_IDR, (1 << BUTTON2_PIN))))

void GPIO_Init(void);

#endif