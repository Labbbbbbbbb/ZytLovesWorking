#ifndef STEP
#define STEP
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "main.h"
void gpio_step(uint8_t dir,uint16_t us,uint16_t angle);   //  angle:4细分
void delay_us(uint16_t us);


#endif // !1