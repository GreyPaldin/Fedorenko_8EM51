#include <stdint.h>

#define GPIOA_BASE          0x40010800U
#define GPIOC_BASE          0x40011000U
#define RCC_BASE            0x40021000U

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

int main(void) {
    // ИНИЦИАЛИЗАЦИЯ
    RCC->APB2ENR |= (1 << 2) | (1 << 4); // GPIOA + GPIOC
    GPIOA->CRL = 0x11110000; // PA4-PA7 как выходы
    GPIOC->CRH = 0x44444444; // PC13 и PC14 как входы
    
    // Подтяжка для кнопок
    GPIOC->ODR |= (1 << 13) | (1 << 14);
    
    uint8_t sequence_step = 0;    // 0-4: шаги последовательности
    uint8_t port_mode = 0;        // 0=выход, 1=вход
    uint8_t led1_state = 0;       // Состояние LED1
    uint8_t last_btn1 = 1;        // PC14
    uint8_t last_btn2 = 1;        // PC13
    
    // НАЧАЛЬНОЕ СОСТОЯНИЕ - все светодиоды выключены
    GPIOA->BSRR = (1 << (4+16)) | (1 << (5+16)) | (1 << (6+16)) | (1 << (7+16));
    
    while(1) {
        uint8_t btn1 = !(GPIOC->IDR & (1 << 14)); // Кнопка 1 (PC14)
        uint8_t btn2 = !(GPIOC->IDR & (1 << 13)); // Кнопка 2 (PC13)
        
        // --- КНОПКА 2: Последовательное включение ---
        if (btn2 && !last_btn2) {
            sequence_step++;
            if (sequence_step > 4) {
                sequence_step = 0;
                // При сбросе выключаем ВСЕ светодиоды
                GPIOA->BSRR = (1 << (4+16)) | (1 << (5+16)) | (1 << (6+16)) | (1 << (7+16));
            } else {
                // ВКЛЮЧАЕМ только следующий светодиод, не выключая предыдущие
                switch(sequence_step) {
                    case 1: 
                        GPIOA->BSRR = (1 << 4); // +LED1
                        break;
                    case 2: 
                        GPIOA->BSRR = (1 << 5); // +LED2
                        break;
                    case 3: 
                        GPIOA->BSRR = (1 << 6); // +LED3
                        break;
                    case 4: 
                        GPIOA->BSRR = (1 << 7); // +LED4
                        break;
                }
            }
            
            delay(100000);
        }
        
        // --- КНОПКА 1: Смена режима порта PC14 ---
        if (btn1 && !last_btn1) {
            port_mode = !port_mode;
            
            if (port_mode == 0) {
                // РЕЖИМ ВЫХОДА: PC14 управляет LED1 (PA4)
                GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x3 << 24); // PC14 как выход
                led1_state = !led1_state;
                
                if (led1_state) {
                    GPIOC->BSRR = (1 << 14); // Включаем через PC14
                    GPIOA->BSRR = (1 << 4);  // Включаем LED1
                } else {
                    GPIOC->BSRR = (1 << (14+16)); // Выключаем через PC14
                    GPIOA->BSRR = (1 << (4+16));  // Выключаем LED1
                }
            } else {
                // РЕЖИМ ВХОДА: PC14 читает кнопку
                GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x8 << 24); // PC14 как вход
                GPIOC->ODR |= (1 << 14); // Подтяжка
                
                // Выключаем LED1 при переходе в режим входа
                GPIOA->BSRR = (1 << (4+16));
            }
            
            delay(100000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}