#include "init.h"

void Init_RCC(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 4);
}

void Init_GPIO(void) {
    // PA2 (LED1) - ИНИЦИАЛИЗАЦИЯ ЧЕРЕЗ РЕГИСТРЫ (напрямую)
    // Output push-pull 50MHz: MODE=11 (0x3), CNF=00
    // PA2 находится в CRL, биты 8-11: MODE2[1:0] и CNF2[1:0]
    *(volatile uint32_t*)(0x40010800U + 0x00) = (*(volatile uint32_t*)(0x40010800U + 0x00) & ~(0xF << 8)) | (0x3 << 8);
    
    // PA3, PA5, PA6 выходы push-pull 50MHz (LED4, LED2, LED3)
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 12)) | (0x3 << 12); // PA3 - LED4
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 20)) | (0x3 << 20); // PA5 - LED2  
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (0x3 << 24); // PA6 - LED3
    
    // PC13 как вход с подтяжкой (Кнопка 1)
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x8 << 20);
    GPIOC->ODR |= (1 << 13); 
    
    // PC14 как вход с подтяжкой (Кнопка 2) 
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24);
    GPIOC->ODR |= (1 << 14); //
}

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}