#include "datou.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <usart.h>
StepperMotorControl moto1;//一号轮
StepperMotorControl moto2;//二号轮
StepperMotorControl moto3;//三号轮
StepperMotorControl moto4;//四号轮
/******************************************************
Function:    		void StepperMotorControl_init(StepperMotorControl *control, uint8_t address) 
Description: 		指定电机初始化函数（这个函数是用来指定电机的速度的）
Calls:				 	NONE 
Input: 					StepperMotorControl *control这个变量是结构体变量，这个结构体里面有2个成员
								分别对应了当前发送的数据，上次发送的数据
								address	是电机设定的地址
******************************************************/

void StepperMotorControl_init(StepperMotorControl *control, uint8_t address) 
{
    uint8_t defaultBytes[8] = {0x00, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B};
    memcpy(control->controlBytes, defaultBytes, 8);
    memcpy(control->lastControlBytes, defaultBytes, 8);
    control->controlBytes[0] = address;
}

static uint8_t hasChanged(StepperMotorControl *control) 
{
    for (uint8_t i = 0; i < 8; i++) {
        if (control->controlBytes[i] != control->lastControlBytes[i]) 
				{
            return 1;
        }
    }
    return 0;
}

static void updateLastControlBytes(StepperMotorControl *control) 
{
    for (uint8_t i = 0; i < 8; i++) {
        control->lastControlBytes[i] = control->controlBytes[i];
    }
}

static void sendCommand(StepperMotorControl *control) 
{
    //if (hasChanged(control)) {
    if (1) {

		HAL_UART_Transmit(&motor_huart, control->controlBytes, 8, 100);
        updateLastControlBytes(control);
    }
}
/******************************************************
Function:    		void set_speed(StepperMotorControl *control, uint8_t direction, uint16_t speed) 
Description: 		设置电机的转速
Calls:				 	static void sendCommand(StepperMotorControl *control) 
Input: 					StepperMotorControl *control这个变量是结构体变量，这个结构体里面有2个成员
								分别对应了当前发送的数据，上次发送的数据
								direction 是电机的方向
								speed	是电机的速度
                                acceleration 是电机的加速度
                                snF 是多机同步标志，false为不启用，true为启
******************************************************/
void set_speed(StepperMotorControl *control, uint8_t direction, uint16_t speed, uint16_t acceleration,uint8_t snF) 
{
    if (direction) {
        control->controlBytes[2] = (0x0) ; // 逆时针方向
    } else {
        control->controlBytes[2] = (0x01) ; // 顺时针方向
    }
    control->controlBytes[3] = speed >>8;
    control->controlBytes[4] = speed & 0xFF;
    control->controlBytes[5] = acceleration;
    control->controlBytes[6] = snF;

    sendCommand(control);
}


StepperMotorControl_location moto_9;//下
StepperMotorControl_location moto_8;//上


/******************************************************
Function:    		void StepperMotorControl_init_location(StepperMotorControl_location *control, uint8_t address) 
Description: 		指定电机初始化函数（这个函数是用来指定电机的位置的）
Calls:				 	NONE 
Input: 					StepperMotorControl_location *control这个变量是结构体变量，这个结构体里面有3个成员
								分别对应了当前发送的数据，上次发送的数据，以及当前的位置值
								address	是电机设定的地址
******************************************************/

void StepperMotorControl_init_location(StepperMotorControl_location *control, uint8_t address) 
{
	
	
    uint8_t defaultBytes[13] = {0x00, 0xFD, 0x00, 0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x6B};
    memcpy(control->controlBytes, defaultBytes, 13);
    memcpy(control->lastControlBytes, defaultBytes, 13);
    control->controlBytes[0] = address;
}
static uint8_t hasChanged_location(StepperMotorControl_location *control) 
{
    for (uint8_t i = 0; i < 13; i++) {
        if (control->controlBytes[i] != control->lastControlBytes[i]) 
				{
            return 1;
        }
    }
    return 0;
}
static void updateLastControlBytes_location(StepperMotorControl_location *control) 
{
    for (uint8_t i = 0; i < 13; i++) {
        control->lastControlBytes[i] = control->controlBytes[i];
    }
}

static void sendCommand_location(StepperMotorControl_location *control) 
{
    if (hasChanged_location(control)) {

				HAL_UART_Transmit(&motor_huart, control->controlBytes, 13, 100);
        updateLastControlBytes_location(control);
    }
}

void set_location(StepperMotorControl_location *control, uint8_t direction, uint16_t speed,uint8_t acceleration,int32_t pulse_num, uint8_t re_or_ab,uint8_t snF) 
{		
	if (direction) {
        control->controlBytes[2] = (0x0) ; // 逆时针方向
    } else {
        control->controlBytes[2] = (0x01) ; // 顺时针方向
    }
    control->controlBytes[3] = speed >>8;
    control->controlBytes[4] = speed & 0xFF;   
    control->controlBytes[5] = acceleration;
	control->controlBytes[6] = (pulse_num&0xFF000000)>>24;/* 脉冲数中字节 */
	control->controlBytes[7] = (pulse_num&0xFF0000)>>16;/* 脉冲数中字节 */
	control->controlBytes[8] = (pulse_num&0xFF00)>>8;/* 脉冲数中字节 */
	control->controlBytes[9] = pulse_num&0xFF;	/* 脉冲数低字节 */
	control->controlBytes[10] = re_or_ab;/* 相位/绝对标志，false为相对运动，true为绝对值运动 */
    control->controlBytes[11] = snF;

    sendCommand_location(control);
}


/******************************************************
Function:    	void set_angle_control_location(StepperMotorControl_location *control, float target_angle_num)
Description: 	控制指定电机的角度
Calls:				 static void set_location(StepperMotorControl_location *control, Command com, int pulse_num) 
Input: 					StepperMotorControl_location *control这个变量是结构体变量，这个结构体里面有3个成员
								分别对应了当前发送的数据，上次发送的数据，以及当前的位置值（由于需要通过用角度的差值来计算脉冲数，所以需要记录当前的位置）
								target_angle_num	是目标角度
******************************************************/
// #define a_circle_pulse 3200.0//这是一圈需要的脉冲数
// void set_angle_control_location(StepperMotorControl_location *control, float target_angle_num)
// {
// 	double error_angle=target_angle_num-(control->now_angle);
// 	int need_pulse=(int)(error_angle/360.0*a_circle_pulse);
// 	if(error_angle!=0)
// 	{
// 		if(need_pulse<0)
// 		{
// 			set_location(control,reverse, -need_pulse);//注意此处的reverse可能需要改成forward，这个要根据你的物理结构而定
// 		}
// 		else
// 		{
// 			set_location(control,forward, need_pulse);//注意此处的forward可能需要改成reverse，这个要根据你的物理结构而定
// 		}
// 		control->now_angle=target_angle_num;
		
// 	}
// }
