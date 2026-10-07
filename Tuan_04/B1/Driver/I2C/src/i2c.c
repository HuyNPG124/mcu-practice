#include "i2c.h"

void I2C_Init(void) {
    RCC_APB2ENR |= (1 << 3); 
    RCC_APB1ENR |= (1 << 21);

    GPIOB_CRL &= ~(0xFF000000U); 
    GPIOB_CRL |= 0xFF000000U;    

    I2C1_CR1 |= (1 << 15);       
    I2C1_CR1 &= ~(1 << 15);      

    I2C1_CR2 = 8;                
    I2C1_CCR = 40;               
    I2C1_TRISE = 9;              

    I2C1_CR1 |= (1 << 0);        
}

void I2C_Start(void) {
    I2C1_CR1 |= (1 << 8);        
    while (!(I2C1_SR1 & (1 << 0))); 
}

void I2C_Stop(void) {
    I2C1_CR1 |= (1 << 9);        
}

void I2C_SendAddr(uint8_t addr, uint8_t is_read) {
    I2C1_DR = addr | is_read;
    while (!(I2C1_SR1 & (1 << 1)));
    volatile uint32_t temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp; 
}

void I2C_WriteData(uint8_t data) {
    while (!(I2C1_SR1 & (1 << 7)));
    I2C1_DR = data;
    while (!(I2C1_SR1 & (1 << 2)));
}

// Cấu trúc hàm đọc (Ack và Nack) mà bạn đang viết dở
uint8_t I2C_ReadData_Ack(void) {
    I2C1_CR1 |= (1 << 10); // Bật ACK
    while (!(I2C1_SR1 & (1 << 6))); // Chờ RXNE
    return (uint8_t)I2C1_DR;
}

uint8_t I2C_ReadData_Nack(void) {
    I2C1_CR1 &= ~(1 << 10); // Tắt ACK
    I2C1_CR1 |= (1 << 9);   // Bật STOP
    while (!(I2C1_SR1 & (1 << 6))); // Chờ RXNE
    return (uint8_t)I2C1_DR;
}

void I2C_Write(uint8_t dev_addr, uint8_t *pData, uint16_t size) {
    I2C_Start();
    I2C_SendAddr(dev_addr, 0); 
    
    for (uint16_t i = 0; i < size; i++) {
        while (!(I2C1_SR1 & (1 << 7))); 
        I2C1_DR = pData[i];
    }
    
    while (!(I2C1_SR1 & (1 << 2)));
    I2C_Stop();
}
