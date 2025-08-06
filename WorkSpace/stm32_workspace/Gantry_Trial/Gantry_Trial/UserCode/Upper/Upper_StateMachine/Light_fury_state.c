/*
 * @Author: ZYT
 * @Date: 2024-07-20 21:33:49
 * @LastEditors: ZYT
 * @LastEditTime: 2024-08-20 15:48:08
 * @FilePath: \Gantry_Trial\UserCode\Upper\Upper_StateMachine\Light_fury_state.c
 * @Brief:
 *
 * Copyright (c) 2024 by zyt, All Rights Reserved.
 */
#include "Light_fury_state.h"

#define X_maxvelocity   4000
#define Y_maxvelocity   300
#define X_Acceleration  500
#define Y_Acceleration  60

#define OFFSET12       50
#define OFFSET34       50      //100


float initial_posX;
float initial_posYL;
float initial_posYR;
float current_posX;
float current_posYL;
float current_posYR;
uint16_t tickflag;
uint16_t tickflag_x;
uint16_t starttick;

uint16_t stateflag ;

uint16_t detectflag;
float rem;

uint16_t once;
uint16_t once02;
void StateMachine_Task(void *argument)
{
    osDelay(100);//距离要重新规划一下防止拖行太长，以及pid要重新调
    /*距离变量*/
    float mnq     = 2793; // 中间 内圈 滑行前
    float mnh     = 3020; // 中间 内圈 滑行后
    float mwq     = 3270; // 中间 外圈 滑行前
    float mwh     = 3345; // 中间 外圈 滑行后
    float ywl     = 460;  // 外圈的y轴参数
    float ywr     = 453;  // 
    float ynl     = 124;  // 内圈的y轴参数
    float ynr     = 123;     // 内圈的y轴参数
    float wxq_12        = 2933;   // 12区外圈x前
    float wxh_12        = 3146; // 12区外圈x前
    float wxq_34        = 2250; // 34区外圈x前
    float wxh_34        = 2400; // 34区外圈x后
    float nxq_12        = 2793; // 12区内圈x前
    float nxh_12        = 3020; // 12区内圈x后
    float nxq_34        = 2430; // 34区内圈x前
    float nxh_34        = 2605; // 34区内圈x后
    float stake_x12 = 3700;   // 12区木桩x
    float stake_x34 = 177;    // 34区木桩x
    float stake_y   = 549;    // 木桩y
    float weight_m_x = 2556;//中间木桩x轴
    float wait_ticks    = 1.5;
    float wait_ticks_x  = 1.0;
    uint16_t cur1       = 0;
    uint16_t cur2       = 0;
    // weight_placement[0] = 1;
    // weight_placement[1] = 0;
    // weight_placement[2] = 0;
    // weight_placement[3] = 0;
    // weight_placement[4] = 0;
    for (;;) {

        if (stateflag == 0) // 夹取34区的砝码
        {
            if (weight_placement[3] == 1 && weight_placement[2] == 1) // 都在内圈
            {
                LightFury.gantry_t.position.x  = nxq_34;
                LightFury.gantry_t.position.yL = ynl;
                LightFury.gantry_t.position.yR = ynr;
                float diff[3]                  = {0};
                diff[0]                        = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                diff[1]                        = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                diff[2]                        = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                if (diff[0] < 20) {
                    if (tickflag_x == 0) {
                        starttick  = xTaskGetTickCount();
                        tickflag_x = 1;
                    }
                    cur2 = xTaskGetTickCount() - starttick;
                }

                if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                    osDelay(100);
                    cur2       = 0;
                    stateflag  = 1;
                    tickflag_x = 0;
                }

            } else if (weight_placement[3] == 1 && weight_placement[2] == 0) //
            {
                LightFury.gantry_t.position.x  = wxq_34;
                LightFury.gantry_t.position.yL = ywl;
                LightFury.gantry_t.position.yR = ynr;
                float diff[2]                  = {0};

                diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                diff[1] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                if (diff[0] < 20) {
                    if (tickflag_x == 0) {
                        starttick  = xTaskGetTickCount();
                        tickflag_x = 1;
                    }
                    cur2 = xTaskGetTickCount() - starttick;
                }
                if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                    osDelay(100);
                    stateflag  = 1;
                    cur2       = 0;
                    tickflag_x = 0;
                }
            } else if (weight_placement[3] == 0 && weight_placement[2] == 1) // 4区在外 3区在内------------->先抓内圈的3区 即left
            {
                LightFury.gantry_t.position.x  = wxq_34;
                LightFury.gantry_t.position.yL = ynl;
                LightFury.gantry_t.position.yR = ywr;
                float diff[2]                  = {0};
                diff[0]                        = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                diff[1]                        = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                if (diff[0] < 20) {
                    if (tickflag_x == 0) {
                        starttick  = xTaskGetTickCount();
                        tickflag_x = 1;
                    }
                    cur2 = xTaskGetTickCount() - starttick;
                }

                if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                    osDelay(100);
                    stateflag  = 1;
                    cur2       = 0;
                    tickflag_x = 0;
                }
            } else if (weight_placement[2] == 0 && weight_placement[3] == 0) // 都在外圈
            {
                LightFury.gantry_t.position.x  = wxq_34;
                LightFury.gantry_t.position.yL = ywl;
                LightFury.gantry_t.position.yR = ywr;
                float diff[3]                  = {0};
                diff[0]                        = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                if (diff[0] < 20) {
                    if (tickflag_x == 0) {
                        starttick  = xTaskGetTickCount();
                        tickflag_x = 1;
                    }
                    cur2 = xTaskGetTickCount() - starttick;
                }

                if ((diff[0] < 4) || cur2 > wait_ticks_x * 1000) {
                    osDelay(100);
                    stateflag  = 1;
                    cur2       = 0;
                    tickflag_x = 0;
                }
            }
        }
        else if (stateflag == 1) // 往前滑行一段吸上砝码
        {
            /*修改往前拖行吸上砝码时的pid*/
            if (once == 0) {
                pid_reset(&(LightFury.Motor_X->posPID), -40, 0, 0);
                pid_reset(&(LightFury.Motor_X->speedPID), 3, 0.3, 0.3);
                HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 0);
                HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 0);
                HAL_GPIO_WritePin(ElectromagnetYL_GPIO_Port, ElectromagnetYL_Pin, 1);
                HAL_GPIO_WritePin(ElectromagnetYR_GPIO_Port, ElectromagnetYR_Pin, 1);
                osDelay(500); // 增加delay 等待气缸落下 待定
                starttick = xTaskGetTickCount();
                once = 1;
            }
            if (weight_placement[2] == 1 && weight_placement[3] == 1) //
            {
                LightFury.gantry_t.position.x = nxh_34;
                float diff[2]                 = {0};
                diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                cur1                          = xTaskGetTickCount() - starttick;
                if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                    osDelay(100);
                    stateflag = 2;
                }
            }
            if (weight_placement[2] == 1 && weight_placement[3] == 0) // 4区在内 3区在外------------->先抓内圈的4区 即right
            {
                LightFury.gantry_t.position.x = nxh_34;
                float diff[2]                 = {0};
                diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                osDelay(280);
                HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                cur1 = xTaskGetTickCount() - starttick;

                if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                    osDelay(100);
                    stateflag = 2;
                }
            }
            if (weight_placement[2] == 0 && weight_placement[3] == 1) // 4区在外 3区在内------------->先抓内圈的2区 即left
            {
                LightFury.gantry_t.position.x = nxh_34;
                float diff[2]                 = {0};
                diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);

                osDelay(280);
                HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                cur1 = xTaskGetTickCount() - starttick;

                if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                    osDelay(100);
                    stateflag = 2;
                }
            }
            if (weight_placement[2] == 0 && weight_placement[3] == 0) // 都在外圈
            {
                LightFury.gantry_t.position.x = wxh_34;
                float diff[2]                 = {0};
                diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                cur1                          = xTaskGetTickCount() - starttick;

                if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                    osDelay(100);
                    stateflag = 2;
                }
            }
        }
        else if (stateflag == 2)
        {
            once = 0;
            HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
            HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
            pid_reset(&(LightFury.Motor_X->posPID), -27, 0, 0);

            osDelay(300);
            stateflag = 3;
        }
        

        else if (stateflag == 3)
        {
            
            // 先让左右的爪子退到两边
            LightFury.gantry_t.position.yL = 590;
            LightFury.gantry_t.position.yR = 590;
            osDelay(100);
            stateflag   = 4;
        
        }
        else if (stateflag == 4) {
            // 前往短区木桩            加上微调检测，添加两个GPIO_readpin
            
            LightFury.gantry_t.position.x  = stake_x12+10;      //往前再走一截 便于微调
            LightFury.gantry_t.position.yL = stake_y;
            LightFury.gantry_t.position.yR = stake_y-5;
            float diff[3]                  = {0};
            diff[0]                        = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
            diff[1]                        = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
            diff[2]                        = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
            if(diff[0]<500&&once==0){
                detectflag = 1;
                once       = 1;
            }
            
            if(detectflag==0&&once==1) 
            {
                osDelay(800);
                stateflag = 5;
            }
            if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2)) {
                osDelay(500);
                stateflag = 5;
            }
        
        } 
         else if (stateflag == 5)
            {
            once = 0;
            
            if (detectflag == 0) {
                osDelay(2);
                rem        = Lidar1.distance_aver;
                pid_reset(&(LightFury.Motor_X->posPID), -15, 0, 0);
                LightFury.gantry_t.position.x = rem + OFFSET12;
            } else {
                LightFury.gantry_t.position.x = stake_x12;
                pid_reset(&(LightFury.Motor_X->posPID), -30, 0, 0);
            }
            osDelay(1000);
            HAL_GPIO_WritePin(ElectromagnetYL_GPIO_Port, ElectromagnetYL_Pin, 0);
            HAL_GPIO_WritePin(ElectromagnetYR_GPIO_Port, ElectromagnetYR_Pin, 0);
            osDelay(1200);
            LightFury.gantry_t.position.x = stake_x12 - 100;
            pid_reset(&(LightFury.Motor_X->posPID), -30, 0, 0);

            osDelay(300);
            stateflag = 6;
            }
            if (stateflag == 6) // 夹取12区以及中间的砝码
            {
                pid_reset(&(LightFury.Motor_X->posPID), -30, 0, 0);

                if (weight_placement[0] == 1 && weight_placement[1] == 1 && weight_placement[4] == 1) // 都在内圈
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ynl;
                    LightFury.gantry_t.position.yR = ynr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 1 && weight_placement[0] == 0 && weight_placement[4] == 1) //
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ynl;
                    LightFury.gantry_t.position.yR = ywr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 0 && weight_placement[0] == 1 && weight_placement[4] == 1) //
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ywl;
                    LightFury.gantry_t.position.yR = ynr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 0 && weight_placement[0] == 0 && weight_placement[4] == 1) //
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ywl;
                    LightFury.gantry_t.position.yR = ywr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 1 && weight_placement[0] == 1 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ynl;
                    LightFury.gantry_t.position.yR = ynr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 1 && weight_placement[0] == 0 && weight_placement[4] == 0) // 都在内圈
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ynl;
                    LightFury.gantry_t.position.yR = ywr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 0 && weight_placement[0] == 1 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x  = mnq;
                    LightFury.gantry_t.position.yL = ywl;
                    LightFury.gantry_t.position.yR = ynr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }
                if (weight_placement[1] == 0 && weight_placement[0] == 0 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x  = wxq_12;
                    LightFury.gantry_t.position.yL = ywl;
                    LightFury.gantry_t.position.yR = ywr;
                    float diff[3]                  = {0};

                    diff[0] = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    diff[1] = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                    diff[2] = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                    if (diff[0] < 20) {
                        if (tickflag_x == 0) {
                            starttick  = xTaskGetTickCount();
                            tickflag_x = 1;
                        }
                        cur2 = xTaskGetTickCount() - starttick;
                    }

                    if ((diff[0] < 4 && diff[1] < 2 && diff[2] < 2) || cur2 > wait_ticks_x * 1000) {
                        osDelay(100);
                        tickflag_x = 0;
                        cur2       = 0;
                        stateflag  = 7;
                    }
                }

            } else if (stateflag == 7) // 往前滑行一段吸上砝码
            {
                if (once == 0) {
                    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 0);
                    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 0);
                    HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 0);
                    HAL_GPIO_WritePin(ElectromagnetYL_GPIO_Port, ElectromagnetYL_Pin, 1);
                    HAL_GPIO_WritePin(ElectromagnetYR_GPIO_Port, ElectromagnetYR_Pin, 1);
                    HAL_GPIO_WritePin(ElectromagnetX_GPIO_Port, ElectromagnetX_Pin, 1);

                    osDelay(800);
                    starttick = xTaskGetTickCount();
                    once      = 1; // 增加delay 等待气缸落下 待定
                }
                pid_reset(&(LightFury.Motor_X->posPID), -35, 0, 0);                                   /*修改往前拖行吸上砝码时的pid*/
                if (weight_placement[1] == 1 && weight_placement[0] == 1 && weight_placement[4] == 1) //
                {

                    LightFury.gantry_t.position.x = mnh;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    cur1                          = xTaskGetTickCount() - starttick;
                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 1 && weight_placement[1] == 0 && weight_placement[4] == 1) //
                {
                    LightFury.gantry_t.position.x = wxh_12;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(400);
                    HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 1);
                    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 0 && weight_placement[1] == 1 && weight_placement[4] == 1) //
                {
                    LightFury.gantry_t.position.x = wxh_12;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(400);
                    HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 1);
                    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 0 && weight_placement[1] == 0 && weight_placement[4] == 1) //
                {
                    LightFury.gantry_t.position.x = wxh_12;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(400);
                    HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 1);
                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 1 && weight_placement[1] == 1 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x = mwh;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(320);
                    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 1 && weight_placement[1] == 0 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x = mwh;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(320);
                    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                    osDelay(260);
                    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 0 && weight_placement[1] == 1 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x = mwh;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(320);
                    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                    osDelay(260);
                    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);

                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }
                if (weight_placement[0] == 0 && weight_placement[1] == 0 && weight_placement[4] == 0) //
                {
                    LightFury.gantry_t.position.x = mwh;
                    float diff[2]                 = {0};
                    diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                    osDelay(500);
                    HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                    HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                    cur1 = xTaskGetTickCount() - starttick;

                    if ((diff[0] < 2)||cur1>wait_ticks*1000) {
                        osDelay(100);
                        stateflag = 8;
                    }
                }

            } else if (stateflag == 8) {
                once = 0;
                HAL_GPIO_WritePin(CylinderYL_GPIO_Port, CylinderYL_Pin, 1);
                HAL_GPIO_WritePin(CylinderYR_GPIO_Port, CylinderYR_Pin, 1);
                HAL_GPIO_WritePin(CylinderX_GPIO_Port, CylinderX_Pin, 1);
                pid_reset(&(LightFury.Motor_X->posPID), -30, 0, 0);

                osDelay(200);
                stateflag = 9;
            } else if (stateflag == 9) {
                LightFury.gantry_t.position.x = weight_m_x;
                float diff[1]                 = {0};
                diff[0]                       = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);

                if ((diff[0] < 2)) {
                    osDelay(200);
                    pid_reset(&(LightFury.Motor_X->posPID), -25, 0, 0);
                    stateflag = 10;
                }
            } else if (stateflag == 10) { // 前往长区木桩
                HAL_GPIO_WritePin(ElectromagnetX_GPIO_Port, ElectromagnetX_Pin, 0);
                osDelay(300);
                
                LightFury.gantry_t.position.x  = stake_x34-20; 
                LightFury.gantry_t.position.yL = stake_y + 3;
                LightFury.gantry_t.position.yR = stake_y - 2;
                
                float diff[3]                  = {0};
                diff[0]                        = fabs(LightFury.gantry_t.position.x - Lidar1.distance_aver);
                diff[1]                        = fabs(LightFury.gantry_t.position.yL - Lidar2.distance_aver);
                diff[2]                        = fabs(LightFury.gantry_t.position.yR - Lidar3.distance_aver);
                if (diff[0] < 600 && once == 0) 
                {
                    detectflag = 1;
                    once       = 1;
                }
                if (detectflag == 0 && once == 1) {
                    osDelay(800);
                    stateflag = 11;
                }
                if ((diff[0] < 2 && diff[1] < 2 && diff[2] < 2)) {
                    osDelay(800);
                    stateflag = 11;
               
            }
            
            } else if (stateflag == 11) {
                if(detectflag==0){
                    osDelay(2);
                    rem                           = Lidar1.distance_aver;
                    pid_reset(&(LightFury.Motor_X->posPID), -30, 0, 0);

                    LightFury.gantry_t.position.x = rem+OFFSET34;
                }else{
                    pid_reset(&(LightFury.Motor_X->posPID), -30, 0, 0);
                    LightFury.gantry_t.position.x = stake_x34;
                }
                osDelay(1000);
                HAL_GPIO_WritePin(ElectromagnetYL_GPIO_Port, ElectromagnetYL_Pin, 0);
                HAL_GPIO_WritePin(ElectromagnetYR_GPIO_Port, ElectromagnetYR_Pin, 0);
                osDelay(100);
                stateflag = 12;
            } else if (stateflag == 12) {
                osDelay(1200);
                pid_reset(&(LightFury.Motor_X->posPID), -15, 0, 0);
                LightFury.gantry_t.position.x = 150;
            }
        Lidar6.distance_aver = stateflag;
        
        osDelay(2);
        }
}

void StateMachine_Start(void)
{
    osThreadId_t StateMachineHandle;
    const osThreadAttr_t StateMachine_attributes = {
        .name       = "StateMachine",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    StateMachineHandle = osThreadNew(StateMachine_Task, NULL, &StateMachine_attributes);
}

