/*
 * @Author: ZYT
 * @Date: 2024-07-19 12:40:45
 * @LastEditors: ZYT
 * @LastEditTime: 2024-07-30 22:58:01
 * @FilePath: \Gantry_Trial\UserCode\Upper\Protect\reset.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by zyt, All Rights Reserved. 
 */
#include "reset.h"

void Reset_Task(void *argument)
{
    uint16_t flag = 0;
    while (1) {
        uint16_t ResetFlag = HAL_GPIO_ReadPin(Reset_GPIO_Port, Reset_Pin);
        uint16_t ResetFlag1 = HAL_GPIO_ReadPin(Reset1_GPIO_Port, Reset1_Pin);
        uint16_t ResetFlag2 = HAL_GPIO_ReadPin(Reset2_GPIO_Port, Reset2_Pin);
        if (ResetFlag && ResetFlag1 && ResetFlag2) {
            // float degree1x = Core_xy[0].Motor_X->AxisData.AxisAngle_inDegree;
            // float degree2x = Core_xy[1].Motor_Y->AxisData.AxisAngle_inDegree;
            // float degree1y = Core_xy[0].Motor_X->AxisData.AxisAngle_inDegree;
            // float degree2y = Core_xy[1].Motor_Y->AxisData.AxisAngle_inDegree;
            if (flag == 0) {
                stateflag = 100;

                pid_reset(&(LightFury.Motor_X->speedPID), 3, 0.3, 0.3);
                pid_reset(&(LightFury.Motor_YL->speedPID), 5, 0.4, 0.3);
                pid_reset(&(LightFury.Motor_YR->speedPID), 5, 0.4, 0.3);

                pid_reset(&(LightFury.Motor_X->posPID), -20, 0, 0);
                pid_reset(&(LightFury.Motor_YL->posPID), -30, 0, 0);
                pid_reset(&(LightFury.Motor_YR->posPID), 30, 0, 0);

                flag = 1;
            } else if (flag == 1) {
                HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 1);
                HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                HAL_GPIO_WritePin(ElectromagnetX_GPIO_Port, ElectromagnetX_Pin, 0);
                HAL_GPIO_WritePin(ElectromagnetYR_GPIO_Port, ElectromagnetYL_Pin, 0);
                HAL_GPIO_WritePin(ElectromagnetYL_GPIO_Port, ElectromagnetYL_Pin, 0);

                LightFury.gantry_t.position.x  = 1390;
                LightFury.gantry_t.position.yL = 590;
                LightFury.gantry_t.position.yR = 590;
            } 
        }
    }
}

void Reset_Start(void)
{
    osThreadId_t ResetHandle;
    const osThreadAttr_t Reset_attributes = {
        .name       = "Reset",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };

    ResetHandle = osThreadNew(Reset_Task, NULL, &Reset_attributes);
}