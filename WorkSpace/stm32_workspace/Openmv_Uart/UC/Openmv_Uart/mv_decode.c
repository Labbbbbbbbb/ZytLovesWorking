#include "mv_decode.h"  //大端的解包方式，前面的是高位
uint16_t gimbal_x;     //云台openmv反馈回来的yaw轴位置
uint16_t gimbal_y;     //云台openmv反馈回来的y轴位置
uint16_t lock_x;   //下方摄像头锁定的x轴位置
uint16_t lock_y;   //下方摄像头锁定的y轴位置
uint8_t gimbal_color;//云台mv返回的颜色  1：红/蓝 2：黄/黑
uint8_t lock_color;  //下方摄像头返回的颜色 1：红/蓝 2：黑 3：黄
uint8_t error_flag;    //指示是否即将吸入不该吸的球


void  Gimbal_Decode()
{
    gimbal_color=receive_buffer1[2];
    gimbal_x = receive_buffer1[3]*256 + receive_buffer1[4];
    gimbal_y = receive_buffer1[3]*256 + receive_buffer1[4];
}
void ErrorFlag_Decode()
{
    error_flag = receive_buffer2[2];
}
void Lock_Decode()
{
    lock_color=receive_buffer3[2];
    lock_x=receive_buffer3[3]*256 + receive_buffer3[4];
    lock_y=receive_buffer3[5]*256 + receive_buffer3[6];
}