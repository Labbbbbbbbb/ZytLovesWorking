#ifndef CONTROL_H
#define CONTROL_H

#include "stm32f4xx.h"
#include "fuzzy_pid.h"
#include "jy901s.h"

#define LEFT 0
#define RIGHT 1
typedef struct{
    float alphaLeft, betaLeft;
    float alphaRight, betaRight;
    float XLeft,YLeft;
    float XRight, YRight;
}IKparam;

extern PID_t left_pid,right_pid;
extern PID_t angle_pid;
extern PID_t gyro_pid;
extern __IO int16_t vel_left;
extern __IO int16_t vel_right;
extern uint32_t tim_mark[2];

void Control_Peripheral_init(void);
void Control_param_init(void);
void Angle_Control_Loop(float ref);
void Wheel_Control_Loop(void);
void Servo_IK_Control(uint8_t index,float height);
void IK_Param_Init(void);
void Roll_Control_Loop(void);
void Velocity_Control_Loop(float forward,float turn);

#endif
