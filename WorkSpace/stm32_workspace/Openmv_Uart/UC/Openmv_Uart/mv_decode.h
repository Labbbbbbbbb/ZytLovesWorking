#ifndef MV_DECODE___
#define MV_DECODE___
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
extern uint8_t receive_buffer1[9];
extern uint8_t receive_buffer2[5];
extern uint8_t receive_buffer3[9];
extern uint16_t gimbal_x;     //云台openmv反馈回来的yaw轴位置
extern uint16_t gimbal_y;
extern uint16_t lock_x;   //下方摄像头锁定的x轴位置
extern uint16_t lock_y;   //下方摄像头锁定的y轴位置
extern uint8_t gimbal_color;//云台mv返回的颜色  1：红/蓝 2：黄/黑
extern uint8_t lock_color;  //下方摄像头返回的颜色 1：红/蓝 2：黑 3：黄
extern uint8_t error_flag;    //指示是否即将吸入不该吸的球
void  Gimbal_Decode();
void ErrorFlag_Decode();
void Lock_Decode();

#endif