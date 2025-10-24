#include "init.h"

// Определение глобальной переменной (только здесь)
uint8_t led1_current_state = 0;

void Init_RCC(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 4); // GPIOA + GPIOC
}

void Init_GPIO(void) {
    // PA3 (LED4) - прямое обращение к регистрам
    volatile uint32_t *GPIOA_CRL = (volatile uint32_t*)(0x40010800U + 0x00);
    *GPIOA_CRL = (*GPIOA_CRL & ~(0xF << 12)) | (0x3 << 12); // PA3 - OUTPUT 50MHz
    
    // PA5, PA6 (LED2, LED3) - через макросы
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 20)) | (0x3 << 20); // PA5 - LED2  
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (0x3 << 24); // PA6 - LED3
    
    // PC13 как вход с подтяжкой (Кнопка 1) - через макросы
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x8 << 20);
    GPIOC->ODR |= (1 << 13); 
    
    // PC14 инициализируем как вход (режим будет меняться динамически) - через макросы
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24); // INPUT
    GPIOC->ODR |= (1 << 14); // Подтяжка вверх
}

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}