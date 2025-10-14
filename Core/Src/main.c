#include "../../CMSIS\Devices\STM32F1xx\Inc\STM32f103xB\stm32f103xb.h"
#include "../../CMSIS/Devices/STM32F1xx/Inc/stm32f1xx.h"

#include "stm32f1xx.h"

int main(void) {
    // 1. Включить тактирование порта C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    // 2. Настройка PC13 как Output Push-Pull (2MHz)
    GPIOC->CRH &= ~GPIO_CRH_CNF13;  // Очистить биты CNF (00: Output push-pull)
    GPIOC->CRH |= GPIO_CRH_MODE13_0; // MODE = 01 (Output mode, max speed 2 MHz)
    
    // 3. ЗАЖЕЧЬ СВЕТОДИОД (установить PC13 в 0 - светодиод зажигается)
    GPIOC->BSRR = GPIO_BSRR_BR13;
    
    // 4. Бесконечный цикл (ничего не делать)
    while(1) {
        // Просто висим, светодиод горит
    }
}