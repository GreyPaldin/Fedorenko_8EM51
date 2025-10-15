// #include "../../CMSIS\Devices\STM32F1xx\Inc\STM32f103xB\stm32f103xb.h"
// #include "../../CMSIS/Devices/STM32F1xx/Inc/stm32f1xx.h"
#include <stdint.h>

// Регистры STM32F103
#define GPIOA_BASE          0x40010800U
#define RCC_BASE            0x40021000U

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)

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

void delay(void) {
    for(volatile uint32_t i = 0; i < 500000; i++);
}

int main(void) {
    // 1. ВКЛЮЧАЕМ ТАКТИРОВАНИЕ PORTA
    RCC->APB2ENR |= (1 << 2);
    
    // 2. Настраиваем светодиоды как выходы
    // PA4, PA5, PA6, PA7 = Output Push-Pull
    GPIOA->CRL &= ~(0xFFFF0000); // Очищаем биты для PA4-PA7
    GPIOA->CRL |= 0x11110000;    // MODE=01 (Output), CNF=00 (Push-pull)
    
    // 3. Настраиваем кнопки как входы с подтяжкой
    // PA0, PA1 = Input with pull-up
    GPIOA->CRL &= ~(0x000000FF); // Очищаем биты для PA0-PA1
    GPIOA->CRL |= 0x00000088;    // CNF=10 (Input pull-up/down), MODE=00 (Input)
    GPIOA->ODR |= (1 << 0) | (1 << 1); // Включаем подтяжку к питанию
    
    // 4. ТЕСТ: Мигаем всеми светодиодами 3 раза
    for(int i = 0; i < 3; i++) {
        GPIOA->BSRR = (1<<4) | (1<<5) | (1<<6) | (1<<7); // ВКЛ все
        delay();
        GPIOA->BSRR = (1<<20) | (1<<21) | (1<<22) | (1<<23); // ВЫКЛ все
        delay();
    }
    
    // 5. ОСНОВНОЙ ЦИКЛ с реакцией на кнопки
    while(1) {
        // Проверяем кнопку 1 (PA0)
        if(!(GPIOA->IDR & (1 << 0))) { // Если кнопка нажата (0V)
            GPIOA->BSRR = (1 << 4); // ВКЛЮЧАЕМ LED1 (PA4)
        } else {
            GPIOA->BSRR = (1 << (4 + 16)); // ВЫКЛЮЧАЕМ LED1
        }
        
        // Проверяем кнопку 2 (PA1)  
        if(!(GPIOA->IDR & (1 << 1))) { // Если кнопка нажата (0V)
            GPIOA->BSRR = (1 << 5); // ВКЛЮЧАЕМ LED2 (PA5)
        } else {
            GPIOA->BSRR = (1 << (5 + 16)); // ВЫКЛЮЧАЕМ LED2
        }
        
        // Короткая задержка
        for(volatile uint32_t i = 0; i < 10000; i++);
    }
}