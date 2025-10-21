#include "init.h"

void Init_RCC(void) {
    // Включение тактирования GPIOA и GPIOC через макросы
    RCC->APB2ENR |= (1 << 2) | (1 << 4);
}

void Init_GPIO(void) {
    // PA5 (LED2) - ИНИЦИАЛИЗАЦИЯ ЧЕРЕЗ РЕГИСТРОВ
    // Output push-pull 50MHz: MODE=11 (0x3), CNF=00
    *(volatile uint32_t*)(0x40010800U + 0x00) = (*(volatile uint32_t*)(0x40010800U + 0x00) & ~(0xF << 20)) | (0x3 << 20);
    
    // PA6-PA7 (LED3, LED4)
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (PIN_MODE_OUTPUT_50MHz << 24);
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 28)) | (PIN_MODE_OUTPUT_50MHz << 28);
    
    
    // PC13 как вход с подтяжкой
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | ((PIN_MODE_INPUT | CNF_PULL_UPDOWN) << 20);
    GPIOC->ODR |= (1 << 13);
    
    // PC14 как вход с подтяжкой
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | ((PIN_MODE_INPUT | CNF_PULL_UPDOWN) << 24);
    GPIOC->ODR |= (1 << 14);
}