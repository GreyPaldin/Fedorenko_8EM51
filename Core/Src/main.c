#include "init.h"

int main(void) {
    Init_RCC();
    Init_GPIO();
    
    uint8_t sequence_step = 0;
    uint8_t button_mode = 0; // 0=последовательность, 1=управление LED1
    uint8_t led1_state = 0; // состояние LED1 в режиме управления
    uint8_t last_btn1 = 1; // PC13 (кнопка 1)
    uint8_t last_btn2 = 1; // PC14 (кнопка 2)
    
    // Начальное состояние - все светодиоды выключены
    GPIOA->BSRR = (1 << (2 + 16)) | (1 << (3 + 16)) | (1 << (5 + 16)) | (1 << (6 + 16));

    // Мигание всеми светодиодами при старте
    GPIOA->BSRR = (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6); // Все вкл
    delay(500000);
    GPIOA->BSRR = (1 << (2 + 16)) | (1 << (3 + 16)) | (1 << (5 + 16)) | (1 << (6 + 16)); // Все выкл
    delay(500000);

    while(1) {
        // Чтение кнопок (0 = нажата, 1 = отпущена)
        uint8_t btn1 = (GPIOC->IDR & (1 << 13)) == 0; // PC13 - кнопка 1
        uint8_t btn2 = (GPIOC->IDR & (1 << 14)) == 0; // PC14 - кнопка 2
        
        // КНОПКА 1 
        if (btn1 && !last_btn1) {
            if (button_mode == 0) {
                // РЕЖИМ 0: Последовательность всех светодиодов
                sequence_step = (sequence_step + 1) % 5;
                
                // Выключить все светодиоды перед включением новых
                GPIOA->BSRR = (1 << (2 + 16)) | (1 << (3 + 16)) | (1 << (5 + 16)) | (1 << (6 + 16));
                
                // Включить по последовательности
                switch(sequence_step) {
                    case 1: 
                        GPIOA->BSRR = (1 << 2); // LED1
                        break;
                    case 2: 
                        GPIOA->BSRR = (1 << 2) | (1 << 5); // LED1 + LED2
                        break;
                    case 3: 
                        GPIOA->BSRR = (1 << 2) | (1 << 5) | (1 << 6); // LED1 + LED2 + LED3
                        break;
                    case 4: 
                        GPIOA->BSRR = (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6); // Все
                        break;
                    case 0: 
                        break;
                }
                
            } else {
                // РЕЖИМ 1: Управление только LED1
                led1_state = !led1_state;
                
                if (led1_state) {
                    GPIOA->BSRR = (1 << 2); // Включить LED1
                } else {
                    GPIOA->BSRR = (1 << (2 + 16)); // Выключить LED1
                }
            }
            
            delay(10000);
        }
        
        // КНОПКА 2 (PC14): смена режима работы кнопки 1
        if (btn2 && !last_btn2) {
            // Сохраняем текущее состояние LED1 перед миганием
            uint8_t led1_was_on = (GPIOA->ODR & (1 << 2)) != 0;
            
            button_mode = !button_mode; // Переключаем режим
            
            // Индикация смены режима миганием LED1
            for(int i = 0; i < (button_mode == 0 ? 2 : 3); i++) {
                GPIOA->BSRR = (1 << 2); // LED1 вкл
                delay(15000);
                GPIOA->BSRR = (1 << (2 + 16)); // LED1 выкл
                delay(15000);
            }
            
            // ВОССТАНАВЛЕНИЕ состояние LED1 после мигания
            if (led1_was_on) {
                GPIOA->BSRR = (1 << 2); 
            } else {
                GPIOA->BSRR = (1 << (2 + 16)); 
            }
            
            led1_state = led1_was_on;
            
            delay(10000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}