//
// Created by lynliam on 23-7-3.
//

#ifndef STM32F407_DEMO2_MY_SPI_H
#define STM32F407_DEMO2_MY_SPI_H

#include "main.h"
#include "gpio.h"
#include "bsp_delay.h"

#define  SPI_CS_PIN          GPIO_PIN_1
#define  SPI_CS_PORT         GPIOD

#define  SPI_CLK_PIN         GPIO_PIN_0
#define  SPI_CLK_PORT        GPIOD

#define  SPI_MOSI_PIN        GPIO_PIN_3
#define  SPI_MOSI_PORT       GPIOD

#define  SPI_MISO_PIN        GPIO_PIN_4
#define  SPI_MISO_PORT       GPIOD

#define  SPI_FLASH_CS_LOW()    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN,GPIO_PIN_RESET)  /* Select SPI MEM1: ChipSelect pin low  */
#define  SPI_FLASH_CS_HIGH()   HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET)    /* Deselect SPI MEM1: ChipSelect pin high */

void SendandReceiveByte(uint8_t byte);
void SPI_PIN_Init(void);
void SPI_SendByte(uint8_t byte);

#endif //STM32F407_DEMO2_MY_SPI_H
