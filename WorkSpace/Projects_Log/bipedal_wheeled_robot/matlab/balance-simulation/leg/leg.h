/**
 ******************************************************************************
 * @file    leg.h
 * @author  Wang Hongxi
 * @version V1.0.0
 * @date    2022/1/11
 * @brief
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#ifndef __LEG_H
#define __LEG_H
#include "motorA1.h"
#include "motor.h"
#include "motorHT.h"
#include "config.h"
#include "arm_math.h"

#ifndef user_sinf
#define user_sinf arm_sin_f32
#endif
#ifndef user_cosf
#define user_cosf arm_cos_f32
#endif

typedef struct
{
    A1_t HipMotor[2];
    Motor_t ChassisMotor;

    float alpha, dalpha, ddalpha, ddalpha_lpfCoef;

    // five-links parameters
    float l[6], dL0, ddL0, ddL0_lpfCoef; // l[0]/dL0/ddL0 represents length/dlength/ddlength of whole leg
    float m[5];                          // m[0] represents nothing
    float phi[5], dphi[5];               // phi[0] represents atan(yC/(xC-l/2)) = alpha+pi/2
    float x[5], dx[5];
    float y[5], dy[5];

    float Mass;
    float Center_Of_Mass[2];
} Leg_t;

void Leg_Init(Leg_t *leg);
void Leg_Kinematic_Solving(Leg_t *leg, float phi1, float dphi1, float phi4, float dphi4, float dt);

#endif
