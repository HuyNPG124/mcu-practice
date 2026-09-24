#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
} TIM_Typedef;

#define TIM2_BASE       0x40000000UL
#define TIM2            ((TIM_Typedef *)TIM2_BASE)

#define RCC_BASE        0x40021000UL
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x1CUL))

#define TIM2_CH1        1
#define TIM2_CH2        2
#define TIM2_CH3        3
#define TIM2_CH4        4

void TIM2_PWM_Init(void);
void TIM2_PWM_SetDuty(uint8_t channel, uint8_t duty);
void TIM2_Start(void);
void TIM2_Stop(void);

#endif