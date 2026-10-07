#ifndef I2C_H
#define I2C_H

#include <stdint.h> // Bắt buộc phải có dòng này để dùng uint8_t, uint32_t

#define I2C1_BASE       0x40005400U

#define I2C1_CR1        (*(volatile uint32_t *)(I2C1_BASE + 0x00U))
#define I2C1_CR2        (*(volatile uint32_t *)(I2C1_BASE + 0x04U))
#define I2C1_OAR1       (*(volatile uint32_t *)(I2C1_BASE + 0x08U))
#define I2C1_OAR2       (*(volatile uint32_t *)(I2C1_BASE + 0x0CU))
#define I2C1_DR         (*(volatile uint32_t *)(I2C1_BASE + 0x10U))
#define I2C1_SR1        (*(volatile uint32_t *)(I2C1_BASE + 0x14U))
#define I2C1_SR2        (*(volatile uint32_t *)(I2C1_BASE + 0x18U))
#define I2C1_CCR        (*(volatile uint32_t *)(I2C1_BASE + 0x1CU))
#define I2C1_TRISE      (*(volatile uint32_t *)(I2C1_BASE + 0x20U))

#define RCC_BASE        0x40021000U
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x1CU))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18U))

#define GPIOB_BASE      0x40010C00U
#define GPIOB_CRL       (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))

// CHỈ khai báo tên hàm ở đây, kết thúc bằng dấu ;
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAddr(uint8_t addr, uint8_t is_read);
void I2C_WriteData(uint8_t data);
uint8_t I2C_ReadData_Ack(void);
uint8_t I2C_ReadData_Nack(void);

void I2C_Write(uint8_t dev_addr, uint8_t *pData, uint16_t size);
void I2C_Read(uint8_t dev_addr, uint8_t *pData, uint16_t size);

#endif /* I2C_H */
