#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018)
#define GPIOA_CRH     (*(volatile unsigned int *)0x40010804)
#define USART1_SR     (*(volatile unsigned int *)0x40013800)
#define USART1_DR     (*(volatile unsigned int *)0x40013804)
#define USART1_BRR    (*(volatile unsigned int *)0x40013808)
#define USART1_CR1    (*(volatile unsigned int *)0x4001380C)

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

void SystemInit(void) {}
void _init(void) {}

int main(void) {
    USART1_Init();
    char buffer[100];
    int index = 0;
    
    while(1) {
        if(USART1_SR & (1 << 5)) {
            char c = USART1_DR;
            if(c == '|'){
                buffer[index] = '\0';
                USART1_SendString("VM02_Nhom6: ");
                USART1_SendString(buffer);
                USART1_SendString("\n\r");
                index = 0;
            } else {
                if(index < 99){
                    buffer[index++] = c;
                }
            }
        }
    }
    return 0;
}
