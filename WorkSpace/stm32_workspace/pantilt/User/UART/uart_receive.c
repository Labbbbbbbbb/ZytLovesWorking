#include "uart_receive.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART2)
    {
        if(Receivebuffer[0]==0xAA&&Receivebuffer[1]==0xFF&&Receivebuffer[6]==0xFF&&Receivebuffer[7]==0xAA)
        {
            int16_t spe1_tmp=0;
            int16_t spe2_tmp=0;
            spe1_tmp = (Receivebuffer[2] << 8) | Receivebuffer[3];  // 16-bit 组合
            spe2_tmp = (Receivebuffer[4] << 8) | Receivebuffer[5];      //高位在前低位在后，大端模式
            spe1=spe1_tmp;
            spe2=spe2_tmp;
        }
        HAL_UART_Receive_IT(&huart2,Receivebuffer,sizeof(Receivebuffer));
    }
}

        //Speed = -(Speed&~(1<<15));
