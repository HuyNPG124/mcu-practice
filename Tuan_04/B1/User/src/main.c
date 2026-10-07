#include "FreeRTOS.h"
#include "task.h"
#include "RCC.h"  
#include "GPIO.h" 
#include <stddef.h> 

// Định nghĩa cấu hình chân LED 
#define LED1_PIN GPIO_PIN_0
#define LED2_PIN GPIO_PIN_1
#define LED3_PIN GPIO_PIN_2

// Tự triển khai hàm memcpy và memset cho FreeRTOS (Do dùng -nostdlib)
void *memcpy(void *dest, const void *src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

// ==========================================================
// CÁC TASK XỬ LÝ ĐỘC LẬP
// ==========================================================

// Task 1: Nháy LED 0.1Hz (Đảo trạng thái mỗi 5000ms)
void Task_LED_0_1Hz(void *pvParameters) {
    while (1) {
        GPIO_Toggle_Pin(GPIOA, LED1_PIN);
        vTaskDelay(pdMS_TO_TICKS(5000));  
    }
}

// Task 2: Nháy LED 1Hz (Đảo trạng thái mỗi 500ms)
void Task_LED_1Hz(void *pvParameters) {
    while (1) {
        GPIO_Toggle_Pin(GPIOA, LED2_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));   
    }
}

// Task 3: Nháy LED 10Hz (Đảo trạng thái mỗi 50ms)
void Task_LED_10Hz(void *pvParameters) {
    while (1) {
        GPIO_Toggle_Pin(GPIOA, LED3_PIN);
        vTaskDelay(pdMS_TO_TICKS(50));    
    }
}

// ==========================================================
// HÀM MAIN
// ==========================================================
int main(void) {
    // 1. Cấu hình xung nhịp phần cứng
    RCC_Config_72Mhz();
    RCC_Enable_PortA();

    // 2. Cấu hình 3 chân GPIO ở chế độ Output Push-Pull
    GPIO_Config(GPIOA, LED1_PIN, GPIO_MODE_OUTPUT_PP);
    GPIO_Config(GPIOA, LED2_PIN, GPIO_MODE_OUTPUT_PP);
    GPIO_Config(GPIOA, LED3_PIN, GPIO_MODE_OUTPUT_PP);

    // 3. Khởi tạo các Task trong FreeRTOS
    xTaskCreate(Task_LED_0_1Hz, "LED_0.1Hz", 128, NULL, 1, NULL);
    xTaskCreate(Task_LED_1Hz,   "LED_1Hz",   128, NULL, 1, NULL);
    xTaskCreate(Task_LED_10Hz,  "LED_10Hz",  128, NULL, 1, NULL);

    // 4. Bắt đầu bộ lập lịch (Scheduler)
    vTaskStartScheduler();

    while (1) {}
}
