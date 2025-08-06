//
// Created by lynliam on 23-7-3.
//

#include "My_spi.h"

//模拟MOSI
void SPI_MOSI(unsigned char Status)
{
    if(Status)
        HAL_GPIO_WritePin(SPI_MOSI_PORT,SPI_MOSI_PIN,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(SPI_MOSI_PORT,SPI_MOSI_PIN,GPIO_PIN_RESET);
}
//模拟CLK
void SPI_CLK(unsigned char Status)
{
    if(Status)
        HAL_GPIO_WritePin(SPI_CLK_PORT,SPI_CLK_PIN,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(SPI_CLK_PORT,SPI_CLK_PIN,GPIO_PIN_RESET);
}

//模拟MISO
unsigned char SPI_MISO(void)
{
    if(HAL_GPIO_ReadPin(SPI_MOSI_PORT,SPI_MOSI_PIN))
        return 1;
    else
        return 0;
}

void SPI_PIN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStructure.Pin = SPI_MOSI_PIN; //MOSI 写入，主机向从机
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
    GPIO_InitStructure.Pull=GPIO_NOPULL;
    HAL_GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
    SPI_MOSI(1);

    GPIO_InitStructure.Pin = SPI_CLK_PIN; //SCK
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
    HAL_GPIO_Init(SPI_CLK_PORT, &GPIO_InitStructure);
    SPI_CLK(0);

    GPIO_InitStructure.Pin = SPI_MISO_PIN ;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_CS_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);
    SPI_FLASH_CS_HIGH();
}

void SendandReceiveByte(uint8_t byte)
{
    SPI_FLASH_CS_LOW();
    SPI_MOSI(0);
    Delay_us(1);
    for (uint16_t bit = 0x8000; bit; bit >>= 1) {
            if (byte & bit) {
                SPI_MOSI(1);
            } else {
                SPI_MOSI(0);
            }
            // 上升沿时钟
            SPI_CLK(1);
            //Delay_us(1);
            SPI_CLK(0);
            //Delay_us(1);
    }
    SPI_MOSI_PORT->MODER &= ~(3<<(2*3));
    SPI_MOSI_PORT->MODER |=  (0<<(2*3));
    SPI_MOSI_PORT->PUPDR &= ~(3<<(2*3));
    SPI_MOSI_PORT->PUPDR |=  (1<<(2*3));
    Delay_us(1);


    uint8_t data[4];
    for(int j=0;j<4;j++)
    {
        for(int i=0;i<8;i++)
        {
            data[j]<<=1;
            if(SPI_MISO())
                data[j]|=0x01;
            
            SPI_CLK(1);
            //Delay_us(1);
            SPI_CLK(0);
            //Delay_us(1);
        }
    }
    SPI_MOSI_PORT->MODER &= ~(3<<(2*3));
    SPI_MOSI_PORT->MODER |=  (1<<(2*3));
    SPI_FLASH_CS_HIGH();
    printf("data[0]:%x,data[1]:%x,data[2]:%x,data[3]:%x\n",data[0],data[1],data[2],data[3]);
}

void SPI_SendByte(uint8_t byte) {
    uint8_t bit;

    for (bit = 0x80; bit; bit >>= 1) {
        if (byte & bit) {
            SPI_MOSI(1);
        } else {
            SPI_MOSI(0);
        }
        // 上升沿时钟
        SPI_CLK(0);
        Delay_us(1);
        SPI_CLK(1);
        Delay_us(1);
    }
}
