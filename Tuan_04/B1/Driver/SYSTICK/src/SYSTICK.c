#include "SYSTICK.h"

#define SYSTEM_CLOCK_HZ 72000000UL

static volatile uint32_t g_tick = 0;

void SysTick_Init(uint32_t tick_ms)
{
    SYSTICK_LOAD = (SYSTEM_CLOCK_HZ / 1000UL) * tick_ms - 1UL;
    SYSTICK_VAL = 0;

    SYSTICK_CTRL =
        (1 << 2) |
        (1 << 1) |
        (1 << 0);

    g_tick = 0;
}

void SysTick_Handler(void)
{
    g_tick++;
}

uint32_t SysTick_GetTick(void)
{
    return g_tick;
}

void SysTick_DelayMs(uint32_t ms)
{
    uint32_t start = SysTick_GetTick();

    while ((SysTick_GetTick() - start) < ms)
    {
    }
}