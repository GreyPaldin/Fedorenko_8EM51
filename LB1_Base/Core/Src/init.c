#include "init.h"

// Глобальные переменные
uint8_t pa0_mode = 0; // 0 - кнопка, 1 - выход

void GPIO_Init(void) {
    // 1. Включаем тактирование GPIOA и GPIOC - РАЗЛОЖЕННЫЕ РЕГИСТРЫ
    // RCC_APB2ENR = PERIPH_BASE + AHBPERIPH_BASE + RCC_BASE + APB2ENR_offset
    // = 0x40000000 + 0x00020000 + 0x00001000 + 0x00000018 = 0x40021018
    *(volatile uint32_t*)(0x40000000U + 0x00020000U + 0x00001000U + 0x00000018U) |= (1 << 2) | (1 << 4);
    
    // 2. PA2 (LED1) - РАЗЛОЖЕННЫЕ РЕГИСТРЫ БЕЗ МАКРОСОВ
    // GPIOA_CRL = PERIPH_BASE + APB2PERIPH_BASE + GPIOA_BASE + CRL_offset
    // = 0x40000000 + 0x00010000 + 0x00000800 + 0x00000000 = 0x40010800
    *(volatile uint32_t*)(0x40000000U + 0x00010000U + 0x00000800U + 0x00000000U) = 
        (*(volatile uint32_t*)(0x40000000U + 0x00010000U + 0x00000800U + 0x00000000U) & ~(0xF << 8)) | (0x3 << 8);
    
    // 3. PA5 (LED2) - через ручные макросы
    GPIOA_CRL = (GPIOA_CRL & GPIOA_MODE_PA5_MASK) | GPIOA_MODE_PA5_OUT;
    
    // 4. PA6 (LED3) - через ручные макросы
    GPIOA_CRL = (GPIOA_CRL & GPIOA_MODE_PA6_MASK) | GPIOA_MODE_PA6_OUT;
    
    // 5. PA3 (LED4) - через ручные макросы
    GPIOA_CRL = (GPIOA_CRL & GPIOA_MODE_PA3_MASK) | GPIOA_MODE_PA3_OUT;
    
    // 6. PA0 (Кнопка 1) - через ручные макросы
    GPIOA_CRL = (GPIOA_CRL & GPIOA_MODE_PA0_MASK) | GPIOA_MODE_PA0_INPUT;
    SET_BIT(GPIOA_ODR, GPIOA_PULLUP_PA0);
    
    // 7. PC14 (Кнопка 2) - через ручные макросы
    GPIOC_CRH = (GPIOC_CRH & GPIOC_MODE_PC14_MASK) | GPIOC_MODE_PC14_INPUT;
    SET_BIT(GPIOC_ODR, GPIOC_PULLUP_PC14);
    
    // 8. Начальное состояние - все светодиоды выключены
    GPIOA_BSRR = GPIOA_BSRR_PA2_RESET;  // Выкл PA2 (через регистры)
    GPIOA_BSRR = GPIOA_BSRR_PA5_RESET;  // Выкл PA5
    GPIOA_BSRR = GPIOA_BSRR_PA6_RESET;  // Выкл PA6  
    GPIOA_BSRR = GPIOA_BSRR_PA3_RESET;  // Выкл PA3
}

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

void toggle_PA0_mode(void) {
    if (pa0_mode == 0) {
        // Переключаем PA0 в режим вывода
        GPIOA_CRL = (GPIOA_CRL & GPIOA_MODE_PA0_MASK) | GPIOA_MODE_PA0_OUTPUT;
        GPIOA_BSRR = GPIOA_PULLUP_PA0;  // Устанавливаем HIGH
        pa0_mode = 1;
    } else {
        // Переключаем PA0 в режим входа
        GPIOA_CRL = (GPIOA_CRL & GPIOA_MODE_PA0_MASK) | GPIOA_MODE_PA0_INPUT;
        SET_BIT(GPIOA_ODR, GPIOA_PULLUP_PA0);
        pa0_mode = 0;
    }
}