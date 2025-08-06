#include  "step.h"
#include "tim.h"
#include "math.h"
void gpio_step(uint8_t dir,uint16_t us,uint16_t angle)   //  angle:64细分
{                                                       //us:延迟微秒数234-390
    HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,dir);
    for(uint16_t i=0;i<=round(angle/1.8*64);i++)
    {
        HAL_GPIO_WritePin(PUL_GPIO_Port,PUL_Pin,1);
        delay_us(us);
        HAL_GPIO_WritePin(PUL_GPIO_Port,PUL_Pin,0);
        delay_us(us);
    }

}

void delay_us(uint16_t us)
{
	uint16_t counter = 0;
	__HAL_TIM_SetAutoreload(&htim4, us);		//设置ARR�?
	__HAL_TIM_SetCounter(&htim4, counter); 		//设置CNT初始�?
	HAL_TIM_Base_Start(&htim4); 				//启动时基，开始计�?

	while(counter != us) //counter是读取CNT�?得到的�?�，即若CNT还没有到�?设定的us，则�?直计数，直到时间到了再跳出循�?
	{
		counter = __HAL_TIM_GetCounter(&htim4); // 获取定时器当前计�?
	}
	HAL_TIM_Base_Stop(&htim4); // 停止定时�?
}