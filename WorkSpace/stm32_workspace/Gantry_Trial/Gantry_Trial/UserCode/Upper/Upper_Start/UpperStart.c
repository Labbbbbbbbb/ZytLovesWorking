/*
 * @Author: ZYT
 * @Date: 2024-05-19 14:41:19
 * @LastEditors: ZYT
 * @LastEditTime: 2024-08-20 11:50:27
 * @FilePath: \Gantry_Trial\UserCode\Upper\Upper_Start\UpperStart.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by zyt, All Rights Reserved. 
 */

#include "UpperStart.h"

void StartDefaultTask(void *argument)
{
    // Uart_State = 1; // 后续要删！
    /*等待树莓派数据*/
    RaspReceive_Enable();
    //UsartUpdate_Rasp_Start();

    /*串口使能*/
    Usart_start();
    UsartUpdate_Start();
    osDelay(3000);
    for (;;) {
        if (Uart_State == 1) {
            break;
        }else
        {
            HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
        }
    }
    /*初始化函数*/
    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
    HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 1);

    osDelay(1000);
    LightFury_Motor_init();
    /*开启线程*/
    StateMachine_Start();
    Upper_Servo_Start();
    Reset_Start();
    /*如需复位程序请记得开启线程*/
    for (;;) {
        // HAL_UART_Transmit(&huart5, "hello", 6, 0xffff);
        printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
               Lidar1.distance_aver,
               Lidar2.distance_aver,
               Lidar3.distance_aver,
               rem,
               LightFury.gantry_t.position.x,
               detectflag,
               LightFury.Motor_X->posPID.KP,
               receive_buffer[0],
               weight_placement[0],
               weight_placement[1],
               weight_placement[2],
               weight_placement[3],
               weight_placement[4]);
        // printf("%f,%f\n", LightFury.Motor_YR->AxisData.AxisAngle_inDegree,remem_12xR);
        // printf("%f,%f,%f,%f,%f,%d\n", weight_placement[0], weight_placement[1], weight_placement[2],
        //       weight_placement[3], weight_placement[4], receive_buffer[0]);
        osDelay(50);
        }
}
