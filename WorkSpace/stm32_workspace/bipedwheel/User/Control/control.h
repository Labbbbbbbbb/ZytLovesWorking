#ifndef CONTROL_H
#define CONTROL_H

#include "stm32f4xx.h"
#include "wtr_calculate.h"
#include "jy901s.h"

typedef struct{
    float alphaLeft, betaLeft;
    float alphaRight, betaRight;
    float XLeft,YLeft;
    float XRight, YRight;
}IKparam;

extern PID_t left_pid,right_pid;
extern PID_t angle_pid;
extern PID_t gyro_pid;

void Control_Peripheral_init(void);
void Control_param_init(void);
void Angle_Control_Loop(void);
void Wheel_Control_Loop(void);
void Servo_IK_Control(float height);
void IK_Param_Init(void);


#endif
