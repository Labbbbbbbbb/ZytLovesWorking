#include "fuzzy_pid.h"
// ========================
// 1. 定义模糊集合符号
// ========================
#define NB -0.30  // Negative Big
#define NM -0.20  // Negative Medium
#define NS -0.10  // Negative Small
#define ZO  0.0  // Zero
#define PS  0.10  // Positive Small
#define PM  0.20  // Positive Medium
#define PB  0.30  // Positive Big
// 三角形隶属函数
double trimf(double x, double a, double b, double c) {
    if (x <= a || x >= c) return 0.0;
    if (x < b) return (x - a) / (b - a);
    return (c - x) / (c - b);
}

// ========================
// 2. 模糊推理函数
// ========================
void fuzzy_inference(double e, double ec, double *delta_Kp, double *delta_Ki, double *delta_Kd) {
    // 输入范围: e ∈ [-0.5, 0.5], ec ∈ [-2, 2]
    
    // 计算输入隶属度
    double e_mf[7] = {
        trimf(e, -50, -50, -35), // NB
        trimf(e, -35, -20, -15), // NM
        trimf(e, -15, -12, 9),   // NS
        trimf(e, -9, 0.0, 9),    // ZO
        trimf(e, 9, 12, 15),     // PS
        trimf(e, 12, 15, 20),    // PM
        trimf(e, 15, 20, 25)     // PB
    };
    
    double ec_mf[7] = {
        trimf(ec, -200, -170, -140), // NB
        trimf(ec, -140, -110, -80), // NM
        trimf(ec, -80, -50, -20), // NS
        trimf(ec, -20, 0, 20), // ZO
        trimf(ec, 20, 50, 80), // PS
        trimf(ec, 80, 110, 140), // PM
        trimf(ec, 140, 170, 200) // PB
    };

    // ========================
    // 3. 规则表（根据你提供的图片）
    // ========================

    // ΔKp 规则表
    double rule_Kp[7][7] = {
        {PS, NS, NB, NB, NB, NM, PS},  // NB
        {PS, NS, NB, NM, NM, NS, ZO},  // NM
        {ZO, NS, NM, NM, NS, NS, ZO},  // NS
        {ZO, NS, NS, ZO, NS, NS, ZO},  // ZO
        {ZO, ZO, ZO, ZO, ZO, ZO, ZO},  // PS
        {PB, NS, PS, PS, PS, PS, PB},  // PM
        {PB, PM, PM, PM, PS, PS, PB}   // PB
    };

    // ΔKi 规则表
    double rule_Ki[7][7] = {
        {PS, NS, NB, NB, NB, NM, PS},  // NB
        {PS, NS, NB, NM, NM, NS, ZO},  // NM
        {ZO, NS, NM, NM, NS, NS, ZO},  // NS
        {ZO, NS, NS, ZO, NS, NS, ZO},  // ZO
        {ZO, ZO, ZO, ZO, ZO, ZO, ZO},  // PS
        {PB, NS, PS, PS, PS, PS, PB},  // PM
        {PB, PM, PM, PM, PS, PS, PB}   // PB
    };

    // ΔKd 规则表
    double rule_Kd[7][7] = {
        {NB, NB, NM, NM, NS, ZO, ZO},  // NB
        {NB, NB, NM, NS, NS, ZO, ZO},  // NM
        {NB, NM, NS, NS, ZO, PS, PS},  // NS
        {NM, NS, NS, ZO, PS, PM, PM},  // ZO
        {NS, NS, ZO, PS, PS, PM, PB},  // PS
        {ZO, ZO, ZO, PS, PM, PB, PB},  // PM
        {ZO, ZO, PS, PM, PM, PB, PB}   // PB
    };

    // ========================
    // 4. 重心法去模糊化
    // ========================
    double sum_Kp = 0.0, sum_Ki = 0.0, sum_Kd = 0.0;
    double weight_sum = 0.0;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            double firing_strength = fmin(e_mf[i], ec_mf[j]); // AND 操作
            if (firing_strength > 0) {
                sum_Kp += firing_strength * rule_Kp[i][j];
                sum_Ki += firing_strength * rule_Ki[i][j];
                sum_Kd += firing_strength * rule_Kd[i][j];
                weight_sum += firing_strength;
            }
        }
    }

    if (weight_sum > 0) {
        *delta_Kp = sum_Kp / weight_sum;
        *delta_Ki = sum_Ki / weight_sum;
        *delta_Kd = sum_Kd / weight_sum;
    } else {
        *delta_Kp = 0.0;
        *delta_Ki = 0.0;
        *delta_Kd = 0.0;
    }

    // ========================
    // 5. 输出缩放（实际应用中需根据系统调整）
    // ========================
    *delta_Kp *= 0.5;  // Kp 调整范围 [-1.5, 1.5]
    *delta_Ki *= 0.2;  // Ki 调整范围 [-0.6, 0.6]
    *delta_Kd *= 0.3;  // Kd 调整范围 [-0.9, 0.9]
}

// ========================
// 6. PID 控制器结构
// ========================


void fuzzy_pid_update(PID_t *pid, double error, double dt) {
    pid->integral += error * dt;
    double derivative = (error - pid->error[1]) / dt;
    pid->output = pid->KP * error + pid->KI * pid->integral + pid->KD * derivative;
    pid->error[1] = error;
}
