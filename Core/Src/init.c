#include "init.h"

void Init_RCC(void) {
    // Включение тактирования GPIOA и GPIOC через RCC
    RCC->APB2ENR |= (1 << 2) | (1 << 4);
}

void Init_GPIO(void) {
    // PA5-PA7 как выходы push-pull (светодиоды 2,3,4)
    // PA5: Output push-pull 50MHz  
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 20)) | (0x3 << 20);
    // PA6: Output push-pull 50MHz
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (0x3 << 24);
    // PA7: Output push-pull 50MHz
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 28)) | (0x3 << 28);
    
    // PC13 как вход с подтяжкой (кнопка 2)
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x8 << 20);
    GPIOC->ODR |= (1 << 13);
    
    // PC14 как вход с подтяжкой (изначально режим кнопки)
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24);
    GPIOC->ODR |= (1 << 14);
}