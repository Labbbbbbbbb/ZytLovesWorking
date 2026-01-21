#ifndef FUZZYPID_H
#define FUZZYPID_H
#include "math.h"
#include <stdio.h>
#include "wtr_calculate.h"

void fuzzy_pid_update(PID_t *pid, double error, double dt);
void fuzzy_inference(double e, double ec, double *delta_Kp, double *delta_Ki, double *delta_Kd);

#endif/* Define to prevent recursive inclusion -------------------------------------*/