#include <stdint.h>

/* --- ĐỊNH NGHĨA ĐỊA CHỈ THANH GHI STM32F103 & CORTEX-M3 --- */
#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x40010800
#define SYSTICK_BASE    0xE000E010

// Thanh ghi RCC & GPIO
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))

// Thanh ghi SysTick
#define SYST_CSR        (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYST_RVR        (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYST_CVR        (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

// Biến đếm thời gian cho 3 LED
volatile uint32_t cnt_led1 = 0; // LED 1 (PA0)
volatile uint32_t cnt_led2 = 0; // LED 2 (PA1)
volatile uint32_t cnt_led3 = 0; // LED 3 (PA2)

// Trình phục vụ ngắt SysTick (xảy ra mỗi 1ms)
void SysTick_Handler(void) {
    cnt_led1++;
    cnt_led2++;
    cnt_led3++;

    // LED 1 (PA0): Nháy 2Hz -> Đảo trạng thái mỗi 250ms
    if (cnt_led1 >= 250) {
        cnt_led1 = 0;
        GPIOA_ODR ^= (1 << 0);
    }

    // LED 2 (PA1): Nháy 1Hz -> Đảo trạng thái mỗi 500ms
    if (cnt_led2 >= 500) {
        cnt_led2 = 0;
        GPIOA_ODR ^= (1 << 1);
    }

    // LED 3 (PA2): Nháy 0.2Hz -> Đảo trạng thái mỗi 2500ms
    if (cnt_led3 >= 2500) {
        cnt_led3 = 0;
        GPIOA_ODR ^= (1 << 2);
    }
}

int main(void) {
    // 1. Bật Clock cấp cho Port A (Bit 2 của RCC_APB2ENR)
    RCC_APB2ENR |= (1 << 2);

    // 2. Cấu hình PA0, PA1, PA2 là Output Push-Pull 10MHz
    GPIOA_CRL &= ~0x00000FFF; // Xóa cấu hình cũ của PA0, PA1, PA2
    GPIOA_CRL |=  0x00000111; // Gán MODE = 01 (10MHz), CNF = 00 (Push-Pull)

    // 3. Cấu hình SysTick 1ms (Giả thiết thạch anh nội HSI 8MHz: 8000 xung = 1ms)
    SYST_RVR = 8000 - 1;       // Giá trị đếm nạp lại
    SYST_CVR = 0;              // Xóa giá trị đếm hiện tại
    SYST_CSR = 0x07;           // Enable SysTick, Enable Ngắt, Dùng Clock hệ thống

    // 4. Vòng lặp chính
    while (1) {
        __asm__("wfi"); // Chờ ngắt
    }
}
