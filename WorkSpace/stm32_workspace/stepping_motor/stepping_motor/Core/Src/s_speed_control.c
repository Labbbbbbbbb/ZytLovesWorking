//
// Created by Quan2 on 2023/7/14.
//
#include "s_speed_control.h"

/**
* 基础部分功能1
 * 保持小球在区域2内停留不少于5s
*/

void keep_stable(uint16_t x,uint16_t y)
{
    set_pid_target(&motor_instance[0].pid_,x);
    set_pid_target(&motor_instance[0].pid_,y);
}

void motor_control(uint8_t i)
{
    float current_val = 0;
    if(motor_instance[i].motor_enable == 1)
    {
        current_val = PID_realize(&motor_instance[i].pid_,world_point[i]);
        if(current_val > 0)
        {
            motor_instance[i].MOTOR_DIR = 0;
            set_motordir(i);
        }
        else{
            motor_instance[i].motor_enable = 1;
            set_motordir(i);
        }
        motor_pos_control(i,(uint16_t)(current_val),5000);
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim3)
    {
        motor_control(0);
        motor_control(1);
    }
}