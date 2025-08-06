#include "ptz.h"
#include "tim.h"
void Ptz_setangle(int32_t angle){				//168MH/Psc=1MHz,1us  2500--->2.5ms
	if (angle>1000) angle=1000;
	if (angle<-1000) angle=-1000;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500+angle);
}
