#define SYSTICK_CTRL  (*(volatile unsigned int *)0xE000E010)
#define SYSTICK_LOAD  (*(volatile unsigned int *)0xE000E014)
#define SYSTICK_VAL   (*(volatile unsigned int *)0xE000E018) 
#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018) 
#define GPIOA_CRL     (*(volatile unsigned int *)0x40010800) // Cấu hình PA0 - PA7
#define GPIOA_CRH     (*(volatile unsigned int *)0x40010804) // Cấu hình chế độ cho PC8 - PC15
#define GPIOA_IDR     (*(volatile unsigned int *)0x40010808) // Đọc dữ liệu ngõ vào
#define GPIOA_ODR     (*(volatile unsigned int *)0x4001080C) // Xuất tín hiệu đầu ra

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

void GPIO_Init_PortA(void) {
    RCC_APB2ENR |= (1 << 2);
    GPIOA_CRL = 0x88888888;
    GPIOA_CRH = 0x22222222; 
    GPIOA_ODR |= 0x00FF;
}

int main(void) {
    GPIO_Init_PortA();   
    GPIO_Init_PortB();
    while(1) {
        unsigned int input_data = GPIOA_IDR & 0xFF;
        unsigned int inverted_data = (~input_data) & 0xFF;
        GPIOA_ODR = (GPIOA_ODR & 0x00FF) | (inverted_data << 8);
        // Trễ nhẹ để chống nhiễu cơ học
        for(volatile int i = 0; i < 50000; i++);
    }
    return 0;
}
