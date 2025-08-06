#ifndef CALCULATE_H
#define CALCULATE_H

#include "HWT101CT_sdk.h"
#include <math.h>
#include "TLE5012B.h"

#define R1 20  //全向轮半径，单位mm
#define R2 20
#define PI 3.14159
extern float Runge_Kutta_yaw;
extern float Runge_Kutta_pitch;
extern float Runge_Kutta_roll;
extern  float W_matrix[16];
extern float position[2];
extern float tag;
 
/**
float* Matrix44_41Multiplication(float* matrix_a,float* matrix_b,float* result);
float* Matrix22_21Multiplication(float* matrix_a,float* matrix_b,float* result);
float* Matrix4_1Add(float* matrix_a,float* matrix_b,float* result)
void W_matrix_update();
**/

void TimingDataProcess(int32_t Ts);
void Timing_Distance_Calculate(void);

#endif // !1