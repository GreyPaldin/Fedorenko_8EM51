#include "init.h"

void GPIO_Init(void) {
    // 1. Включим тактирование порта A (GPIOA)
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
    
    // 2. Настройка PA0 и PA1 как входы с подтяжкой к питанию
    // PA0 (биты 0-3 в CRL): CNF=10, MODE=00
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_CNF0_1; // Input with pull-up/pull-down
    
    // PA1 (биты 4-7 в CRL): CNF=10, MODE=00  
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA->CRL |= GPIO_CRL_CNF1_1; // Input with pull-up/pull-down
    
    // Включаем подтяжку к питанию для PA0 и PA1
    SET_BIT(GPIOA->ODR, GPIO_ODR_ODR0 | GPIO_ODR_ODR1);

    // 3. Настройка светодиодов как выходов Push-Pull
    // PA4 (LED1) - биты 16-19 в CRL: MODE=01, CNF=00
    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |= GPIO_CRL_MODE4_0; // Output mode, max speed 10 MHz
    
    // PA5 (LED2) - биты 20-23 в CRL: MODE=01, CNF=00
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= GPIO_CRL_MODE5_0;
    
    // PA6 (LED3) - биты 24-27 в CRL: MODE=01, CNF=00
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= GPIO_CRL_MODE6_0;
    
    // PA7 (LED4) - биты 28-31 в CRL: MODE=01, CNF=00
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOA->CRL |= GPIO_CRL_MODE7_0;
}