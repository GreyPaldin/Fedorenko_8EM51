#include "init.h"

// УБИРАЕМ определение переменной отсюда - она уже определена в init.c

int main(void) {
    Init_RCC();
    Init_GPIO();
    
    uint8_t sequence_step = 0;
    uint8_t button_mode = 0; // 0=последовательность, 1=управление LED1
    uint8_t led1_state = 0; // состояние LED1 в режиме управления
    uint8_t last_btn1 = 1; // PC13 (кнопка 1)
    uint8_t last_btn2 = 1; // PC14 (кнопка 2)
    
    // Начальное состояние - все светодиоды выключены
    GPIOA->BSRR = (1 << (3 + 16)) | (1 << (5 + 16)) | (1 << (6 + 16)); // PA3, PA5, PA6
    set_LED1_state(0); // Выключить LED1 на PC14

    // Мигание всеми светодиодами при старте
    GPIOA->BSRR = (1 << 3) | (1 << 5) | (1 << 6); // PA3, PA5, PA6 вкл
    set_LED1_state(1); // LED1 вкл
    delay(500000);
    GPIOA->BSRR = (1 << (3 + 16)) | (1 << (5 + 16)) | (1 << (6 + 16)); // PA3, PA5, PA6 выкл
    set_LED1_state(0); // LED1 выкл
    delay(500000);

    while(1) {
        // Чтение кнопок (0 = нажата, 1 = отпущена)
        uint8_t btn1 = (GPIOC->IDR & (1 << 13)) == 0; // PC13 - кнопка 1
        uint8_t btn2 = read_BTN2(); // PC14 - кнопка 2 (специальная функция)
        
        // КНОПКА 1 
        if (btn1 && !last_btn1) {
            if (button_mode == 0) {
                // РЕЖИМ 0: Последовательность всех светодиодов
                sequence_step = (sequence_step + 1) % 5;
                
                // Выключить все светодиоды перед включением новых
                GPIOA->BSRR = (1 << (3 + 16)) | (1 << (5 + 16)) | (1 << (6 + 16));
                set_LED1_state(0);
                
                // Включить по последовательности
                switch(sequence_step) {
                    case 1: 
                        set_LED1_state(1); // LED1 на PC14
                        break;
                    case 2: 
                        set_LED1_state(1); // LED1
                        GPIOA->BSRR = (1 << 5); // LED2 на PA5
                        break;
                    case 3: 
                        set_LED1_state(1); // LED1
                        GPIOA->BSRR = (1 << 5) | (1 << 6); // LED2 + LED3
                        break;
                    case 4: 
                        set_LED1_state(1); // LED1
                        GPIOA->BSRR = (1 << 3) | (1 << 5) | (1 << 6); // Все остальные
                        break;
                    case 0: 
                        break;
                }
                
            } else {
                // РЕЖИМ 1: Управление только LED1
                led1_state = !led1_state;
                set_LED1_state(led1_state);
            }
            
            delay(10000);
        }
        
        // КНОПКА 2 (PC14): смена режима работы кнопки 1
        if (btn2 && !last_btn2) {
            // Сохраняем текущее состояние LED1 перед миганием
            uint8_t led1_was_on = led1_current_state;
            
            button_mode = !button_mode; // Переключаем режим
            
            // Индикация смену режима миганием LED1
            for(int i = 0; i < (button_mode == 0 ? 2 : 3); i++) {
                set_LED1_state(1); // LED1 вкл
                delay(150000);
                set_LED1_state(0); // LED1 выкл
                delay(150000);
            }
            
            // ВОССТАНАВЛИВАЕМ состояние LED1 после мигания
            set_LED1_state(led1_was_on);
            
            delay(10000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}

// Функция для установки состояния LED1 (PC14)
void set_LED1_state(uint8_t state) {
    led1_current_state = state;
    if (state) {
        // Включить LED1: переводим PC14 в OUTPUT и устанавливаем HIGH
        GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x3 << 24); // OUTPUT 50MHz
        GPIOC->BSRR = (1 << 14); // HIGH
    } else {
        // Выключить LED1: переводим PC14 в OUTPUT и устанавливаем LOW
        GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x3 << 24); // OUTPUT 50MHz
        GPIOC->BSRR = (1 << (14 + 16)); // LOW
    }
}

// Функция для чтения кнопки 2 (PC14)
uint8_t read_BTN2(void) {
    // Сохраняем текущее состояние светодиода
    uint8_t saved_led_state = led1_current_state;
    
    // Гасим LED1 и переводим пин в INPUT для чтения кнопки
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24); // INPUT with pull-up/pull-down
    GPIOC->ODR |= (1 << 14); // Активируем подтяжку вверх
    
    // Небольшая задержка для стабилизации уровня
    delay(100);
    
    // Читаем состояние кнопки
    uint8_t btn_state = (GPIOC->IDR & (1 << 14)) == 0;
    
    // НЕМЕДЛЕННО возвращаем пин в режим вывода и восстанавливаем состояние LED1
    set_LED1_state(saved_led_state);
    
    return btn_state;
}