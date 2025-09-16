#ifndef __JY901S_H
#define __JY901S_H  

#include "stm32f1x.h"   
#include "wit_c_sdk.h" 

#define HUART huart1


#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
static char s_cDataUpdate = 0;
static void AutoScanSensor(void);
static void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum);

void JY901S_Init(void);
void JY901S_Update(void);

#endif



