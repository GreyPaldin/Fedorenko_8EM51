#include "init.h"

void Init_RCC(void) {
    // Включение тактирования GPIOA и GPIOC через RCC
    RCC->APB2ENR |= (1 << 2) | (1 << 4);
}

void Init_GPIO(void) {
    // НАСТРОЙКА PA4-PA7 как выходы push-pull (через макросы)
    // PA4: Output push-pull 50MHz
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 16)) | (0x3 << 16);
    // PA5: Output push-pull 50MHz  
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 20)) | (0x3 << 20);
    // PA6: Output push-pull 50MHz
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (0x3 << 24);
    // PA7: Output push-pull 50MHz
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 28)) | (0x3 << 28);
    
    // НАСТРОЙКА PC13 и PC14 через прямое обращение к памяти
    // PC13 как вход с подтяжкой
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x8 << 20);
    GPIOC->ODR |= (1 << 13);
    
    // PC14 как open-drain выход
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x7 << 24);
    GPIOC->ODR |= (1 << 14);
}