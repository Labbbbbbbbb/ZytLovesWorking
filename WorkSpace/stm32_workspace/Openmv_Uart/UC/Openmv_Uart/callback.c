#include "mv_decode.h"

uint8_t state1;
uint8_t state2;
uint8_t state3;
/*
uint8_t u1_buffer[1];
uint8_t u2_buffer[1];
uint8_t u3_buffer[1];
*/
uint8_t receive_buffer1[9];
uint8_t receive_buffer2[5];
uint8_t receive_buffer3[9];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//
{
	if(huart ->Instance == USART1)  //来自云台openmv(坐标数据)
	{
        if(receive_buffer1[0]==0xFF && receive_buffer1[1]==0xFE)
        {
            if(receive_buffer1[7]==0xFE && receive_buffer1[8]==0xFF)
            {
                Gimbal_Decode();
            }
        }
        HAL_UART_Receive_IT(&huart1,receive_buffer1,sizeof(receive_buffer1));
    }
    if(huart ->Instance == USART2)  //来自下方openmv的u2(错误信息)
	{
        if(receive_buffer2[0]==0xFF && receive_buffer2[1]==0xFE)
        {
            if(receive_buffer2[3]==0xFE && receive_buffer2[4]==0xFF)
            {
                ErrorFlag_Decode();
            }
        }
        HAL_UART_Receive_IT(&huart2,receive_buffer2,sizeof(receive_buffer2));
    }
    if(huart ->Instance == USART3)  //来自下方openmv的u3(坐标数据)
	{
        if(receive_buffer3[0]==0xFF && receive_buffer3[1]==0xFE)
        {
            if(receive_buffer3[7]==0xFE && receive_buffer3[8]==0xFF)
            {
                Lock_Decode();
            }
        }
        HAL_UART_Receive_IT(&huart3,receive_buffer3,sizeof(receive_buffer3));
    }
}