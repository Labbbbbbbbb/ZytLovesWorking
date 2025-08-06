//
// Created by liam on 2023/7/19.
//

#ifndef STEPPING_MOTOR_DECODE_H
#define STEPPING_MOTOR_DECODE_H

#include "stm32f1xx.h"
#include<stdio.h>
typedef union {
    int data;
    uint8_t byte[4];
}byte2int;

void decode(uint8_t *p);
extern byte2int data_decoded[4];
extern int world_point[4];

#endif //STEPPING_MOTOR_DECODE_H
