#include "init.h"

// Глобальные переменные
uint8_t pa0_mode = 0; // 0=кнопка, 1=выход

void Init_RCC(void) {
    // Включаем тактирование GPIOA и GPIOC
    RCC->APB2ENR |= (1 << 2) | (1 << 4);
}

void Init_GPIO(void) {
    // PA5, PA6, PA3 как выходы (LED2, LED3, LED4)
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 20)) | (0x3 << 20); // PA5 - LED2
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (0x3 << 24); // PA6 - LED3  
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 12)) | (0x3 << 12); // PA3 - LED4
    
    // PA0 как вход с подтяжкой (кнопка 1) - PA0 в CRL, биты 0-3
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 0)) | (0x8 << 0);
    GPIOA->ODR |= (1 << 0);
    
    // PC14 как вход с подтяжкой (кнопка 2)
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24);
    GPIOC->ODR |= (1 << 14);
}

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

void toggle_PA0_mode(void) {
    if (pa0_mode == 0) {
        // Переключаем PA0 в режим вывода
        GPIOA->CRL = (GPIOA->CRL & ~(0xF << 0)) | (0x3 << 0);
        GPIOA->BSRR = (1 << 0); // Устанавливаем HIGH
        pa0_mode = 1;
    } else {
        // Переключаем PA0 в режим входа (кнопка)
        GPIOA->CRL = (GPIOA->CRL & ~(0xF << 0)) | (0x8 << 0);
        GPIOA->ODR |= (1 << 0); // Включаем подтяжку вверх
        pa0_mode = 0;
    }
}