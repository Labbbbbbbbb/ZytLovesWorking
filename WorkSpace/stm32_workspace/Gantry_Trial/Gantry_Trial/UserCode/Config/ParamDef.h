/*
 * @Author: ZYT
 * @Date: 2024-07-20 21:31:26
 * @LastEditors: ZYT
 * @LastEditTime: 2024-08-10 15:09:07
 * @FilePath: \Gantry_Trial\UserCode\Config\ParamDef.h
 * @Brief: 
 * 
 * Copyright (c) 2024 by zyt, All Rights Reserved. 
 */

#ifndef __PARAMDEF_H__
#define __PARAMDEF_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "UpperStart.h"
#define BELT_LENGTH_PER_ROUND 95.5044167  //输出轴转一圈同步带走的长度，单位:mm

typedef __IO struct 
{
    __IO struct {
        double x;
        double yL;
        double yR;
    } velocity; // 速度结构体//不知道需不需要来着

    __IO struct {
        double x;
        double yL;
        double yR;
    } position;
    SemaphoreHandle_t xMutex_control; // 互斥锁
}GantryState;


typedef __IO struct {
    DJI_t *Motor_X;
    DJI_t *Motor_YL;
    DJI_t *Motor_YR;
    _Bool cylinder_L;
    _Bool cylinder_M;
    _Bool cylinder_R;
    _Bool electromagnet_L;
    _Bool electromagnet_M;
    _Bool electromagnet_R;
    GantryState gantry_t;
} Gantry_COMPONENT;

extern Gantry_COMPONENT LightFury;       //全局的Gantry状态参数


extern float current_posX;
extern float current_posYL;
extern float current_posYR;
extern uint16_t UartFlag[6];

extern uint8_t usart1_rx[1];
extern uint8_t usart2_rx[1];
extern uint8_t usart3_rx[1];
extern uint8_t usart4_rx[1];
extern uint8_t usart6_rx[1];

extern uint8_t Rxbuffer_1[195];
extern uint8_t Rxbuffer_2[195];
extern uint8_t Rxbuffer_3[195];
extern uint8_t Rxbuffer_4[195];
extern uint8_t Rxbuffer_6[195];

extern LidarPointTypedef Lidar1;
extern LidarPointTypedef Lidar2;
extern LidarPointTypedef Lidar3;
extern LidarPointTypedef Lidar4;
extern LidarPointTypedef Lidar6;

extern  float remem_12xR;
extern  float remem_12xL;

extern uint16_t inner_ring_flag;
extern uint8_t receive_buffer[24];
extern float weight_placement[5];
extern uint16_t Uart_State;
extern uint16_t stateflag;

extern uint16_t detectflag;

extern float rem;
extern float rem_motor;
extern uint16_t motor_flag;

#endif // __PARAMDEF_H__