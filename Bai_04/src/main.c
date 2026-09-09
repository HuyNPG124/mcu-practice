#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018)
#define GPIOA_CRL     (*(volatile unsigned int *)0x40010800)
#define GPIOA_IDR     (*(volatile unsigned int *)0x40010808)
#define GPIOA_ODR     (*(volatile unsigned int *)0x4001080C)

#define GPIOC_CRH     (*(volatile unsigned int *)0x40011004)
#define GPIOC_ODR     (*(volatile unsigned int *)0x4001100C)

// Hàm tạo trễ tương đối bằng vòng lặp đếm chay
void Delay_ms(volatile int time) {
    for(volatile int i = 0; i < time * 4000; i++);
}

void GPIO_Init(void) {
    RCC_APB2ENR |= (1 << 2) | (1 << 4);
    GPIOA_CRL &= ~(0xF << 0); 
    GPIOA_CRL |= (0x8 << 0);  
    GPIOA_ODR |= (1 << 0);    
    GPIOC_CRH &= ~(0xF << 20); 
    GPIOC_CRH |= (0x2 << 20);
}

void SystemInit(void) {
}

void _init(void) {
}

int main(void) {
    GPIO_Init();
    
    while(1) {
        if ((GPIOA_IDR & (1 << 0)) == 0) {
            Delay_ms(20); 
            if ((GPIOA_IDR & (1 << 0)) == 0) {
                while ((GPIOA_IDR & (1 << 0)) == 0);
                Delay_ms(20); 
                GPIOC_ODR ^= (1 << 13);
            }
        }
    }
    return 0;
}
