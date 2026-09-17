#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

uint8_t rx_data;             // Biến lưu 1 ký tự nhận được
char rx_buffer[50];          // Bộ đệm chứa cả chuỗi lệnh
uint8_t rx_index = 0;
uint8_t cmd_ready = 0;       // Cờ báo hiệu đã nhận đủ 1 lệnh (gặp dấu !)

uint8_t is_on = 0;           // Trạng thái LED (0 = OFF, 1 = ON)
uint8_t current_pwm = 50;    // Duty cycle hiện tại (mặc định 50%)

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
// Bật PWM Kênh 1 (PA0)
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

// Bắt đầu tắt LED (Duty = 0)
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

// Kích hoạt ngắt UART lần đầu tiên
HAL_UART_Receive_IT(&huart1, &rx_data, 1);

  while (1)
  {

   if (cmd_ready) {
    char tx_buf[50];
    
    // 1. Lệnh ON/OFF[cite: 1]
    if (strcmp(rx_buffer, "ON") == 0) {
        is_on = 1;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, current_pwm * 10); // Sáng theo cấu hình gần nhất[cite: 1]
        HAL_UART_Transmit(&huart1, (uint8_t*)"LED ON\r\n", 8, 100);
    } 
    else if (strcmp(rx_buffer, "OFF") == 0) {
        is_on = 0;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); // Tắt đèn
        HAL_UART_Transmit(&huart1, (uint8_t*)"LED OFF\r\n", 9, 100);
    } 
    
    // 2. Lệnh PWM (VD: PWM:70%)[cite: 1]
    else if (strncmp(rx_buffer, "PWM:", 4) == 0) {
        int val = atoi(&rx_buffer[4]); // Lấy con số sau chữ "PWM:"
        if (val >= 0 && val <= 100) {
            current_pwm = val; // Chỉ đổi cấu hình
            
            // Nếu đang ON thì cập nhật độ sáng thực tế luôn, nếu OFF thì giữ nguyên[cite: 1]
            if (is_on) {
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, current_pwm * 10);
            }
            sprintf(tx_buf, "Set PWM = %d%%\r\n", current_pwm);
            HAL_UART_Transmit(&huart1, (uint8_t*)tx_buf, strlen(tx_buf), 100);
        }
    } 
    
    // 3. Lệnh Status[cite: 1]
    else if (strcmp(rx_buffer, "Status") == 0) {
        sprintf(tx_buf, "Trang thai: %s, Do sang: %d%%\r\n", is_on ? "ON" : "OFF", current_pwm);
        HAL_UART_Transmit(&huart1, (uint8_t*)tx_buf, strlen(tx_buf), 100);
    } 
    
    // Lệnh không hợp lệ
    else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"Sai lenh!\r\n", 11, 100);
    }

    // Xóa bộ đệm và mở lại ngắt để đón lệnh mới
    rx_index = 0;
    cmd_ready = 0;
    HAL_UART_Receive_IT(&huart1, &rx_data, 1);
   }
  } 
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim2);

}


static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (rx_data == '!') { 
            rx_buffer[rx_index] = '\0'; // Chốt chuỗi
            cmd_ready = 1;              // Bật cờ xử lý
        } 
        // LỌC KÝ TỰ: Bỏ qua phím Enter (\r, \n) và khoảng trắng (Space)
        else if (rx_data != '\r' && rx_data != '\n' && rx_data != ' ') {
            if (rx_index < 49) {
                rx_buffer[rx_index++] = rx_data; // Chỉ lưu chữ và số
            }
        }
        
        // Tiếp tục mở ngắt đón ký tự tiếp theo
        if (!cmd_ready) {
            HAL_UART_Receive_IT(&huart1, &rx_data, 1);
        }
    }
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
}
#ifdef
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif 
