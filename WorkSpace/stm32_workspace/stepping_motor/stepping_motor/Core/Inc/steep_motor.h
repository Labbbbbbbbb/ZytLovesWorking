//
// Created by Quan2 on 2023/7/14.
//

#ifndef STEPPING_MOTOR_STEEP_MOTOR_H
#define STEPPING_MOTOR_STEEP_MOTOR_H

#include "stm32f1xx.h"
#include "tim.h"
#include "bsp_pid.h"

#define ENA_PIN          GPIO_PIN_10
#define PUL_PIN          GPIO_PIN_8
#define DIR_PIN          GPIO_PIN_9

#define ENA_GPIO          GPIOA
#define PUL_GPIO          GPIOA
#define DIR_GPIO          GPIOA

typedef struct {
    GPIO_TypeDef *GPIO_ENA;
    uint16_t GPIO_Pin_ENA;
    GPIO_TypeDef *GPIO_DIR;
    uint16_t GPIO_Pin_DIR;
}GPIO_;

typedef struct {
    uint8_t MOTOR_DIR;                       //0为正转  1为反转
    uint8_t motor_enable;                    //1为开启
    TIM_HandleTypeDef TIM_PWMHandle;
    uint16_t TIM_CHANNELHandle;
    GPIO_ gpio_param;
    _PID_t pid_;
}Motor_;

extern Motor_ motor_instance[2];

void motor_init(uint8_t i,TIM_HandleTypeDef TIM_PWMHandle,uint16_t TIM_CHANNELHandle);
void gpio_bind(uint8_t i,GPIO_TypeDef *GPIO_ENA_,uint16_t GPIO_Pin_ENA_,GPIO_TypeDef *GPIO_DIR_,uint16_t GPIO_Pin_DIR_);
void motor_speed_control(uint8_t i, uint16_t speed);
void motor_enable(uint8_t  i);
void motor_disable(uint8_t i);
void set_motordir(uint8_t i);
void motor_pos_control(uint8_t i, uint16_t pos, uint16_t speed);
#endif //STEPPING_MOTOR_STEEP_MOTOR_H
