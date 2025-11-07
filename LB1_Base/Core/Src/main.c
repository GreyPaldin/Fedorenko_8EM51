#include "init.h"

int main(void) {
    GPIO_Init();
    
    uint8_t leds_sequence = 1; // Начинаем с LED2
    uint8_t last_btn1 = 1;
    uint8_t last_btn2 = 1;
    
    while(1) {
        //Чтение кнопок
        uint8_t btn1 = (READ_BIT(GPIOA_IDR, GPIOA_IDR_PA0) == 0);
        uint8_t btn2 = (READ_BIT(GPIOC_IDR, GPIOC_IDR_PC14) == 0);
        
        //Обработка кнопки 1 (PA0) - только в режиме кнопки
        if (btn1 && !last_btn1 && pa0_mode == 0) {
            leds_sequence++;
            if (leds_sequence > 4) {
                leds_sequence = 1;
            }
            
            //Выключаем все светодиоды
            GPIOA_BSRR = GPIOA_BSRR_PA2_RESET | GPIOA_BSRR_PA5_RESET | 
                         GPIOA_BSRR_PA6_RESET | GPIOA_BSRR_PA3_RESET;
            
            //Активация светодиодов в зависимости от состояния системы
            switch(leds_sequence) {
                case 1: //LED2
                    GPIOA_BSRR = GPIOA_BSRR_PA5_SET;
                    break;
                case 2: //LED2 + LED3
                    GPIOA_BSRR = GPIOA_BSRR_PA5_SET | GPIOA_BSRR_PA6_SET;
                    break;
                case 3: //LED2 + LED3 + LED4
                    GPIOA_BSRR = GPIOA_BSRR_PA5_SET | GPIOA_BSRR_PA6_SET | GPIOA_BSRR_PA3_SET;
                    break;
                case 4: //LED1 (PA2) - ТОЛЬКО ЧЕРЕЗ РЕГИСТРЫ
                    GPIOA_BSRR = GPIOA_BSRR_PA2_SET;
                    break;
            }
            
            delay(10000);
        }
        
        //Обработка кнопки 2 (PC14) - переключение режима PA0
        if (btn2 && !last_btn2) {
            toggle_PA0_mode();
            delay(10000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}