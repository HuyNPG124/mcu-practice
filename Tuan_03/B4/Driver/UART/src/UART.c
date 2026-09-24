#include "UART.h"
#include "GPIO.h"
#include "RCC.h"

void UART1_Init(uint32_t baudrate)
{
    RCC_Enable_PortA();
    RCC_Enable_UART1();

    // PA9  = USART1_TX
    // PA10 = USART1_RX

    GPIO_Config(GPIOA, GPIO_PIN_9, GPIO_MODE_AF_PP);
    GPIO_Config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT_FLOATING);

    // USART1 clock = 72 MHz
    USART1_BRR = (72000000UL + baudrate / 2) / baudrate;

    // Reset CR1
    USART1_CR1 = 0;

    // UE = 1
    USART1_CR1 |= (1 << 13);

    // TE = 1
    USART1_CR1 |= (1 << 3);

    // RE = 1
    USART1_CR1 |= (1 << 2);

    // QUAN TRỌNG:
    // KHÔNG bật RXNEIE
    // USART1_CR1 |= (1 << 5);

    // KHÔNG gọi NVIC_UART_En();
}

void UART1_DMA_Enable(void) {
    USART1_CR3 |= (1 << 7);
}

void UART1_SendChar(char c)
{
    while (!(USART1_SR & (1 << 7)))
    {
    }

    USART1_DR = (uint32_t)c;
}


void UART1_SendString(const char *str)
{
    while (*str)
    {
        UART1_SendChar(*str++);
    }
}


char UART1_ReadChar(void)
{
    while (!(USART1_SR & (1 << 5)))
    {
    }

    return (char)(USART1_DR & 0xFF);
}
