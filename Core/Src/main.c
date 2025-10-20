#include "init.h"

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

int main(void) {
    // ИНИЦИАЛИЗАЦИЯ ПЕРИФЕРИИ
    Init_RCC();
    Init_GPIO();
    
    uint8_t sequence_step = 0;
    uint8_t port_mode = 0; // 0=кнопка, 1=светодиод
    uint8_t last_btn1 = 1;
    uint8_t last_btn2 = 1;
    
    // Начальное состояние
    GPIOA->BSRR = 0xF0 << 16; // Все светодиоды PA выкл
    GPIOC->BSRR = (1 << 14); // PC14 в 1 (светодиод выкл)
    
    while(1) {
        uint8_t btn1 = !(GPIOC->IDR & (1 << 14));
        uint8_t btn2 = !(GPIOC->IDR & (1 << 13));
        
        // Кнопка 2: последовательность
        if (btn2 && !last_btn2) {
            sequence_step = (sequence_step + 1) % 5;
            
            GPIOA->BSRR = 0xF0 << 16; // Выкл PA4-PA7
            
            if (sequence_step >= 1) GPIOA->BSRR = (1 << 4);
            if (sequence_step >= 2) GPIOA->BSRR = (1 << 5);
            if (sequence_step >= 3) GPIOA->BSRR = (1 << 6); 
            if (sequence_step >= 4) GPIOA->BSRR = (1 << 7);
            
            if (port_mode == 1) {
                GPIOC->BSRR = (1 << (14+16));
            }
            
            delay(200000);
        }
        
        // Кнопка 1: смена режима
        if (btn1 && !last_btn1) {
            port_mode = !port_mode;
            
            if (port_mode) {
                GPIOC->BSRR = (1 << (14+16));
            } else {
                GPIOC->BSRR = (1 << 14);
            }
            
            delay(200000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}