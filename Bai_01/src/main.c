#define SYSTICK_CTRL  (*(volatile unsigned int *)0xE000E010)
#define SYSTICK_LOAD  (*(volatile unsigned int *)0xE000E014)
#define SYSTICK_VAL   (*(volatile unsigned int *)0xE000E018) 
#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018) 
#define GPIOC_CRH     (*(volatile unsigned int *)0x40011004) // Cấu hình chế độ cho PC8 - PC15
#define GPIOC_ODR     (*(volatile unsigned int *)0x4001100C) // Xuất tín hiệu đầu ra

volatile unsigned int msTicks = 0;

void SysTick_Handler(void) {
    if(msTicks > 0){
        msTicks--;
    }
}

void SysTick_Init(void) {
    SYSTICK_LOAD = 8000 - 1;
    SYSTICK_VAL = 0;
    SYSTICK_CTRL = 7;
}


void Delay_ms(unsigned int delay) {
    msTicks = delay;
    while (msTicks != 0);
}

void SystemInit(void) {
} 

void _init(void) {
}

void GPIO_Init_PC13_Output(void) {
    RCC_APB2ENR |= (1 << 4);
    GPIOC_CRH &= ~(0xF << 20); 
    GPIOC_CRH |= (2 << 20);
}

int main(void) {
    SysTick_Init();
    GPIO_Init_PC13_Output();   
    
    while(1) {
        GPIOC_ODR ^= (1 << 13);
        Delay_ms(200);
    }
    return 0;
}
