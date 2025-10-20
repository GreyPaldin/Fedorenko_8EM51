#ifndef INIT_H
#define INIT_H

#include <stdint.h>

// Базовые адреса
#define GPIOA_BASE          0x40010800U
#define GPIOC_BASE          0x40011000U
#define RCC_BASE            0x40021000U

// Структуры периферии
typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

// Макросы для доступа к периферии
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)

// Макросы для настройки пинов
#define PIN_MODE_INPUT      0x8
#define PIN_MODE_OUTPUT_10MHz 0x1
#define PIN_MODE_OUTPUT_2MHz  0x2
#define PIN_MODE_OUTPUT_50MHz 0x3

#define CNF_ANALOG          0x0
#define CNF_FLOATING        0x4
#define CNF_PULL_UPDOWN     0x8
#define CNF_OPEN_DRAIN      0xC

// Функции для настройки портов
void Init_RCC(void);
void Init_GPIO(void);

#endif