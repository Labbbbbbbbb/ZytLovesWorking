#include "RS485.h"
uint8_t RS485_RxBuffer[12];
uint8_t RS485_TxData[8];
uint8_t RS485_RxData[8];

void RS485_Init(void)
{
    HAL_UART_Receive_IT(huart_485,RS485_RxBuffer,sizeof(RS485_RxBuffer));
}

void RS485_Transmit(uint8_t* TxData)
{
    HAL_GPIO_WritePin(EN_PORT,EN_PIN,1);
    uint8_t tx[12]={0};
    uint16_t crc=0;
    tx[0]=0x0A;
    tx[1]=0xBB;
    for(uint8_t i=0;i<8;i++)
    {
        tx[i+2]=TxData[i];
        crc+=TxData[i];
    }
    tx[10]=(uint8_t)(crc>>8);   //校验位的高位
    tx[11]=(uint8_t)(crc);      //校验位的低位

    HAL_UART_Transmit(huart_485,tx,sizeof(tx),0xff);
    HAL_GPIO_WritePin(EN_PORT,EN_PIN,0);
}

void RS485_Receive_Decode(uint8_t* RxData)
{
    if(RxData[0]==0x0A &&RxData[1]==0xBB)
    {
        uint16_t crc=0;
        for(uint8_t i=0;i<8;i++)
        {
            crc+=RxData[i+2];            
        }
        uint8_t crc_h=(uint8_t)(crc>>8);
        uint8_t crc_l=(uint8_t)(crc);
        if(crc_h==RxData[10] && crc_l==RxData[11])
        {
            for(uint8_t i=0;i<8;i++)
            {
                RS485_RxData[i]=RxData[i+2];            
            }
        }
    }

}
/**
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)   //暂时把u2的回调函数删了
{

   
    if(huart->Instance == UART_485_Instance)
    {
        RS485_Receive_Decode(RS485_RxBuffer);
        HAL_UART_Receive_IT(huart_485,RS485_RxBuffer,sizeof(RS485_RxBuffer));

    }

}

**/