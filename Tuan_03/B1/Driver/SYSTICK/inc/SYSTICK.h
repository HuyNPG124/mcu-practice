#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>

#define SYSTICK_BASE 0xE000E010UL

#define SYSTICK_CTRL (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL  (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

void SysTick_Init(uint32_t tick_ms);
uint32_t SysTick_GetTick(void);
void SysTick_DelayMs(uint32_t ms);
void SysTick_Handler(void);

#endif