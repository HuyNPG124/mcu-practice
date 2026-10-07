#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Cấu hình cơ bản */
#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      ( 72000000 ) // Xung nhịp 72MHz
#define configTICK_RATE_HZ                      ( 1000 )     // Tần số ngắt OS 1ms
#define configMAX_PRIORITIES                    ( 5 )
#define configMINIMAL_STACK_SIZE                ( 128 )
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1

/* Cấu hình bộ nhớ RAM (Heap) - Cấp 10KB cho OS */
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 10 * 1024 ) )

/* Các hàm Hook (tắt để đơn giản hóa) */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_MALLOC_FAILED_HOOK            0

/* Kích hoạt các hàm API cần thiết */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1

/* ĐẶC BIỆT QUAN TRỌNG: Gắn ngắt của FreeRTOS vào vector ngắt của ARM Cortex-M */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
/* ========================================================== */
/* Cấu hình ngắt đặc thù cho ARM Cortex-M3 (STM32F103)        */
/* ========================================================== */

/* Dòng STM32F103 sử dụng 4 bit để quy định mức ưu tiên ngắt (0-15) */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS         4
#endif

/* Mức ưu tiên ngắt thấp nhất (Thường dành cho Kernel / SysTick) - STM32 là 15 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15

/* Mức ưu tiên ngắt cao nhất được phép gọi hàm API của FreeRTOS (VD: Từ ISR gọi xQueueSendFromISR) */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* FreeRTOS sử dụng 8-bit trên thanh ghi Cortex-M, nên cần dịch trái phần ưu tiên */
#define configKERNEL_INTERRUPT_PRIORITY                 ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY            ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
