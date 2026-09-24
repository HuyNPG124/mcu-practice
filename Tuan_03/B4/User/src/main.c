#include <stdint.h>
#include "RCC.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "TIM.h"
#include "ADC.h"
#include "DMA.h"

#define BUFFER_SIZE 100  // 100 mẫu/giây

uint16_t adc_buffer[BUFFER_SIZE];
volatile uint8_t ht_flag = 0; // Cờ Half-Transfer
volatile uint8_t tc_flag = 0; // Cờ Transfer-Complete

// Hàm chuyển đổi số nguyên thành chuỗi và truyền qua UART
void UART_SendNumber(uint16_t num) {
    char buf[6];
    int i = 0;
    
    if (num == 0) buf[i++] = '0'; 
    
    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Đảo ngược chuỗi số
    for (int j = 0; j < i / 2; j++) {
        char t = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    buf[i] = '\0';
    
    UART1_SendString(buf);
    UART1_SendString("\n\r");
}

// Trình phục vụ ngắt DMA1 Kênh 1
void DMA1_Channel1_IRQHandler(void) {
    // Kiểm tra cờ Half-Transfer
    if (DMA1_ISR & DMA1_CH1_HTIF) {
        DMA1_IFCR = DMA1_CH1_HTIF; // Xóa cờ
        ht_flag = 1;
    }
    
    // Kiểm tra cờ Transfer-Complete
    if (DMA1_ISR & DMA1_CH1_TCIF){
        DMA1_IFCR = DMA1_CH1_TCIF; // Xóa cờ
        tc_flag = 1;
    }
}

// Khai báo hàm rỗng để trình biên dịch không báo lỗi khi thiếu thư viện stdlib
void SystemInit(void) {}
void _init(void) {}

int main(void) {
    // 1. Cấp xung nhịp cho các ngoại vi 
    RCC->RCC_AHBENR.BITS.DMA1EN = 1;
    RCC->APB2ENR.BITS.IOPAEN = 1;   
    RCC->APB2ENR.BITS.ADC1EN = 1;   
    RCC->APB2ENR.BITS.USART1EN = 1; 
    RCC->APB1ENR.BITS.TIM3EN = 1;   
    
    RCC_Enable_AFIO();
    RCC_Config_72Mhz();

    // 2. Khởi tạo GPIO 
    GPIO_Config(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT_ANALOG);   // PA3 (Kênh 3) cho ADC
    GPIO_Config(GPIOA, GPIO_PIN_9, GPIO_MODE_AF_PP);          // PA9 cho UART TX
    GPIO_Config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT_FLOATING); // PA10 cho UART RX

    // 3. Khởi tạo UART1
    UART1_Init(115200);

    // 4. Khởi tạo DMA cho ADC 
    DMA1_Channel1_Init(adc_buffer, BUFFER_SIZE);
    DMA1_CCR1 |= DMA_CCR_HTIE | DMA_CCR_TCIE;    // Bật ngắt Half-Transfer và Transfer-Complete
    
    // Kích hoạt ngắt DMA trên NVIC 
    *(volatile uint32_t*)0xE000E100 |= (1 << DMA1_CHANNEL1_IRQ);

    // 5. Khởi tạo Timer 3 (Tần số 100Hz = 10ms)
    // Cấu hình trực tiếp thanh ghi do TIM.h 
    #define TIM3_BASE 0x40000400
    #define TIM3_CR1  (*(volatile uint32_t *)(TIM3_BASE + 0x00))
    #define TIM3_CR2  (*(volatile uint32_t *)(TIM3_BASE + 0x04))
    #define TIM3_PSC  (*(volatile uint32_t *)(TIM3_BASE + 0x28))
    #define TIM3_ARR  (*(volatile uint32_t *)(TIM3_BASE + 0x2C))

    TIM3_PSC = 800 - 1;         // 8MHz / 800 = 10kHz
    TIM3_ARR = 100 - 1;         // 10kHz / 100 = 100Hz
    TIM3_CR2 &= ~(7 << 4);      // Xóa các bit MMS
    TIM3_CR2 |= (2 << 4);       // Đặt MMS = 010 (Sự kiện Update làm tín hiệu TRGO)

    // 6. Khởi tạo ADC1 (Dựa trên ADC.h)
    ADC01_CH3_Init();
    ADC01_SQR3 = 3;
    // Cấu hình bổ sung cho ADC: Kích hoạt DMA và Trigger từ TIM3 TRGO
    ADC01_CR2 |= (1 << 8);              // Bật DMA (Bit 8)
    ADC01_CR2 |= (1 << 20);             // Bật External Trigger (EXTTRIG - Bit 20)
    ADC01_CR2 &= ~(7 << 17);            // Xóa cài đặt EXTSEL cũ
    ADC01_CR2 |= (4 << 17);             // Chọn TIM3 TRGO (EXTSEL = 100)

    // 7. Kích hoạt toàn bộ hệ thống
    DMA1_Channel1_Start();
    TIM3_CR1 |= (1 << 0);  // Kích hoạt bộ đếm TIM3 (CEN)

    // 8. Vòng lặp chính xử lý dữ liệu (Double Buffering)
    while(1) {
        if (ht_flag) {
            ht_flag = 0;
            // Xử lý và truyền nửa đầu mảng trong khi DMA an toàn ghi vào nửa sau
            for (int i = 0; i < BUFFER_SIZE / 2; i++) {
                UART_SendNumber(adc_buffer[i]);
            }
        }
        
        if (tc_flag) {
            tc_flag = 0;
            // Xử lý và truyền nửa sau mảng trong khi DMA vòng lại ghi đè nửa đầu
            for (int i = BUFFER_SIZE / 2; i < BUFFER_SIZE; i++) {
                UART_SendNumber(adc_buffer[i]);
            }
        }
    }
}
