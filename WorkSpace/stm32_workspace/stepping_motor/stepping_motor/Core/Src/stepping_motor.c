//
// Created by Quan2 on 2023/7/14.
//
#include "steep_motor.h"

Motor_ motor_instance[2];   //0号电机控制x轴方向
volatile uint16_t pos_ = 0;
volatile uint8_t motor_temp= 0;

void motor_init(uint8_t i,TIM_HandleTypeDef TIM_PWMHandle,uint16_t TIM_CHANNELHandle)
{
    motor_instance[i].MOTOR_DIR = 0;
    motor_instance[i].motor_enable = 0;
    motor_instance[i].TIM_PWMHandle = TIM_PWMHandle;
    motor_instance[i].TIM_CHANNELHandle = TIM_CHANNELHandle;
    PID_Param_init(&motor_instance[i].pid_);
}

void gpio_bind(uint8_t i,GPIO_TypeDef *GPIO_ENA_,uint16_t GPIO_Pin_ENA_,
               GPIO_TypeDef *GPIO_DIR_,uint16_t GPIO_Pin_DIR_)
{
    motor_instance[i].gpio_param.GPIO_DIR = GPIO_DIR_;
    motor_instance[i].gpio_param.GPIO_Pin_DIR = GPIO_Pin_DIR_;
    motor_instance[i].gpio_param.GPIO_ENA = GPIO_ENA_ ;
    motor_instance[i].gpio_param.GPIO_Pin_ENA = GPIO_Pin_ENA_;
}

void motor_speed_control(uint8_t i, uint16_t speed)
{
    if(speed >=0)
    {
        if(motor_instance[i].MOTOR_DIR ==1)
        {
            set_motordir(i);
            motor_instance[i].MOTOR_DIR = 0;
        }
        HAL_TIM_PWM_Stop(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1);
        __HAL_TIM_SET_AUTORELOAD(&motor_instance[i].TIM_PWMHandle,speed);
        __HAL_TIM_SET_COMPARE(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1,speed/2);
        HAL_TIM_PWM_Start(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1);
    }
    else
    {
        if(motor_instance[i].MOTOR_DIR ==0)
        {
            set_motordir(i);
            motor_instance[i].MOTOR_DIR = 1;
        }
        HAL_TIM_PWM_Stop(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1);
        __HAL_TIM_SET_AUTORELOAD(&motor_instance[i].TIM_PWMHandle,(-speed));
        __HAL_TIM_SET_COMPARE(&motor_instance[i].TIM_PWMHandle, TIM_CHANNEL_1, (-speed/2));
        HAL_TIM_PWM_Start(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1);
    }
}

void motor_pos_control(uint8_t i, uint16_t pos, uint16_t speed)
{
    __HAL_TIM_SET_AUTORELOAD(&motor_instance[i].TIM_PWMHandle,speed);
    __HAL_TIM_SET_COMPARE(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1,(uint16_t)(speed/2));
    //HAL_TIM_PWM_Start(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&motor_instance[i].TIM_PWMHandle,TIM_CHANNEL_1);
    pos_ = pos;
    motor_temp = i;
}


void motor_enable(uint8_t  i)
{
    motor_instance[i].motor_enable = 1;
    HAL_GPIO_WritePin(motor_instance[i].gpio_param.GPIO_ENA,motor_instance[i].gpio_param.GPIO_Pin_ENA,GPIO_PIN_SET);
}

void motor_disable(uint8_t i)
{
    motor_instance[i].motor_enable = 0;
    HAL_GPIO_WritePin(motor_instance[i].gpio_param.GPIO_ENA,motor_instance[i].gpio_param.GPIO_Pin_ENA,GPIO_PIN_RESET);
}

void set_motordir(uint8_t i)
{
    if(motor_instance[i].MOTOR_DIR ==0)
    {
        HAL_GPIO_WritePin(motor_instance[i].gpio_param.GPIO_DIR,motor_instance[i].gpio_param.GPIO_Pin_DIR,GPIO_PIN_RESET);
    }
    else
        HAL_GPIO_WritePin(motor_instance[i].gpio_param.GPIO_DIR,motor_instance[i].gpio_param.GPIO_Pin_DIR,GPIO_PIN_SET);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) //中断回调
{
    if(htim == &htim4)
    {
        static int16_t i = 0;
        i++;
        if(i >= pos_)  //此处10为产生PWM的数量，可设置为变量实时改变
        {
            i =0;
            HAL_TIM_PWM_Stop(&motor_instance[motor_temp].TIM_PWMHandle, motor_instance[motor_temp].TIM_CHANNELHandle); //产生指定数量的PWM波后停止PWM的产生
        }
    }
}

