#include "init.h"
#include <stdint.h>

int main(void) {
    Init_RCC();
    Init_GPIO();
    
    uint8_t last_btn1 = 1;
    uint8_t last_btn2 = 1;
    uint32_t btn2_press_time = 0;
    uint8_t btn2_long_press = 0;
    
    // Начальное состояние
    power_state = 1;
    update_leds();

    while(1) {
        // Чтение кнопок
        uint8_t btn1 = (GPIOA->IDR & (1 << 0)) == 0;
        uint8_t btn2 = (GPIOC->IDR & (1 << 14)) == 0;
        
        // Обработка кнопки 1 (PA0) - управление питанием
        if (btn1 && !last_btn1) {
            power_state = (power_state + 1) % 4;
            update_leds(); // Обновляем состояния на основе нового питания
            delay(100000);
        }
        
        // Обработка кнопки 2 (PC14)
        if (btn2 && !last_btn2) {
            btn2_press_time = 0;
            btn2_long_press = 0;
            
            // Отладка: начало нажатия
            GPIOC->BSRR = (1 << 13);
            delay(10000);
            GPIOC->BSRR = (1 << (13 + 16));
        }
        
        if (btn2) {
            btn2_press_time++;
            
            // Долгое нажатие (~0.1 секунды)
            if (btn2_press_time > 1000 && !btn2_long_press) {
                current_led = (current_led + 1) % 3;
                btn2_long_press = 1;
                
                // Отладка: долгое нажатие распознано
                for(int i = 0; i < 3; i++) {
                    GPIOC->BSRR = (1 << 13);
                    delay(50000);
                    GPIOC->BSRR = (1 << (13 + 16));
                    delay(50000);
                }
                
                indicate_led_selection(current_led);
            }
        }
        
        if (!btn2 && last_btn2) {
            // Кнопка отпущена
            if (!btn2_long_press && btn2_press_time > 10) {
                // Короткое нажатие - меняем режим мигания
                blink_mode[current_led] = (blink_mode[current_led] + 1) % 4;
                blink_counters[current_led] = 0;
                
                // Отладка: короткое нажатие распознано
                for(int i = 0; i < 2; i++) {
                    GPIOC->BSRR = (1 << 13);
                    delay(30000);
                    GPIOC->BSRR = (1 << (13 + 16));
                    delay(30000);
                }
                
                // Обновляем состояние светодиода после смены режима
                update_leds();
            }
            // Сбрасываем флаги
            btn2_long_press = 0;
            btn2_press_time = 0;
        }
        
        // Обработка мигания всех светодиодов ПАРАЛЕЛЬНО
        handle_blink();
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(100);
    }
}