#include "init.h"
#include <stdint.h>

// Глобальные переменные
uint8_t pa0_mode = 0;
uint8_t current_led = 0;
uint8_t blink_mode[3] = {0, 0, 0};
uint8_t power_state = 1;
uint32_t blink_counters[3] = {0, 0, 0};
uint8_t led_states[3] = {0, 0, 0}; // Состояния светодиодов

// Пороги для 4 режимов мигания (длительность миганий в режимах)
uint32_t blink_thresholds[4] = {0, 800, 320, 120};

void Init_RCC(void) {
    // Включаем тактирование GPIOA и GPIOC
    RCC->APB2ENR |= (1 << 2) | (1 << 4);
}

void Init_GPIO(void) {
    // PA5, PA6, PA3 как выходы (LED2, LED3, LED4)
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 20)) | (0x3 << 20); // PA5 - LED2
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 24)) | (0x3 << 24); // PA6 - LED3  
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 12)) | (0x3 << 12); // PA3 - LED4
    
    // PC13 как выход (дополнительный светодиод для индикации взаимодействия)
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x3 << 20); // PC13
    
    // PA0 как вход с подтяжкой (кнопка 1) - PA0 в CRL, биты 0-3
    GPIOA->CRL = (GPIOA->CRL & ~(0xF << 0)) | (0x8 << 0);
    GPIOA->ODR |= (1 << 0);
    
    // PC14 как вход с подтяжкой (кнопка 2)
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24);
    GPIOC->ODR |= (1 << 14);
}

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

void toggle_PA0_mode(void) {
    if (pa0_mode == 0) {
        GPIOA->CRL = (GPIOA->CRL & ~(0xF << 0)) | (0x3 << 0);
        GPIOA->BSRR = (1 << 0);
        pa0_mode = 1;
    } else {
        GPIOA->CRL = (GPIOA->CRL & ~(0xF << 0)) | (0x8 << 0);
        GPIOA->ODR |= (1 << 0);
        pa0_mode = 0;
    }
}

void set_led_state(uint8_t led_index, uint8_t state) {
    uint8_t led_pin = (led_index == 0) ? 5 : (led_index == 1) ? 6 : 3;
    
    if (state) {
        GPIOA->BSRR = (1 << led_pin); // Включить
    } else {
        GPIOA->BSRR = (1 << (led_pin + 16)); // Выключить
    }
    
    led_states[led_index] = state;
}

void update_leds(void) {
    // Устанавливаем состояния светодиодов на основе питания и режимов
    for(int i = 0; i < 3; i++) {
        uint8_t has_power = 0;
        switch(i) {
            case 0: has_power = (power_state >= 1); break;
            case 1: has_power = (power_state >= 2); break;
            case 2: has_power = (power_state >= 3); break;
        }
        
        if (has_power) {
            if (blink_mode[i] == 0) {
                // Режим 0 - постоянно горит
                set_led_state(i, 1);
            }
            // Для режимов 1-3 состояние управляется в handle_blink()
        } else {
            // Нет питания - выключаем
            set_led_state(i, 0);
        }
    }
}

void handle_blink(void) {
    // Обновляем счетчики для каждого светодиода
    for(int i = 0; i < 3; i++) {
        blink_counters[i]++;
    }
    
    // Обрабатываем мигание для каждого светодиода
    for(int i = 0; i < 3; i++) {
        // Проверяем разрешение на питание
        uint8_t has_power = 0;
        switch(i) {
            case 0: has_power = (power_state >= 1); break;
            case 1: has_power = (power_state >= 2); break;
            case 2: has_power = (power_state >= 3); break;
        }
        
        if (has_power && blink_mode[i] >= 1 && blink_mode[i] <= 3) {
            uint32_t threshold = blink_thresholds[blink_mode[i]];
            
            // Мигаем с выбранной частотой
            if (blink_counters[i] >= threshold) {
                // Инвертируем состояние
                uint8_t new_state = !led_states[i];
                set_led_state(i, new_state);
                blink_counters[i] = 0;
            }
        }
    }
}

void indicate_led_selection(uint8_t led_index) {
    // Сохраняем текущие состояния
    uint8_t saved_states[3];
    for(int i = 0; i < 3; i++) {
        saved_states[i] = led_states[i];
    }
    
    // Выключаем все
    for(int i = 0; i < 3; i++) {
        set_led_state(i, 0);
    }
    
    // Мигаем PC13 для индикации выбора светодиода
    for(int i = 0; i < 3; i++) {
        GPIOC->BSRR = (1 << 13);
        delay(80000);
        GPIOC->BSRR = (1 << (13 + 16));
        delay(80000);
    }
    
    // Мигаем выбранным светодиодом 5 раз
    for(int i = 0; i < 5; i++) {
        set_led_state(led_index, 1);
        delay(80000);
        set_led_state(led_index, 0);
        delay(80000);
    }
    
    // Восстанавливаем состояния
    for(int i = 0; i < 3; i++) {
        set_led_state(i, saved_states[i]);
    }
}