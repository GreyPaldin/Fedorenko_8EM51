#include "init.h"

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

// Мигание встроенным светодиодом платы (PC13)
void blink_internal_led(uint8_t times) {
    // Временно переключаем PC13 в выход для мигания
    uint32_t temp_crh = GPIOC->CRH;
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x3 << 20); // Push-pull выход
    
    for(int i = 0; i < times; i++) {
        GPIOC->BSRR = (1 << 13); // Включить светодиод (1 - выключен, т.к. подключен к питанию)
        delay(100000);
        GPIOC->BSRR = (1 << (13 + 16)); // Выключить светодиод (0 - включен)
        delay(100000);
    }
    
    // Возвращаем в режим входа с подтяжкой
    GPIOC->CRH = temp_crh;
    GPIOC->ODR |= (1 << 13); // Подтяжка к питанию
}

int main(void) {
    Init_RCC();
    Init_GPIO();
    
    uint8_t sequence_step = 0;
    uint8_t port_mode = 0; // 0=кнопка, 1=светодиод
    uint8_t last_btn1 = 1;
    uint8_t last_btn2 = 1;
    
    // Начальное состояние
    GPIOA->BSRR = 0xE0 << 16; // PA5-PA7 выкл
    
    // Убедимся что PC14 в режиме кнопки
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24); // Вход с подтяжкой
    GPIOC->ODR |= (1 << 14); // Подтяжка к питанию
    
    blink_internal_led(1); // 1 мигание - старт
    
    while(1) {
        uint8_t btn1 = !(GPIOC->IDR & (1 << 14)); // PC14
        uint8_t btn2 = !(GPIOC->IDR & (1 << 13)); // PC13
        
        // Кнопка 1 (PC14): смена режима
        if (btn1 && !last_btn1) {
            port_mode = !port_mode;
            blink_internal_led(3); // 3 мигания - режим сменился
            
            if (port_mode == 1) {
                // Переключаем в режим светодиода PUSH-PULL
                GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x3 << 24); // Push-pull 50MHz
                // Включаем LED1 если должен гореть
                if (sequence_step >= 1 && sequence_step <= 4) {
                    GPIOC->BSRR = (1 << 14); // Включить (1 для push-pull)
                } else {
                    GPIOC->BSRR = (1 << (14 + 16)); // Выключить (0)
                }
            } else {
                // Возвращаем в режим кнопки
                GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24); // Вход
                GPIOC->ODR |= (1 << 14); // Подтяжка
            }
            
            delay(200000);
        }
        
        // Кнопка 2 (PC13): последовательность
        if (btn2 && !last_btn2) {
            sequence_step = (sequence_step + 1) % 5;
            blink_internal_led(2); // 2 мигания - последовательность сменилась
            
            // Выключить все PA5-PA7
            GPIOA->BSRR = 0xE0 << 16;
            
            // Включить нужные светодиоды
            switch(sequence_step) {
                case 1: 
                    // Только LED1
                    if (port_mode == 1) GPIOC->BSRR = (1 << 14); // Включить LED1
                    break;
                case 2: 
                    // LED1 + LED2
                    if (port_mode == 1) GPIOC->BSRR = (1 << 14); // LED1
                    GPIOA->BSRR = (1 << 5); // LED2
                    break;
                case 3: 
                    // LED1 + LED2 + LED3
                    if (port_mode == 1) GPIOC->BSRR = (1 << 14); // LED1
                    GPIOA->BSRR = (1 << 5) | (1 << 6); // LED2, LED3
                    break;
                case 4: 
                    // Все светодиоды
                    if (port_mode == 1) GPIOC->BSRR = (1 << 14); // LED1
                    GPIOA->BSRR = (1 << 5) | (1 << 6) | (1 << 7); // LED2, LED3, LED4
                    break;
                case 0: 
                    // Все выключено
                    if (port_mode == 1) GPIOC->BSRR = (1 << (14 + 16)); // Выключить LED1
                    break;
            }
            
            delay(200000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}