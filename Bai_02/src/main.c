#define SYSTICK_CTRL  (*(volatile unsigned int *)0xE000E010)
#define SYSTICK_LOAD  (*(volatile unsigned int *)0xE000E014)
#define SYSTICK_VAL   (*(volatile unsigned int *)0xE000E018) 
#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018)
#define GPIOA_CRL     (*(volatile unsigned int *)0x40010800) 
#define GPIOA_ODR     (*(volatile unsigned int *)0x4001080C)

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

void GPIO_Init_PA0_PA7_Output(void) {
    RCC_APB2ENR |= (1 << 2);
    GPIOA_CRL = 0x22222222; 
}

int main(void) {
    SysTick_Init();
    GPIO_Init_PA0_PA7_Output();   
    
    int position = 0;
    int direction = 1; // 1: Trái sang phải, -1: Phải sang trái
    
    while(1) {
        // Đẩy duy nhất 1 bit ở mức High (1) tới vị trí hiện tại
        GPIOA_ODR = (1 << position);
        Delay_ms(200);
        // Cập nhật vị trí cho chu kỳ tiếp theo
        position += direction;
        if (position == 7) {
            direction = -1; 
        } else if (position == 0) {
            direction = 1;  
        }
    }
    return 0;
}
