#ifndef __DATOU_H
#define __DATOU_H

#include "main.h"
#include <stdbool.h>
#define motor_huart huart1

typedef struct 
{
    uint8_t controlBytes[8];
    uint8_t lastControlBytes[8];

} StepperMotorControl;
typedef struct 
{
    uint8_t controlBytes[13];
    uint8_t lastControlBytes[13];
	float now_angle;
} StepperMotorControl_location;
typedef  enum 
{
	forward=0x12,		//正
	reverse=0x02		//反
}Command;
extern StepperMotorControl moto1;
extern StepperMotorControl moto2;
extern StepperMotorControl moto3;
extern StepperMotorControl moto4;
void StepperMotorControl_init(StepperMotorControl *control, uint8_t address);
void set_speed(StepperMotorControl *control, uint8_t direction, uint16_t speed, uint16_t acceleration,uint8_t snF) ;
void set_location(StepperMotorControl_location *control, uint8_t direction, uint16_t vel,uint8_t acc,int32_t pulse_num, uint8_t re_or_ab,uint8_t snF) ;
extern StepperMotorControl_location moto_9;//下
extern StepperMotorControl_location moto_8;//上
void StepperMotorControl_init_location(StepperMotorControl_location *control, uint8_t address) ;
//void set_angle_control_location(StepperMotorControl_location *control, float target_angle_num);
#endif