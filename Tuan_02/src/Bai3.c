#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018)
#define GPIOA_CRL     (*(volatile unsigned int *)0x40010800)
#define GPIOA_CRH     (*(volatile unsigned int *)0x40010804)
// Thanh ghi ADC1
#define ADC1_SR       (*(volatile unsigned int *)0x40012400)
#define ADC1_CR2      (*(volatile unsigned int *)0x40012408)
#define ADC1_SQR3     (*(volatile unsigned int *)0x40012434)
#define ADC1_DR       (*(volatile unsigned int *)0x4001244C)
// Thanh ghi Systick
#define STK_CTRL      (*(volatile unsigned int *)0xE000E010)
#define STK_LOAD      (*(volatile unsigned int *)0xE000E014)
#define STK_VAL       (*(volatile unsigned int *)0xE000E018)
// UART
#define USART1_SR      (*(volatile unsigned int *)0x40013800) //trang thai
#define USART1_DR      (*(volatile unsigned int *)0x40013804) //Du lieu
#define USART1_BRR      (*(volatile unsigned int *)0x40013808) //Baudrate
#define USART1_CR1      (*(volatile unsigned int *)0x4001380C) //Dieu khien

void USART1_Init(void){
    RCC_APB2ENR |= (1 << 14) | (1 << 2);
    GPIOA_CRH &= ~(0xFF << 4);
    GPIOA_CRH |= (0x0B << 4) | (0x04 << 8);
    USART1_BRR = 0x341;
    USART1_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}
void USART1_SendString(char* str){
    while(*str){
        while(!(USART1_SR & (1 << 7)));
        USART1_DR = *str++;
    }
}
void ADC1_Init(void){
    RCC_APB2ENR |= (1 << 9) | (1 << 2);
    GPIOA_CRL &= ~(0xF << 0);
    ADC1_CR2 |= (1 << 0);
    ADC1_SQR3 = 0;
}

unsigned int ADC1_Read(void){
    ADC1_CR2 |= (1 << 0);
    while(!(ADC1_SR & (1 << 1)));
    return ADC1_DR;
}

void Delay(void){
    STK_LOAD = 8000000 - 1;
    STK_VAL = 0;
    STK_CTRL = 5;
    while(!(STK_CTRL & (1 << 16)));
    STK_CTRL = 0;
}
void UART1_SendNumber(int num){
    char buf[10];
    int i = 0;
    if(num == 0){USART1_SendString("0"); return; }
    while (num > 0){
        buf[i++] = (num % 10) +'0';
        num /= 10;
    }
    while (i > 0){
        while(!(USART1_SR & (1 << 7)));
        USART1_DR = buf[--i];
    }
}
void SystemInit(void) {}

void _init(void) {
}

int main(void) {
    USART1_Init();
    ADC1_Init();
    while(1) {
        unsigned int adc_val = ADC1_Read();
        unsigned int volt_x100 = (adc_val * 330) / 4095;
        int phan_nguyen = volt_x100 / 100;
        int phan_thap_phan = volt_x100 % 100;
        USART1_SendString("Dien ap: ");
        UART1_SendNumber(phan_nguyen);    
        USART1_SendString(".");
        if(phan_thap_phan < 10) USART1_SendString("0");
        UART1_SendNumber(phan_thap_phan);
        USART1_SendString(" V\n\r");
        Delay();
    }
    return 0;
}
