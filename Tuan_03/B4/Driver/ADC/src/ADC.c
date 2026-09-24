#include "ADC.h"
#include "RCC.h"

#define GPIOA_CRL *((volatile uint32_t *)0x40010800)

void ADC01_CH3_Init(void)
{
    RCC_Enable_PortA();
    RCC_Enable_ADC01();

    GPIOA_CRL &= ~(uint32_t)(0xF << 12);

    ADC01_CR1 = 0;

    ADC01_SMPR2 &= ~(uint32_t)(0x7 << 9);
    ADC01_SMPR2 |= (uint32_t)(0x2 << 9);

    ADC01_SQR1 &= ~(uint32_t)(0xF << 20);

    ADC01_SQR3 &= ~(uint32_t)(0x1F);
    ADC01_SQR3 |= 3;

    ADC01_CR2 = 0;

    ADC01_CR2 |= (1 << 1);

    ADC01_CR2 |= (1 << 0);

    for (volatile uint32_t i = 0; i < 10000; i++)
    {
    }

    ADC01_CR2 |= (1 << 3);

    while (ADC01_CR2 & (1 << 3))
    {
    }

    ADC01_CR2 |= (1 << 2);

    while (ADC01_CR2 & (1 << 2))
    {
    }

    ADC01_CR2 |= (1 << 0);

    ADC01_CR2 |= (1 << 22);
}

uint16_t ADC01_CH3_Read(void)
{
    while (!(ADC01_SR & (1 << 1)))
    {
    }

    return (uint16_t)(ADC01_DR & 0x0FFF);
}