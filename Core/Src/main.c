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
    RCC->APB2ENR |= (1 << 2) | (1 << 4); // Включаем тактирование GPIOA и GPIOC
    GPIOA->CRL = 0x11110000; // PA4-PA7 как выходы
    GPIOC->CRH = 0x44344444; // PC13 как вход с подтяжкой
    
    // Подтяжка к питанию для PC13
    GPIOC->ODR |= (1 << 13);
    
    // ТЕСТ: 3 мигания
    for(int i = 0; i < 3; i++) {
        GPIOA->BSRR = (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
        delay(500000);
        GPIOA->BSRR = (1 << (4+16)) | (1 << (5+16)) | (1 << (6+16)) | (1 << (7+16));
        delay(500000);
    }
    
    uint8_t current_step = 0;
    uint8_t last_btn = 1;
    
    GPIOA->BSRR = (1 << (4+16)) | (1 << (5+16)) | (1 << (6+16)) | (1 << (7+16));
    
    while(1) {
        // Чтение PC13 вместо PA2
        uint8_t btn = !(GPIOC->IDR & (1 << 13));
        
        if (btn && !last_btn) {
            current_step++;
            if (current_step > 4) current_step = 0;
            
            GPIOA->BSRR = (1 << (4+16)) | (1 << (5+16)) | (1 << (6+16)) | (1 << (7+16));
            
            switch(current_step) {
                case 1: GPIOA->BSRR = (1 << 4); break;
                case 2: GPIOA->BSRR = (1 << 4) | (1 << 5); break;
                case 3: GPIOA->BSRR = (1 << 4) | (1 << 5) | (1 << 6); break;
                case 4: GPIOA->BSRR = (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7); break;
                case 0: break;
            }
            
            delay(100000);
        }
        
        last_btn = btn;
        delay(10000);
    }
}