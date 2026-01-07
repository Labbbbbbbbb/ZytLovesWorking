/*** 
 * @Author: doge60 3020118317@qq.com
 * @Date: 2025-12-13 20:45:06
 * @LastEditors: doge60 3020118317@qq.com
 * @LastEditTime: 2025-12-19 00:23:44
 * @FilePath: \bipedwheel\User\RemoteCtr\RemoteCtr.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by doge60 3020118317@qq.com, All Rights Reserved. 
 */
#ifndef REMOTECTR_H
#define REMOTECTR_H

#include "stm32f4xx.h"   
#include "usart.h"
#include "stdio.h"
#include "bsp_delay.h"

#define BUFFER_SIZE 18

extern uint8_t rx_temp;
extern uint8_t rx_buffer[BUFFER_SIZE];
extern volatile uint8_t buffer_received; //接收标志位
extern uint8_t buffer_index;

extern float forward_ref;
extern float turn_ref;

void ProcessRemoteBuffer(uint8_t *(buffer));


#endif // REMOTECTR_H