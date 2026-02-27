/**
 ******************************************************************************
 * @file    leg.c
 * @author  Wang Hongxi
 * @version V1.0.0
 * @date    2022/1/11
 * @brief
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#include "leg.h"

static void Five_Links_Forward_Kinematics(const float l[5], float phi[5], float x[5], float y[5]);
static void Five_Links_Backward_Kinematics(const float l[5], float x[5], float y[5], float *phi1, float *phi4);

void Leg_Init(Leg_t *leg)
{
    leg->ChassisMotor.ReductionRatio = 13;
    leg->l[1] = ACTIVE_ARM_LEN;
    leg->l[2] = SLAVE_ARM_LEN;
    leg->l[3] = SLAVE_ARM_LEN;
    leg->l[4] = ACTIVE_ARM_LEN;
    leg->l[5] = JOINT_DISTANCE;
    leg->m[1] = ACTIVE_ARM_MASS;
    leg->m[2] = SLAVE_ARM_MASS;
    leg->m[3] = SLAVE_ARM_MASS;
    leg->m[4] = ACTIVE_ARM_MASS;
    leg->Mass = leg->m[1] + leg->m[2] + leg->m[3] + leg->m[4];

    leg->x[A_] = 0;
    leg->y[A_] = 0;
    leg->x[E_] = leg->l[5];
    leg->y[E_] = 0;

    leg->ddL0_lpfCoef = 0.0085;
    leg->ddalpha_lpfCoef = 0.02;
}

void Leg_Kinematic_Solving(Leg_t *leg, float phi1, float dphi1, float phi4, float dphi4, float dt)
{
    leg->phi[4] = phi4;
    leg->phi[1] = phi1;
    leg->dphi[4] = dphi4;
    leg->dphi[1] = dphi1;

    Five_Links_Forward_Kinematics(leg->l, leg->phi, leg->x, leg->y);
    leg->alpha = leg->phi[0] - PI / 2;
    leg->l[0] = sqrtf((leg->x[C_] - leg->l[5] / 2) * (leg->x[C_] - leg->l[5] / 2) + leg->y[C_] * leg->y[C_]);

    leg->Center_Of_Mass[X] = (leg->x[A_] + leg->x[B_]) / 2 * leg->m[1] + (leg->x[E_] + leg->x[D_]) / 2 * leg->m[4] + (leg->x[C_] + leg->x[B_]) / 2 * leg->m[2] + (leg->x[C_] + leg->x[D_]) / 2 * leg->m[3];
    leg->Center_Of_Mass[X] /= leg->Mass;
    leg->Center_Of_Mass[Y] = (leg->y[A_] + leg->y[B_]) / 2 * leg->m[1] + (leg->y[E_] + leg->y[D_]) / 2 * leg->m[4] + (leg->y[C_] + leg->y[B_]) / 2 * leg->m[2] + (leg->y[C_] + leg->y[D_]) / 2 * leg->m[3];
    leg->Center_Of_Mass[Y] /= leg->Mass;

    float phi[5], x[5], y[5];
    const float deltat = 0.0001f;
    phi[4] = leg->phi[4] + leg->dphi[4] * deltat;
    phi[1] = leg->phi[1] + leg->dphi[1] * deltat;
    Five_Links_Forward_Kinematics(leg->l, phi, x, y);

    float dL0 = (sqrtf((x[C_] - leg->l[5] / 2) * (x[C_] - leg->l[5] / 2) + y[C_] * y[C_]) - leg->l[0]) / deltat;
    leg->ddL0 = (dL0 - leg->dL0) / (leg->ddL0_lpfCoef + dt) +
                leg->ddL0 * leg->ddL0_lpfCoef / (leg->ddL0_lpfCoef + dt);
    leg->dL0 = dL0;
    float dalpha = ((phi[0] - PI / 2) - leg->alpha) / deltat;
    leg->ddalpha = (dalpha - leg->dalpha) / (leg->ddalpha_lpfCoef + dt) +
                   leg->ddalpha * leg->ddalpha_lpfCoef / (leg->ddalpha_lpfCoef + dt);
    leg->dalpha = dalpha;
    leg->dphi[0] = (phi[0] - leg->phi[0]) / deltat;
    leg->dphi[2] = (phi[2] - leg->phi[2]) / deltat;
    leg->dphi[3] = (phi[3] - leg->phi[3]) / deltat;
    for (uint8_t i = 0; i < 5; i++)
    {
        leg->dx[i] = (x[i] - leg->x[i]) / deltat;
        leg->dy[i] = (y[i] - leg->y[i]) / deltat;
    }
}

/**
 * @brief Five-Links Forward Kinematics
 * @param[in]       l Links length in m
 * @param[in out]   phi Links angle in rad (input: phi1 phi4)
 * @param[out]      x joints position in m
 * @param[out]      y joints position in m
 */
static void Five_Links_Forward_Kinematics(const float l[5], float phi[5], float x[5], float y[5])
{
    x[D_] = l[5] + l[4] * user_cosf(phi[4]);
    y[D_] = 0 + l[4] * user_sinf(phi[4]);
    x[B_] = 0 + l[1] * user_cosf(phi[1]);
    y[B_] = 0 + l[1] * user_sinf(phi[1]);
    float BD = sqrtf((x[D_] - x[B_]) * (x[D_] - x[B_]) + (y[D_] - y[B_]) * (y[D_] - y[B_]));
    float A0 = 2 * l[2] * (x[D_] - x[B_]);
    float B0 = 2 * l[2] * (y[D_] - y[B_]);
    float C0 = l[2] * l[2] + BD * BD - l[3] * l[3];
    phi[2] = 2 * atan2f(B0 + sqrtf(A0 * A0 + B0 * B0 - C0 * C0), A0 + C0);
    x[C_] = x[B_] + l[2] * user_cosf(phi[2]);
    y[C_] = y[B_] + l[2] * user_sinf(phi[2]);
    phi[0] = atan2f(y[C_], x[C_] - l[5] / 2);
    phi[3] = atan2f(y[C_] - y[D_], x[C_] - x[D_]);
}

/**
 * @brief Five-Links Backward Kinematics
 * @param[in]       l Links length in m
 * @param[in out]   x joints position in m (input: xC)
 * @param[in out]   y joints position in m (input: yC)
 * @param[out]      phi Links angle in rad
 */
static void Five_Links_Backward_Kinematics(const float l[5], float x[5], float y[5], float *phi1, float *phi4)
{
    float AC = sqrtf((x[C_] - x[A_]) * (x[C_] - x[A_]) + (y[C_] - y[A_]) * (y[C_] - y[A_]));
    float CE = sqrtf((x[C_] - x[E_]) * (x[C_] - x[E_]) + (y[C_] - y[E_]) * (y[C_] - y[E_]));
    float theta_1 = acosf((AC * AC + l[5] * l[5] - CE * CE) / (2 * AC * l[5]));
    float theta_2 = acosf((CE * CE + l[5] * l[5] - AC * AC) / (2 * CE * l[5]));
    float theta_3 = acosf((l[1] * l[1] + AC * AC - l[2] * l[2]) / (2 * AC * l[1]));
    float theta_4 = acosf((CE * CE + l[4] * l[4] - l[3] * l[3]) / (2 * CE * l[4]));
    *phi1 = theta_1 + theta_3;
    *phi4 = PI - theta_2 - theta_4;
}
