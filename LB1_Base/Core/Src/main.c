#include "init.h"

int main(void) {
    Init_RCC();
    Init_GPIO();
    
    uint8_t leds_sequence = 1; // Начинаем с 1, чтобы сразу LED2 горел
    uint8_t last_btn1 = 1; // PA0
    uint8_t last_btn2 = 1; // PC14
    
    // Начальное состояние - включаем LED2 сразу
    GPIOA->BSRR = (1 << (6 + 16)) | (1 << (3 + 16)); // Выключаем LED3 и LED4
    GPIOA->BSRR = (1 << 5); // Включаем LED2

    while(1) {
        // Чтение кнопок
        uint8_t btn1 = (GPIOA->IDR & (1 << 0)) == 0; // PA0 - кнопка 1
        uint8_t btn2 = (GPIOC->IDR & (1 << 14)) == 0; // PC14 - кнопка 2
        
        // Обработка кнопки 1 (PA0) - только в режиме кнопки
        if (btn1 && !last_btn1 && pa0_mode == 0) {
            // Увеличиваем шаг последовательности (1-4)
            leds_sequence++;
            if (leds_sequence > 4) {
                leds_sequence = 1;
            }
            
            // Сначала выключаем все светодиоды
            GPIOA->BSRR = (1 << (5 + 16)) | (1 << (6 + 16)) | (1 << (3 + 16));
            
            // Включаем светодиоды согласно последовательности
            switch(leds_sequence) {
                case 1: // LED2
                    GPIOA->BSRR = (1 << 5);
                    break;
                case 2: // LED2 + LED3
                    GPIOA->BSRR = (1 << 5) | (1 << 6);
                    break;
                case 3: // LED2 + LED3 + LED4
                    GPIOA->BSRR = (1 << 5) | (1 << 6) | (1 << 3);
                    break;
                case 4: // Все выключены
                    // Уже выключены выше
                    break;
            }
            
            delay(10000); // Задержка для антидребезга
        }
        
        // Обработка кнопки 2 (PC14) - переключение режима PA0
        if (btn2 && !last_btn2) {
            toggle_PA0_mode();
            
            // БЕЗ мигания светодиодами - просто переключаем режим
            delay(10000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}