#ifndef __AS5600_H
#define __AS5600_H
#include "stm32f4xx.h"
void delay_s(uint32_t i);

uint8_t AS5600_ReadOneByte(uint8_t addr);
uint16_t AS5600_ReadRawAngleTwo(void)

#endif