#ifndef INIT_H
#define INIT_H

#include <stdint.h>

// Базовые адреса периферии
#define PERIPH_BASE       0x40000000U
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000U)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x20000U)

// Адреса GPIO
#define GPIOA_BASE        (APB2PERIPH_BASE + 0x0800U)
#define GPIOC_BASE        (APB2PERIPH_BASE + 0x1000U)
#define RCC_BASE          (AHBPERIPH_BASE + 0x1000U)

// Структуры регистров
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

// Указатели на периферию
#define GPIOA            ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOC            ((GPIO_TypeDef *)GPIOC_BASE)
#define RCC              ((RCC_TypeDef *)RCC_BASE)

// Объявление глобальных переменных
extern uint8_t pa0_mode;
extern uint8_t current_led;
extern uint8_t blink_mode[3];
extern uint8_t power_state;
extern uint32_t blink_counters[3];
extern uint32_t blink_thresholds[4];
extern uint8_t led_states[3]; // Состояния светодиодов (0-выкл, 1-вкл)

void Init_RCC(void);
void Init_GPIO(void);
void delay(uint32_t count);
void toggle_PA0_mode(void);
void update_leds(void);
void handle_blink(void);
void indicate_led_selection(uint8_t led_index);
void set_led_state(uint8_t led_index, uint8_t state);

#endif