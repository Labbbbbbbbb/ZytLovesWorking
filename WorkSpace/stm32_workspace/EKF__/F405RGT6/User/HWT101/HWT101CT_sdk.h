#ifndef HWT101CT_SDK_H
#define HWT101CT_SDK_H

#ifdef __cplusplus
extern "C" {
#endif
/**--------------------------C-----------------------------**/

#include "main.h"
#include "usart.h"
#include "wit_c_sdk.h"
#include "bsp_delay.h"
#include<stdio.h>

#define  HUART_CURRENT  huart2

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80


extern float fAcc[3], fGyro[3], fAngle[3];
extern const uint32_t c_uiBaud[10];

extern float temp[9];
extern uint8_t s;
typedef union
{
    float data;
    uint8_t byte[4];
}float_to_byte;

extern float_to_byte data_imu[13];

void trans_float(UART_HandleTypeDef huart);


void CopeCmdData(unsigned char ucData);
void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
void Delayms(uint16_t ucMs);
void ProcessData(void);

void UsartInit(UART_HandleTypeDef huart, USART_TypeDef* USART,unsigned int uiBaud);
uint8_t HW101_Init(void);


#ifdef __cplusplus
};
/**-----------------------------C++---------------------------**/
#endif

#endif // !HWT101CT_SDK_H
