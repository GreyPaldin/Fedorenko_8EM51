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
    
    // PA4-PA7 как выходы push-pull (обычные светодиоды)
    GPIOA->CRL = 0x11110000;
    
    // PC14 как open-drain с подтяжкой
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 24)) | (0x7 << 24); // PC14: open-drain output
    GPIOC->ODR |= (1 << 14); // Подтяжка к VCC
    
    // PC13 как вход с подтяжкой
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (0x8 << 20); 
    GPIOC->ODR |= (1 << 13);
    
    uint8_t sequence_step = 0;
    uint8_t port_mode = 0; // 0=кнопка, 1=светодиод
    uint8_t last_btn1 = 1;
    uint8_t last_btn2 = 1;
    
    // Начальное состояние
    GPIOA->BSRR = 0xF0 << 16; // Все светодиоды выкл
    GPIOC->BSRR = (1 << 14); // PC14 в 1 (светодиод выкл)
    
    while(1) {
        uint8_t btn1 = !(GPIOC->IDR & (1 << 14));
        uint8_t btn2 = !(GPIOC->IDR & (1 << 13));
        
        // Кнопка 2: последовательность
if (btn2 && !last_btn2) {
    sequence_step = (sequence_step + 1) % 5;
    
    // Сбрасываем только светодиоды PA4-PA7, НЕ трогая PC14
    GPIOA->BSRR = 0xF0 << 16; // Выкл PA4-PA7
    
    // Включаем нужные светодиоды на PA
    if (sequence_step >= 1) GPIOA->BSRR = (1 << 4);
    if (sequence_step >= 2) GPIOA->BSRR = (1 << 5);
    if (sequence_step >= 3) GPIOA->BSRR = (1 << 6); 
    if (sequence_step >= 4) GPIOA->BSRR = (1 << 7);
    
    // ВАЖНО: если PC14 в режиме светодиода - восстанавливаем его состояние
    if (port_mode == 1) {
        GPIOC->BSRR = (1 << (14+16)); // Зажигаем светодиод на PC14
    }
    
    delay(200000);
}
        
        // Кнопка 1: смена режима
        if (btn1 && !last_btn1) {
            port_mode = !port_mode;
            
            if (port_mode) {
                // РЕЖИМ СВЕТОДИОДА: зажигаем
                GPIOC->BSRR = (1 << (14+16)); // 0 на выходе - светодиод горит
            } else {
                // РЕЖИМ КНОПКИ: выключаем
                GPIOC->BSRR = (1 << 14); // 1 на выходе - светодиод не горит
            }
            
            delay(200000);
        }
        
        last_btn1 = btn1;
        last_btn2 = btn2;
        delay(10000);
    }
}